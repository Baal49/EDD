#include "arbolito.hpp"
#include "nodosenlazadosp.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

arbolito::arbolito() : root(nullptr), jefe(nullptr) {}

arbolito::~arbolito() {
    liberarArbol(root);
}

void arbolito::liberarArbol(Nodos* nodo) {
    if (nodo == nullptr) return;
    liberarArbol(nodo->hijoizq);
    liberarArbol(nodo->hijoder);
    delete nodo;
}

void arbolito::imprimirNodo(const Nodos* n)const{
    if(n!=nullptr){
        std::cout<<"id: "<<n->id<<" nombre: "<<n->nombre<<" apellido: "<<n->apellido<<" edad: "<<n->edad<<" nombre: "<<n->nombre;
    }
}
// -------------------------------------------------------------------
// Carga desde CSV
// Columnas: id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
// -------------------------------------------------------------------
static std::string siguienteCampo(const std::string& linea, size_t& posicion) {
    size_t coma = linea.find(',', posicion);
    std::string campo;
    if (coma == std::string::npos) {
        campo = linea.substr(posicion);
        posicion = linea.size();
    } else {
        campo = linea.substr(posicion, coma - posicion);
        posicion = coma + 1;
    }
    return campo;
}

bool arbolito::cargarDesdeCSV(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str());
    std::cout<<"hola mundo";
    if (!archivo.is_open()) {
        std::cout<<"hola mundo2";
        return false;
    }
    std::cout<<"hola mund3";
    nodosenlazadosp pool; // lista enlazada
    std::string linea;
    bool primeraLinea = true;

    while (std::getline(archivo, linea)) {
        if (!linea.empty() && linea[linea.size() - 1] == '\r') {
            linea.erase(linea.size() - 1);
        }
        if (linea.empty()) continue;

        if (primeraLinea) {
            primeraLinea = false;
            continue; // saltar encabezado
        }

        size_t posicion = 0;
        std::string idTexto      = siguienteCampo(linea, posicion);
        std::string nombre       = siguienteCampo(linea, posicion);
        std::string apellido     = siguienteCampo(linea, posicion);
        std::string generoTexto  = siguienteCampo(linea, posicion);
        std::string edadTexto    = siguienteCampo(linea, posicion);
        std::string idjefeTexto  = siguienteCampo(linea, posicion);
        std::string mueltoTexto  = siguienteCampo(linea, posicion);
        std::string presoTexto   = siguienteCampo(linea, posicion);
        std::string fuejefeTexto = siguienteCampo(linea, posicion);
        std::string esjefeTexto  = siguienteCampo(linea, posicion);

        if (idTexto.empty()) continue;

        int id = std::atoi(idTexto.c_str());
        char genero = generoTexto.empty() ? 'H' : generoTexto[0];
        int edad = std::atoi(edadTexto.c_str());
        int idjefe = std::atoi(idjefeTexto.c_str());
        bool muelto = (std::atoi(mueltoTexto.c_str()) == 1);
        bool preso = (std::atoi(presoTexto.c_str()) == 1);
        bool fuejefe = (std::atoi(fuejefeTexto.c_str()) == 1);
        bool esjefe = (std::atoi(esjefeTexto.c_str()) == 1);

        Nodos* nodo = new Nodos(id, nombre, apellido, genero, edad, idjefe, muelto, preso, fuejefe, esjefe);
        pool.agregar(nodo);

        if (esjefe) {
            jefe = nodo;
        }
    }
    archivo.close();

    // Segunda pasada: enlazar cada nodo como hijo (sucesor) de su jefe (id_boss)
    for (nodosenlazadosp::Entrada* Entrada = pool.head; Entrada != nullptr; Entrada = Entrada->siguiente) {
        Nodos* nodo = Entrada->nodo;
        if (nodo->idjefe == 0) {
            root = nodo; // la raiz del arbol (capo historico / primer jefe)
            continue;
        }
        Nodos* padre = pool.buscarPorId(nodo->idjefe);
        if (padre == nullptr) {
            // id_boss no encontrado: se ignora como huerfano (dato invalido)
            continue;
        }
        nodo->padre = padre;
        if (padre->hijoizq == nullptr) {
            padre->hijoizq = nodo;
        } else if (padre->hijoder == nullptr) {
            padre->hijoder = nodo;
        }
        // si ya tiene 2 hijos, se ignora (el arbol es binario: max 2 sucesores)
    }

    return true;
}

// -------------------------------------------------------------------
// Busquedas
// -------------------------------------------------------------------
Nodos* arbolito::buscarPorId(int id) const {
    return buscarPorIdRec(root, id);
}

Nodos* arbolito::buscarPorIdRec(Nodos* nodo, int id) const {
    if (nodo == nullptr) return nullptr;
    if (nodo->id == id) return nodo;
    Nodos* encontrado = buscarPorIdRec(nodo->hijoizq, id);
    if (encontrado != nullptr) return encontrado;
    return buscarPorIdRec(nodo->hijoder, id);
}

Nodos* arbolito::primerLibreVivo(Nodos* nodo, bool incluirPresos) const {
    if (nodo == nullptr) return nullptr;
    if (!nodo->muelto && (incluirPresos || !nodo->preso)) {
        return nodo;
    }
    Nodos* encontrado = primerLibreVivo(nodo->hijoizq, incluirPresos);
    if (encontrado != nullptr) return encontrado;
    return primerLibreVivo(nodo->hijoder, incluirPresos);
}

Nodos* arbolito::obtenerHermano(Nodos* nodo) const {
    if (nodo == nullptr || nodo->padre == nullptr) return nullptr;
    Nodos* padre = nodo->padre;
    if (padre->hijoizq == nodo) return padre->hijoder;
    if (padre->hijoder == nodo) return padre->hijoizq;
    return nullptr;
}

Nodos* arbolito::buscarAncestroConDosSucesoresLibres(Nodos* inicio, bool incluirPresos) const {
    if (inicio == nullptr) return nullptr;
    Nodos* ancestro = inicio->padre;
    while (ancestro != nullptr) {
        Nodos* izquierdo = ancestro->hijoizq;
        Nodos* derecho = ancestro->hijoder;
        bool izquierdoLibre = (izquierdo != nullptr) && !izquierdo->muelto && (incluirPresos || !izquierdo->preso);
        bool derechoLibre = (derecho != nullptr) && !derecho->muelto && (incluirPresos || !derecho->preso);
        if (izquierdoLibre && derechoLibre) {
            return ancestro;
        }
       ancestro = ancestro->padre;
    }
    return nullptr;
}

// Reglas de sucesion

Nodos* arbolito::elegirSucesor(Nodos* jefeFallecido) {
    if (jefeFallecido == nullptr) return nullptr;

    // Sucesores del jefe que no esten presos
    Nodos* candidato = primerLibreVivo(jefeFallecido->hijoizq, false);
    if (candidato == nullptr) candidato = primerLibreVivo(jefeFallecido->hijoder, false);
    if (candidato != nullptr) return candidato;

    //si no tiene sucesores se busca al compañero sucesor. Si el hermano esta vivo, libre y no tiene sucesores propios, el se vuelve jefe de una
    Nodos* hermano = obtenerHermano(jefeFallecido);
    if (hermano != nullptr) {
        bool hermanoLibre = !hermano->muelto && !hermano->preso;
        if (hermanoLibre && hermano->hijoizq == nullptr && hermano->hijoder == nullptr) {
            return hermano;
        }
        candidato = primerLibreVivo(hermano, false);
        if (candidato != nullptr) return candidato;
    }

    // si ni el, ni su anterior jefe asigno otro sucesor utilizable se busca el compañero sucesor de su anterior jefe. Si ese esta vivo, libre y sin sucesores, se vuelve jefe
    Nodos* jefeAnterior = jefeFallecido->padre;
    if (jefeAnterior != nullptr) {
        Nodos* hermanoJefeAnterior = obtenerHermano(jefeAnterior);
        if (hermanoJefeAnterior != nullptr) {
            bool libre2 = !hermanoJefeAnterior->muelto && !hermanoJefeAnterior->preso;
            if (libre2 && hermanoJefeAnterior->hijoizq == nullptr && hermanoJefeAnterior->hijoder == nullptr) {
                return hermanoJefeAnterior;
            }
            candidato = primerLibreVivo(hermanoJefeAnterior, false);
            if (candidato != nullptr) return candidato;
        }
    }

    // si no encontro sucesor en el arbol del jefe de su jefe entonces busca el jefe viejo mas cercano que tenga dos sucesocres libres.
    Nodos* ancestro = buscarAncestroConDosSucesoresLibres(jefeFallecido, false);
    if (ancestro != nullptr) {
        candidato = primerLibreVivo(ancestro->hijoizq, false);
        if (candidato == nullptr) candidato = primerLibreVivo(ancestro->hijoder, false);
        if (candidato != nullptr) return candidato;
    }

    // si todos los jefes/sucesores libres murieron: hace la misma busqueda pero permite candidatos presos como referencia, hasta encontrar a alguien vivo y no preso en cualquier parte del arbol
    candidato = primerLibreVivo(root, false);
    return candidato; // puede ser nullptr si absolutamente nadie esta vivo y libre
}

void arbolito::manejarMuerteJefe(Nodos* jefeFallecido) {
    jefeFallecido->esjefe = false;
    jefeFallecido->fuejefe = true;

    Nodos* sucesor = elegirSucesor(jefeFallecido);
    if (sucesor != nullptr) {
        if (jefe != nullptr && jefe->id != sucesor->id) {
            jefe->esjefe = false;
        }
        sucesor->esjefe = true;
        sucesor->fuejefe = true;
        jefe = sucesor;
        std::cout << "\n*** El jefe " << jefeFallecido->nombre << " " << jefeFallecido->apellido
                  << " ha muerto. Nuevo jefe: " << sucesor->nombre << " " << sucesor->apellido
                  << " (id: " << sucesor->id << ") ***\n";
    } else {
        jefe = nullptr;
        std::cout << "\n*** El jefe " << jefeFallecido->nombre << " " << jefeFallecido->apellido
                  << " ha muerto y no se encontro ningun sucesor vivo y libre. "
                  << "La familia se ha quedado sin jefe. ***\n";
    }
}

void arbolito::jubilarJefe(Nodos* jefe) {
    // Regla: si el jefe paso de 70 años o va a prision, el puesto pasa al primer sucesor libre y vivo de SU PROPIO arbol.
    jefe->esjefe = false;
    jefe->fuejefe = true;

    Nodos* sucesor = primerLibreVivo(jefe->hijoizq, false);
    if (sucesor == nullptr) sucesor = primerLibreVivo(jefe->hijoder, false);

    if (sucesor != nullptr) {
        sucesor->esjefe = true;
        sucesor->fuejefe = true;
        jefe = sucesor;
        std::cout << "\n*** " << jefe->nombre << " " << jefe->apellido
                  << " deja el puesto de jefe (edad/prision). Nuevo jefe: "
                  << sucesor->nombre << " " << sucesor->apellido
                  << " (id: " << sucesor->id << ") ***\n";
    } else {
        jefe = nullptr;
        std::cout << "\n*** " << jefe->nombre << " " << jefe->apellido
                  << " deja el puesto de jefe (edad/prision) y no hay sucesor "
                  << "disponible en su arbol. La familia se ha quedado sin jefe. ***\n";
    }
}

void arbolito::verificarCondicionJefe(Nodos* nodo) {
    if (nodo == nullptr || jefe == nullptr || nodo->id != jefe->id) {
        return; // la regla automatica solo aplica al jefe ACTUAL
    }
    if (nodo->muelto) {
        manejarMuerteJefe(nodo);
        return;
    }
    if (nodo->edad > 70 || nodo->preso) {
        jubilarJefe(nodo);
    }
}

// -------------------------------------------------------------------
// Edicion de nodos
// -------------------------------------------------------------------
bool arbolito::editarNodo(int id, const std::string& nombre, const std::string& apellido, char genero,
                          int edad, bool muelto, bool preso, bool fuejefe, bool esjefe) {
    Nodos* nodo = buscarPorId(id);
    if (nodo == nullptr) return false;

    nodo->nombre = nombre;
    nodo->apellido = apellido;
    nodo->genero = genero;
    nodo->edad = edad;
    nodo->muelto = muelto;
    nodo->preso = preso;
    nodo->fuejefe = fuejefe;
    nodo->esjefe = esjefe;

    if (esjefe) {
        if (jefe != nullptr && jefe->id != nodo->id) {
            jefe->esjefe = false;
        }
        jefe = nodo;
    }

    verificarCondicionJefe(nodo);
    return true;
}

// -------------------------------------------------------------------
// Impresion
// -------------------------------------------------------------------
void arbolito::mostrarSucesionRec(Nodos* nodo, int profundidad) const {
    if (nodo == nullptr) return;

    if (!nodo->muelto) {
        for (int i = 0; i < profundidad; ++i) std::cout << "   ";
        std::cout << "- " << nodo->nombre << " " << nodo->apellido
                  << " (id:" << nodo->id << ", edad:" << nodo->edad << ")";
        if (nodo->esjefe) std::cout << "  [JEFE ACTUAL]";
        if (nodo->preso) std::cout << "  [EN PRISION]";
        if (nodo->fuejefe && !nodo->esjefe) std::cout << "  [EX-JEFE]";
        std::cout << "\n";
    }

    mostrarSucesionRec(nodo->hijoizq, profundidad + 1);
    mostrarSucesionRec(nodo->hijoder, profundidad + 1);
}

void arbolito::mostrarLineaSucesion() const {
    std::cout << "\n===== LINEA DE SUCESION ACTUAL (SOLO VIVOS) =====\n";
    if (root == nullptr) {
        std::cout << "El arbol esta vacio.\n";
        return;
    }
    mostrarSucesionRec(root, 0);
    std::cout << "==================================================\n";
}
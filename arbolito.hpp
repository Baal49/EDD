#ifndef ARBOLITO_HPP
#define ARBOLITO_HPP

#include <string>
#include "nodos.hpp"

class arbolito {
private:
    Nodos* root;
    Nodos* jefe; // puntero rapido al jefe actual

    // ---- utilidades internas de recorrido ----
    Nodos* buscarPorIdRec(Nodos* nodo, int id) const;
    void mostrarSucesionRec(Nodos* nodo, int profundidad) const;
    void liberarArbol(Nodos* nodo);

    // Busca en preorden (nodo, izquierda, derecha) el primer miembro vivo, ademas exige que no este preso.
    Nodos* primerLibreVivo(Nodos* nodo, bool incluirPresos) const;

    // Devuelve el hermano (el "compañero sucesor") de un nodo dentro del arbol.
    Nodos* obtenerHermano(Nodos* nodo) const;

    // Busca hacia arriba (ancestros) el jefe mas cercano que tenga DOS
    // sucesores vivos y libres (fuera de prision).
    Nodos* buscarAncestroConDosSucesoresLibres(Nodos* inicio, bool incluirPresos) const;

    // Aplica toda la cadena de reglas de sucesion para encontrar
    // reemplazo cuando el jefe indicado muere.
    Nodos* elegirSucesor(Nodos* jefeFallecido);

    void manejarMuerteJefe(Nodos* jefeFallecido);
    void jubilarJefe(Nodos* jefe); // por edad > 70 o por ir a prision

public:
    arbolito();
    ~arbolito();

    bool cargarDesdeCSV(const std::string& ruta);

    void mostrarLineaSucesion() const;

    Nodos* buscarPorId(int id) const;

    // Cambia los datos de un nodo (nunca su id ni el id de su jefe/padre).
    bool editarNodo(int id, const std::string& nombre, const std::string& apellido, char genero,
                   int edad, bool muelto, bool preso, bool fuejefe, bool esjefe);

    // Revisa si, tras una edicion, el nodo afectado es el jefe actual y si
    // se cumplen condiciones automaticas (murio, cumplio 70+, fue a prision).
    void verificarCondicionJefe(Nodos* nodo);

    void imprimirNodo(const Nodos* n) const;

    Nodos* obtenerJefe() const { return jefe; }
    bool estaVacio() const { return root == nullptr; }
};

#endif
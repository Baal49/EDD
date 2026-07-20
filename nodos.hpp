#ifndef NODOS_HPP
#define NODOS_HPP

#include <string>

// Representa a un miembro de la familia dentro del la pelea por sucesion.
// Cada nodo puede tener hasta DOS hijos (sucesor izquierdo y sucesor derecho), que son los sucesores asignados por ese miembro cuando fue/es jefe.
struct Nodos {
    int id;
    std::string nombre;
    std::string apellido;
    char genero;      // 'H' (hombre) o 'M' (mujer)
    int edad;
    int idjefe;       // id del padre (jefe que lo nombro sucesor). 0 si es la raiz.
    bool muelto;       // si e true = muerto
    bool preso;       // si e true = en prision
    bool fuejefe;      // si e true = fue jefe en algun momento
    bool esjefe;       // si e true = es el jefe actual

    Nodos* hijoder;   // primer sucesor asignado
    Nodos* hijoizq;   // segundo sucesor asignado
    Nodos* padre;     // puntero al jefe que lo nombro (para recorrer el arbol hacia arriba)

    Nodos(int id_, const std::string& nombre_, const std::string& apellido_, char genero_,
          int edad_, int idjefe_, bool muelto_, bool preso_, bool fuejefe_, bool esjefe_)
        : id(id_), nombre(nombre_), apellido(apellido_), genero(genero_), edad(edad_),
          idjefe(idjefe_), muelto(muelto_), preso(preso_), fuejefe(fuejefe_), esjefe(esjefe_),
          hijoder(nullptr), hijoizq(nullptr), padre(nullptr) {}
};

#endif
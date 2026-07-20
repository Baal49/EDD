#ifndef NODE_POOL_HPP
#define NODE_POOL_HPP

#include "nodos.hpp"

// nodosenlazadosp: lista enlazada simple hecha a mano.
// Se usa SOLO como bodega temporal mientras se leen los nodos del CSV
// y se va armando el arbol (no se pueden usar vectores, asi que
// implementamos nuestra propia lista enlazada minima).
class nodosenlazadosp {
public:
    struct Entrada {
        Nodos* nodo;
        Entrada* siguiente;
        Entrada(Nodos* n, Entrada* nx) : nodo(n), siguiente(nx) {}
    };

    Entrada* head;
    Entrada* tail;

    nodosenlazadosp() : head(nullptr), tail(nullptr) {}

    ~nodosenlazadosp() {
        Entrada* act = head;
        while (act != nullptr) {
            Entrada* siguiente = act->siguiente;
            delete act;       // <--- Correcto: borramos el nodo actual
            act = siguiente;  // <--- Avanzamos al siguiente
        }
    }

    // Se agrega al final para conservar el orden en que aparecen en el CSV
    // (asi el primer sucesor listado para un jefe queda como hijo izquierdo).
    void agregar(Nodos* n) {
        Entrada* e = new Entrada(n, nullptr);
        if (tail == nullptr) {
            head = e;
            tail = e;
        } else {
            tail->siguiente = e;
            tail = e;
        }
    }

    Nodos* buscarPorId(int id) const {
        for (Entrada* actual = head; actual != nullptr; actual = actual->siguiente) {
            if (actual->nodo->id == id) return actual->nodo;
        }
        return nullptr;
    }
};

#endif
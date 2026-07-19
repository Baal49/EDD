#ifndef NODE_POOL_H
#define NODE_POOL_H

#include "nodos.h"

// NodePool: lista enlazada simple hecha a mano.
// Se usa SOLO como bodega temporal mientras se leen los nodos del CSV
// y se va armando el arbol (no se pueden usar vectores, asi que
// implementamos nuestra propia lista enlazada minima).
class nodosenlazadosp {
public:
    struct Entry {
        Nodos* nodo;
        Entry* siguiente;
        Entry(Nodos* n, Entry* nx) : nodo(n), siguiente(nx) {}
    };

    Entry* head;
    Entry* tail;

    nodosenlazadosp() : head(nullptr), tail(nullptr) {}

    ~nodosenlazadosp() {
        // Solo se liberan los "Entry" (nodos de la lista), NO los Nodos*,
        // porque esos ya pertenecen al arbol y los libera FamilyTree.
        Entry* act = head;
        while (act != nullptr) {
            Entry* siguiente = act->siguiente;
            delete siguiente;
            act = siguiente;
        }
    }

    // Se agrega al final para conservar el orden en que aparecen en el CSV
    // (asi el primer sucesor listado para un jefe queda como hijo izquierdo).
    void add(Nodos* n) {
        Entry* e = new Entry(n, nullptr);
        if (tail == nullptr) {
            head = e;
            tail = e;
        } else {
            tail->siguiente = e;
            tail = e;
        }
    }

    Nodos* buscarporid(int id) const {
        for (Entry* act = head; act != nullptr; act = act->siguiente) {
            if (act->nodo->id == id) return act->nodo;
        }
        return nullptr;
    }
};

#endif
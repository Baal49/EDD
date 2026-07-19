#include <iostream>
#include <string>
#include <cstdlib>
#include "arbolito.h"

static void mostrarMenu() {
    std::cout << "\n============ SISTEMA DE SUCESION FAMILIAR ============\n";
    std::cout << "1. Mostrar linea de sucesion actual (solo vivos)\n";
    std::cout << "2. Ver datos de un miembro\n";
    std::cout << "3. Editar datos de un miembro\n";
    std::cout << "4. Salir\n";
    std::cout << "Seleccione una opcion: ";
}

static int leerInt() {
    std::string linea;
    std::getline(std::cin, linea);
    if (linea.empty()) return -1;
    return std::atoi(linea.c_str());
}

static void editarMiembro(arbolito& tree) {
    std::cout << "Ingrese el ID del miembro a editar: ";
    int id = leerInt();
    Nodos* n = tree.buscarPorId(id);
    if (n == nullptr) {
        std::cout << "No existe un miembro con ese ID.\n";
        return;
    }

    std::cout << "\nDatos actuales:\n";
    tree.imprimirNodo(n);
    std::cout << "\n(Deje vacio y presione ENTER para conservar el valor actual)\n";

    std::string nombre, apellido, generoStr, edadStr, mueltoStr, carcelStr, erajefeStr, esjefeStr;

    std::cout << "Nuevo nombre [" << n->nombre << "]: ";
    std::getline(std::cin, nombre);
    if (nombre.empty()) nombre = n->nombre;

    std::cout << "Nuevo apellido [" << n->apellido << "]: ";
    std::getline(std::cin, apellido);
    if (apellido.empty()) apellido = n->apellido;

    std::cout << "Nuevo genero H/M [" << n->genero << "]: ";
    std::getline(std::cin, generoStr);
    char genero = generoStr.empty() ? n->genero : generoStr[0];

    std::cout << "Nueva edad [" << n->edad << "]: ";
    std::getline(std::cin, edadStr);
    int edad = edadStr.empty() ? n->edad : std::atoi(edadStr.c_str());

    std::cout << "Esta muerto? 1=Si 0=No [" << (n->muelto ? 1 : 0) << "]: ";
    std::getline(std::cin, mueltoStr);
    bool muelto = mueltoStr.empty() ? n->muelto : (std::atoi(mueltoStr.c_str()) == 1);

    std::cout << "Esta en prision? 1=Si 0=No [" << (n->preso ? 1 : 0) << "]: ";
    std::getline(std::cin, carcelStr);
    bool preso = carcelStr.empty() ? n->preso : (std::atoi(carcelStr.c_str()) == 1);

    std::cout << "Fue jefe? 1=Si 0=No [" << (n->fuejefe ? 1 : 0) << "]: ";
    std::getline(std::cin, erajefeStr);
    bool fuejefe = erajefeStr.empty() ? n->fuejefe : (std::atoi(erajefeStr.c_str()) == 1);

    std::cout << "Es el jefe actual? 1=Si 0=No [" << (n->esjefe ? 1 : 0) << "]: ";
    std::getline(std::cin, esjefeStr);
    bool esjefe = esjefeStr.empty() ? n->esjefe : (std::atoi(esjefeStr.c_str()) == 1);

    tree.editarNodo(id, nombre, apellido, genero, edad, muelto, preso, fuejefe, esjefe);
    std::cout << "\nDatos actualizados correctamente.\n";
}

int main() {
    arbolito tree;

    if (!tree.cargarDesdeCSV("data.csv")) {
        std::cout << "No se pudo abrir data.csv. Verifique que el archivo "
                  << "exista dentro de la carpeta bin junto al ejecutable.\n";
        return 1;
    }

    std::cout << "Datos de la familia cargados correctamente.\n";
    if (tree.obtenerJefe() != nullptr) {
        std::cout << "Jefe actual: " << tree.obtenerJefe()->nombre << " "
                  << tree.obtenerJefe()->apellido << " (id:" << tree.obtenerJefe()->id << ")\n";
    } else {
        std::cout << "error, no hay ningun jefe marcado en el CSV.\n";
    }

    int option = -1;
    while (option != 4) {
        mostrarMenu();
        option = leerInt();

        switch (option) {
            case 1:
                tree.mostrarLineaSucesion();
                break;
            case 2: {
                std::cout << "Ingrese el ID del miembro a consultar: ";
                int id = leerInt();
                Nodos* n = tree.buscarPorId(id);
                tree.imprimirNodo(n);
                break;
            }
            case 3:
                editarMiembro(tree);
                break;
            case 4:
                std::cout << "Saliendo del programa. Bye bye.\n";
                break;
            default:
                std::cout << "Opcion invalida, intente de nuevo.\n";
        }
    }

    return 0;
}
# Sistema de Sucesion - Familia Mafia (prototipo en C++)

## Estructura del proyecto

```
familia_mafia/
├── bin/            <- aqui se genera el ejecutable "programa" y aqui vive data.csv
│   └── data.csv    <- datos de prueba (15 miembros)
├── src/            <- codigo fuente (unicamente .h y .cpp)
│   ├── node.hpp
│   ├── node_pool.hpp
│   ├── family_tree.hpp
│   ├── family_tree.cpp
│   └── main.cpp
```

## Compilar y ejecutar

Desde la raiz del proyecto (donde esta el Makefile):

```
make
cd C:\Users\reina\Desktop\eddatos\EDD   
.\proyectocasanostra.exe  
```


El programa carga automaticamente `EDD/casanostra.csv` al iniciar y luego muestra
un menu que se repite hasta que el usuario elige la opcion "Salir".

## Modelo de datos

El arbol es un **arbol binario**: cada miembro (nodo) tiene como maximo DOS
hijos, que representan a los DOS sucesores que ese miembro asigno cuando fue
jefe. La columna `id_boss` del CSV es el id del padre en el arbol (el jefe
que lo nombro sucesor). El nodo con `id_boss = 0` es la raiz del arbol.

No se usa `std::vector` en ningun lugar del proyecto: para guardar los
nodos temporalmente mientras se arma el arbol desde el CSV se implemento
una lista enlazada propia (`NodePool` en `node_pool.hpp`).

## Interpretacion de las reglas de sucesion

El enunciado describe una cadena de reglas para elegir sucesor cuando el
jefe muere. Se implemento de la siguiente manera (ver `chooseSuccessor` en
`family_tree.cpp`):

1. Se busca en el propio arbol del jefe fallecido (sus dos sucesores y los
   descendientes de estos) el primer vivo y fuera de prision, en preorden
   (izquierda antes que derecha).
2. Si no tiene sucesores vivos/libres, se revisa al "compañero sucesor"
   (su hermano en el arbol, el otro sucesor que asigno el jefe anterior).
   Si ese hermano esta vivo, libre y no tiene sucesores propios, se
   convierte el mismo en jefe. Si tiene sucesores, se busca el primero
   vivo/libre en su arbol.
3. Si tampoco se encuentra ahi, se sube un nivel mas: se revisa al
   compañero sucesor del jefe anterior (el "tio" dentro del arbol) y su
   arbol, con la misma logica.
4. Si aun asi no aparece un sucesor, se busca el ancestro mas cercano que
   tenga sus DOS sucesores vivos y libres, y se toma el primero de ellos.
5. Si absolutamente todos los libres (vivos y fuera de prision) estan
   muertos, como ultimo recurso se recorre el arbol completo buscando a
   cualquier miembro vivo y libre, sin importar la rama a la que
   pertenezca (esto cubre la regla de "recurrir a los que estan en
   prision hasta encontrar un sucesor libre").

Ademas, si el jefe actual supera los 70 años o es enviado a prision, el
puesto pasa automaticamente al primer sucesor vivo y libre de su propio
arbol (sin recorrer hacia otras ramas), tal como lo pide el enunciado.

Estas reglas se disparan automaticamente cada vez que se edita un nodo
(opcion 3 del menu) y ese nodo resulta ser el jefe actual.

## Datos de prueba incluidos

`EDD/casanostra.csv` ya trae una familia de 15 miembros organizada como arbol
binario completo (el jefe actual es Salvatore Esposito, id 2), con varios
miembros muertos y presos para poder probar las distintas reglas de
sucesion editando nodos desde el menu.

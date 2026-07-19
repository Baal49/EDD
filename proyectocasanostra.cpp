#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
template <typename N>
class nodo{
    public:
        int id=0;
        string nombre;
        string apellido;
        string genero;
        int edad=0;
        int idjefe=0;
        bool muelto=0;
        bool preso=0;
        bool fuejefe=0;
        bool esjefe=0;
        nodo* hijoder=nullptr;
        nodo*  hijoizq=nullptr;
    nodo(int id,string nombre,string apellido,string genero,int age,int idboss,bool dead,bool jail,bool wasboos,bool isboss){
        this->id=id;
        this->nombre=nombre;
        this->apellido=apellido;
        this->edad=age;
        this->idjefe=idboss;
        this->muelto=dead;
        this->preso=jail;
        this->fuejefe=wasboos;
        this->esjefe=isboss;
    }
};
template <typename C>
class Arbol{
    public:
    nodo<C>* jefe=nullptr;
    int nivel=0;
    void insertar(int id,string nombre,string apellido,string genero,int age,int idboss,bool dead,bool jail,bool wasboos,bool isboss){
        cout<<"isboss: "<<isboss<<"dead: "<<dead<<endl;
        if(this->jefe==nullptr&&isboss==1&&dead==0){
            nodo<C>* nodito=new nodo<C>(id,nombre,apellido,genero,age,idboss,dead,jail,wasboos,isboss);
            jefe=nodito;
            return;
        }
        else{
        nodo<C>* padre=nullptr;
        nodo<C>* act=this->jefe;
        while (act!=nullptr&&act->id!=id)
        {
            padre=act;
            if(act->hijoizq==nullptr){

                act=act->hijoder;
            }
            else if(act->hijoder==nullptr){
                act=act->hijoizq;
            }
        }
        if(act!=nullptr){
            return;
        }
        nodo<C>* nodito=new nodo<C>(id,nombre,apellido,genero,age,idboss,dead,jail,wasboos,isboss);
        if(padre->hijoizq==nullptr){
            padre->hijoizq=nodito;
            nivel++;
        }
        else{
            padre->hijoder=nodito;
            nivel++;
        }
        
    }
}
void mostrardato(){
   datos(jefe);
}
void datos(nodo<C>* act){
    if(act==nullptr){
        return;
    }
    cout<<"Nombre: "<<act->nombre<<" apellido: "<<act->apellido<<endl;
    if(act->hijoder!=nullptr){
        cout<<"familia:";
    }
    datos(act->hijoder);
    if(act->hijoizq!=nullptr){
        cout<<"sucesor:";    
    }
    datos(act->hijoizq);
}
};
int main(){
 Arbol<int> arbolito;
 ifstream archivo("casanostra.csv");
 vector<vector<string>> datos;
 string linea;
 std::string encabezado;
 std::getline(archivo, encabezado); 
 while(getline(archivo,linea)){
    vector<string>fila;
    stringstream ss(linea);
    string celda;
    while(getline(ss,celda,',')){
        fila.push_back(celda);
    }
    datos.push_back(fila);
 }
 for (const auto& fila : datos) {
        for (const auto& celda : fila) {
            std::cout << celda << "\t";
        }
        std::cout << std::endl;
    }
for(int i=0;i<datos.size();i++){
    arbolito.insertar(stoi(datos[i][0]),datos[i][1],datos[i][2],datos[i][3],(stoi(datos[i][4])),(stoi(datos[i][5])!=0),(stoi(datos[i][6])!=0),(stoi(datos[i][7])!=0),(stoi(datos[i][8])!=0),(stoi(datos[i][9])!=0));
}
 
 //arbolito.insertar(stoi(datos[1][0]),datos[1][1],datos[1][2],datos[1][3],(stoi(datos[1][4])),(stoi(datos[1][5])!=0),(stoi(datos[1][6])!=0),(stoi(datos[1][7])!=0),(stoi(datos[1][8])!=0),(stoi(datos[1][9])!=0));
 arbolito.mostrardato();
}
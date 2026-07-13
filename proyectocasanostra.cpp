#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
template <typename N>
class nodo{
    public:
        int id=0;
        char nombre[100];
        string apellido[100];
        int edad=0;
        int idjefe=0;
        bool muelto=0;
        bool preso=0;
        bool fuejefe=0;
        bool esjefe=0;
        nodo* hijoder=nullptr;
        nodo*  hijoizq=nullptr;
    nodo(int id,string nombre,string apellido,int age,int idboss,bool dead,bool jail,bool wasboos,bool isboss){
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
    void insertar(int id,string nombre,string apellido,int age,int idboss,bool dead,bool jail,bool wasboos,bool isboss){
        if(this->jefe==nullptr&&isboss==1&&dead==0){
            nodo<C>* nodito=new nodo(dato);
            return;
        }
        nodo<C>* padre=nullptr;
        nodo<C>* act=this->root;
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
        nodo<C>* nodito=new nodo(dato);
        if(padre->hijoizq==nullptr){
            padre->hijoizq=nodito;
        }
        else{
            padre->hijoder=nodito;
        }
        
    }
};
int main(){
 Arbol<int> arbolito;
 ifstream archivo("casanostra.csv");
 
 arbolito.insertar();
}
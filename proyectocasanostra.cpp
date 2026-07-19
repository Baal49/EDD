#include <iostream>
#include <fstream>
#include <sstream>
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
        if(padre->hijoizq==nullptr&&nodito->idjefe>0){
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
 string linea;
 std::string encabezado;
 std::getline(archivo, encabezado); 
 while(getline(archivo,linea)){
    string campos[10];
    stringstream ss(linea);
    string celda;
    int indice=0;
    while(getline(ss,celda,',') && indice < 10){
        campos[indice++]=celda;
    }
    for(int i=0; i<indice; i++){
        std::cout << campos[i] << "\t";
    }
    std::cout << std::endl;

    arbolito.insertar(stoi(campos[0]),campos[1],campos[2],campos[3],stoi(campos[4]),(stoi(campos[5])!=0),(stoi(campos[6])!=0),(stoi(campos[7])!=0),(stoi(campos[8])!=0),(stoi(campos[9])!=0));
 }
 
 arbolito.mostrardato();
}
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;


struct Rtitles{
     string id;
     string title;
     string type;
     long realease_year;
     long runtime;
     float imdb_score;
     long imdb_votes;
     

     long next;
     bool isDelete;
     string getKey(){return title;}
};

// out: write
ostream& operator<<(ostream& os, const Rtitles& rtitle){
    //id
    int valueSize = rtitle.id.size();
    os.write((char*)&valueSize, sizeof(int));
    os.write(rtitle.id.c_str(), valueSize);
    
    //tittle
    valueSize = rtitle.title.size();
    os.write((char*)&valueSize, sizeof(int));
    os.write(rtitle.title.c_str(), valueSize);
    
    //type
    valueSize = rtitle.type.size();
    os.write((char*)&valueSize, sizeof(int));
    os.write(rtitle.type.c_str(), valueSize);

    //release year
    os.write((char*)&rtitle.realease_year, sizeof(long));
    
    //runtime
    os.write((char*)&rtitle.runtime, sizeof(long));

    //imdb_score
    os.write((char*)&rtitle.imdb_score, sizeof(float));

    //imdb_votes
    os.write((char*)&rtitle.imdb_score, sizeof(long));

    //next
    os.write((char*)&rtitle.next, sizeof(long));

    //delete
    os.write((char*)&rtitle.isDelete, sizeof(bool));
    
    return os;
}


class SequentialFile{
     private:
     string dataFile;
     string auxFile;
     public:
     SequentialFile(string _dataFile, string _auxFile) : dataFile(_dataFile), auxFile(_auxFile){
          
     }
     virtual ~SequentialFile(){};

     Rtitles search(string key){
          ifstream mainFile(dataFile, ios::binary);
          //Busqueda binaria sobre el archivo principal
          //Algoritmo busqueda binaria


          //Si no se encontro en el archivo principal. Busqueda sobre el archivo auxiliar


     }
     vector<Rtitles> rangeSearch(string beginKey, string endKey){
          //
     }

     bool add(Rtitles registro){
          fstream auxiliarFile(auxFile, ios::binary);
          fstream mainFile(dataFile, ios::binary);
          // Buscar en archivo principal en busca de la posicion del nuevo elemento y si es uno repetido.

          // Si no es repetido y tengo la posicion del siguiente elemento.
          int newRegisterPosition = auxiliarFile.tellg();
          auxiliarFile << registro;

     }

     bool remove(string key){

     }

     void rebuilt(){

     }
};
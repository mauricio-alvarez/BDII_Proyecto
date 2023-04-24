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
     
     long next = 2;
     bool isDelete;
     string getKey(){return id;}
};

struct metaIndex{
     long position;
     bool isDelete;
};
ostream& operator<<(ostream& os, const metaIndex& index){
     //position
     os.write((char*)&index.position, sizeof(long));
     //isDelte
     os.write((char*)&index.isDelete, sizeof(bool));
     return os;
}

// out: write
ostream& operator<<(ostream& os, const Rtitles& rtitle){
    //id
    long valueSize = rtitle.id.size();
    os.write((char*)&valueSize, sizeof(long));
    os.write(rtitle.id.c_str(), valueSize);
    
    //tittle
    valueSize = rtitle.title.size();
    os.write((char*)&valueSize, sizeof(long));
    os.write(rtitle.title.c_str(), valueSize);
    
    //type
    valueSize = rtitle.type.size();
    os.write((char*)&valueSize, sizeof(long));
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

// in: read
fstream& operator>>(fstream& is,Rtitles& rtitle){
     long sizeString = 0;
    //id
    is.read((char*)(&sizeString), sizeof(long)); rtitle.id.resize(sizeString);
    is.read(&rtitle.id[0], sizeString);
    
    //title
    is.read((char*)(&sizeString), sizeof(long)); rtitle.title.resize(sizeString);
    is.read(&rtitle.title[0], sizeString);

    //type
    is.read((char*)(&sizeString), sizeof(long)); rtitle.type.resize(sizeString);
    is.read(&rtitle.type[0], sizeString);

    //relaease_year
    is.read((char*)(&rtitle.realease_year), sizeof(long));
    is.read((char*)(&rtitle.runtime), sizeof(long));
    is.read((char*)(&rtitle.imdb_score), sizeof(float));
    is.read((char*)(&rtitle.imdb_votes), sizeof(long));

    is.read((char*)(&rtitle.next), sizeof(long));
    is.read((char*)(&rtitle.isDelete), sizeof(bool));
    
    return is;
}

class SequentialFile{
     private:
     string dataFileName;
     string auxFileName;
     string metaFileName;
     public:
     SequentialFile(string _dataFile, string _auxFile, string _metaFile) : dataFileName(_dataFile), auxFileName(_auxFile), metaFileName(_metaFile){
          
     }
     virtual ~SequentialFile(){};

     Rtitles search(string key){
          ifstream mainFile(dataFileName, ios::binary);
          //Busqueda binaria sobre el archivo principal
          //Algoritmo busqueda binaria


          //Si no se encontro en el archivo principal. Busqueda sobre el archivo auxiliar


     }
     vector<Rtitles> rangeSearch(string beginKey, string endKey){
          return vector<Rtitles> {};
     }

     long findPreviusRecord(fstream *mainFile,fstream *metaFile,Rtitles *record){
          int left = 0;metaFile->seekg(0,ios::end);
          int right = metaFile->tellg()/5 - 1;
          int result=1;
          long addressRecord=0;string key;long sizeKey=0;

          //Si el elemento esta eliminado omitir
          //La busqueda solo se realiza en el mainFile hay que buscar tambien en el auxiliarFile

          while (left <= right) {
               int mid = left + (right - left) / 2;

               metaFile->seekg(mid*5, ios::beg);
               metaFile->read((char*)(&addressRecord), sizeof(long));
               
               mainFile->seekg(addressRecord,ios::beg);
               mainFile->read((char*)(&sizeKey), sizeof(long));

               char buffer[sizeKey];
               mainFile->read((char*)(&buffer), sizeKey);key.assign(buffer, sizeKey);
               //cout<<"Key: "<<key<<" "<<record->getKey()<<endl;
               //cout<<boolalpha<<(key < record->getKey())<<endl;
               if (key < record->getKey()) {
                    result = addressRecord;
                    left = mid + 1;
               } else {
                    right = mid - 1;
               }
          }

          return result;
     }
     long getPointerRecord(fstream *mainFile,fstream *metaFile,Rtitles *record){
          long addressRecord = findPreviusRecord(mainFile,metaFile, record);
          Rtitles auxRecord;
          if(addressRecord == 1){
               return 0;
          }else{
               mainFile->seekg(addressRecord);
               (*mainFile) >> auxRecord;
               if(auxRecord.next == 2){
                    return 2;
               }
          }
          
          return auxRecord.next;
     }

     bool add(Rtitles record){
          fstream auxiliarFile(auxFileName, ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::binary);
          metaIndex newRecordIndex = {0,record.isDelete};
          /*
          // Buscar en archivo principal en busca de la posicion del nuevo elemento y si es uno repetido.


          //Reconstruimos los regitros en el mainFile solo si es necesario.
          rebuilt();

          //Leer donde apuntaba antes en el mainFile
          int oldRegisterPosition = localizePosition(&mainFile,&metaFile, &record);
          

          // Si no es repetido y tengo la posicion del siguiente elemento.
          auxiliarFile.seekg(0,ios::end);
          int newRegisterPosition = auxiliarFile.tellg();

          //Escribir a la nueva ubicacion del registro
            //mainFile.seekg();
          mainFile.write((char*)(&newRegisterPosition), sizeof(record.next));

          //Nuevo registro apunta hacia el antiguo registro
          record.next = oldRegisterPosition;

          //Escribimos el nuevo registro al final del auxiliarFile
          auxiliarFile << record;
          

            //mainFile.seekg(0,ios::end);newRecordIndex.position = mainFile.tellg();

            //mainFile << record;
            //metaFile << newRecordIndex;
          */
          cout<<getPointerRecord(&mainFile,&metaFile, &record)<<endl;
          metaFile.close();
          auxiliarFile.close();
          mainFile.close();
          return true;
     }

     bool remove(string key){

     }

     void rebuilt(){
          
     }
};
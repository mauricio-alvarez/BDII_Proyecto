#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <limits>

using namespace std;

struct Rtitles{
     string id;
     string title;
     string type;
     
     long realease_year;
     long runtime;
     float imdb_score;
     long imdb_votes;
     
     int next = -1;
     bool nextFile = false;
     bool isDelete = false;

     string getKey(){return id;}
     void show(){cout<<id<<" "<<title<<" "<<type<<" "<<realease_year<<" "<<runtime<<" "<<imdb_score<<" "<<imdb_votes<<" "<<next<<" "<<nextFile<<" "<<isDelete<<endl;}
     int getSize(){return id.length()+title.length()+type.length()+sizeof(long)*3+sizeof(bool)*2+sizeof(int)+sizeof(float);}
     int getSizeinFile(){return id.length()+title.length()+type.length()+sizeof(long)*3+sizeof(bool)*2+sizeof(int)*4+sizeof(float);}
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
    os.write((char*)&rtitle.imdb_votes, sizeof(long));

    //next
    os.write((char*)&rtitle.next, sizeof(int));

    //belong to
    os.write((char*)&rtitle.nextFile, sizeof(bool));

    //delete
    os.write((char*)&rtitle.isDelete, sizeof(bool));
    
    return os;
}

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

     is.read((char*)(&rtitle.next), sizeof(int));
     is.read((char*)(&rtitle.nextFile), sizeof(bool));

     is.read((char*)(&rtitle.isDelete), sizeof(bool));
    
    return is;
}

class SequentialFile{
private:
     string dataFileName;
     string auxFileName;
     string metaFileName;
public:
     SequentialFile(string _dataFile, string _auxFile, string _metaFile) : dataFileName(_dataFile), auxFileName(_auxFile), metaFileName(_metaFile){}
     virtual ~SequentialFile(){};

     tuple<int, Rtitles> searchOnAuxiliar(Rtitles record){
          fstream auxiliarFile(auxFileName,ios::in | ios::app | ios::binary);
          Rtitles aux_1Record;
          Rtitles result;
          int position = -1;

          auxiliarFile.seekg(0, ios::beg);
          while(auxiliarFile >> aux_1Record){
               if (aux_1Record.getKey() < record.getKey() && (result.id.empty() || aux_1Record.getKey() > result.getKey()) && !aux_1Record.isDelete) {
                    result = aux_1Record;
                    position = auxiliarFile.tellg() - result.getSizeinFile();
               }
          }

          return make_tuple(position, result);
     }
     tuple<int, Rtitles> searchOnMain(Rtitles record){
          fstream mainFile(dataFileName, ios::in | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::binary);
          
          long addressRecord=0;
          Rtitles result;
          int position = -1;

          int left = 0;metaFile.seekg(0,ios::end);
          int right = metaFile.tellg()/5 - 1;
          while (left <= right) {
               int mid = left + (right - left) / 2;
               metaFile.seekg(mid*5, ios::beg);
               metaFile.read((char*)(&addressRecord), sizeof(long));
               mainFile.seekg(addressRecord,ios::beg);
               mainFile >> result;
               
               if (result.getKey() < record.getKey()) {
                    left = mid + 1;
               } else {
                    right = mid - 1;
               }
          }
          if(right >= 0){
               metaFile.seekg(right*5, ios::beg);
               metaFile.read((char*)(&addressRecord), sizeof(long));
               mainFile.seekg(addressRecord,ios::beg);
               mainFile >> result;
               position = addressRecord;
          }else{
               position = -1;
          }

          mainFile.close();
          metaFile.close();
          
          return make_tuple(position, result);
     }

     
     bool add(Rtitles record){
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::app | ios::binary);
          metaIndex newRecordIndex = {0,record.isDelete};

          rebuilt();

          //VEMOS EN QUE POSICION DEL AUXILIAR ENTRARA EL NUEVO REGISTRO
          auxiliarFile.seekg(0,ios::end);          
          int newRegisterPosition = auxiliarFile.tellg();
          
          
          //CASOS//
          auxiliarFile.seekg(0,ios::end);
          mainFile.seekg(0,ios::end);

          cout<<"MainFile: "<<mainFile.tellg()<<"   "<<"AuxiliarFile: "<<auxiliarFile.tellg()<<endl;
          
          if(auxiliarFile.tellg() == 0 && mainFile.tellg() == 0){
               //Ambos vacios
               
               auxiliarFile << record;
          }else if(auxiliarFile.tellg() != 0 && mainFile.tellg() == 0){
               //Main vacio
               
               int address;
               Rtitles prevRecord;
               tie(address, prevRecord) = searchOnAuxiliar(record);

               cout<<address<<endl;
               cout<<prevRecord.id<<endl;
               
               if(address == -1){
                    record.next = 0;
                    record.nextFile = false;

                    auxiliarFile << record;
               }else{
                    record.next = prevRecord.next;
                    record.nextFile = prevRecord.nextFile;
                    auxiliarFile << record;
                    //cout<<"Escribo :"<<record.id<<endl;

                    auxiliarFile.seekp(address);
                    prevRecord.next = newRegisterPosition;
                    prevRecord.nextFile = false;
                    auxiliarFile << prevRecord;
                    //cout<<"Escribo :"<<prevRecord.id<<endl;
               }

          }else if(auxiliarFile.tellg() == 0 && mainFile.tellg() != 0){
               //Auxiliar vacio
               int address;
               Rtitles prevRecord;
               tie(address, prevRecord) = searchOnMain(record);

               cout<<address<<endl;
               cout<<prevRecord.id<<endl;

               if(address == -1){
                    record.next = 0;
                    record.nextFile = false;
                    auxiliarFile << record;
               }else{
                    record.next = prevRecord.next;
                    record.nextFile = prevRecord.nextFile;
                    auxiliarFile << record;

                    mainFile.seekp(address);
                    prevRecord.next = newRegisterPosition;
                    prevRecord.nextFile = false;
                    mainFile << prevRecord;
               }
          }else{
               //Ambos tienen
               int addressLineal;
               Rtitles prevRecordLineal;

               int addressBinary;
               Rtitles prevRecordBinary;

               tie(addressBinary, prevRecordBinary) = searchOnMain(record);
               tie(addressLineal, prevRecordLineal) = searchOnAuxiliar(record);
               
               cout<<prevRecordBinary.id<<endl;
               cout<<prevRecordLineal.id<<endl;

               
               if(prevRecordBinary.id < prevRecordLineal.id){
                    record.next = prevRecordLineal.next;
                    record.nextFile = prevRecordLineal.nextFile;
                    auxiliarFile << record;

                    auxiliarFile.seekp(addressLineal,ios::beg);
                    prevRecordLineal.next = newRegisterPosition;
                    prevRecordLineal.nextFile = false;
                    auxiliarFile << prevRecordLineal;
               }else{
                    
                    cout<<prevRecordBinary.id<<endl;
                    cout<<addressBinary<<endl;
                    
                    record.next = prevRecordBinary.next;
                    record.nextFile = prevRecordBinary.nextFile;
                    if(addressBinary==-1){
                         record.next = 0;record.nextFile = true;
                    }else{
                         record.next = prevRecordBinary.next;record.nextFile = prevRecordBinary.nextFile;
                    }
                    auxiliarFile << record;

                    mainFile.seekp(addressBinary,ios::beg);
                    prevRecordBinary.next = newRegisterPosition;
                    prevRecordBinary.nextFile = false;
                    mainFile << prevRecordBinary;
                    
               }
               
               
          }
          
          metaFile.close();
          auxiliarFile.close();
          mainFile.close();
          return true;
     }
     Rtitles search(string key){
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::app | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::app | ios::binary);

          Rtitles auxRecord, result;
          long addressRecord;
          int left = 0;metaFile.seekg(0,ios::end);
          int right = metaFile.tellg()/5 - 1;

          while (left <= right) {
               int mid = left + (right - left) / 2;

               metaFile.seekg(mid*5, ios::beg);
               metaFile.read((char*)(&addressRecord), sizeof(long));
               mainFile.seekg(addressRecord,ios::beg);
               mainFile >> auxRecord;

               if (auxRecord.getKey() == key && !auxRecord.isDelete) {
                    result = auxRecord;break;
               } else if (auxRecord.getKey() < key) {
                    left = mid + 1;
               } else {
                    right = mid - 1;
               }
          }
          //SI NO ENCONTRO EN EL MAIN BUSQUEDA EN EL AUXILIAR
          if(result.id.empty()){
               auxiliarFile.seekg(0, ios::beg);
               while(auxiliarFile >> auxRecord){
                    if(auxRecord.getKey() == key && !auxRecord.isDelete){
                         result = auxRecord;
                         break;
                    }         
               }
          
          }


          auxiliarFile.close();
          mainFile.close();
          metaFile.close();
          return result;
     }
     
     tuple<int, bool, Rtitles> positionSearch(string key){
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::app | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::app | ios::binary);
          bool filePath = true;
          Rtitles auxRecord, result;
          long addressRecord;
          int left = 0;metaFile.seekg(0,ios::end);
          int right = metaFile.tellg()/5 - 1;

          while (left <= right) {
               int mid = left + (right - left) / 2;
               metaFile.seekg(mid*5, ios::beg);
               metaFile.read((char*)(&addressRecord), sizeof(long));
               mainFile.seekg(addressRecord,ios::beg);
               mainFile >> auxRecord;
               if (auxRecord.getKey() == key && !auxRecord.isDelete) {
                    result = auxRecord;break;
               } else if (auxRecord.getKey() < key) {
                    left = mid + 1;
               } else {
                    right = mid - 1;
               }
          }
          //SI NO ENCONTRO EN EL MAIN BUSQUEDA EN EL AUXILIAR
          if(result.id.empty()){
               auxiliarFile.seekg(0, ios::beg);
               while(auxiliarFile >> auxRecord){
                    if(auxRecord.getKey() == key && !auxRecord.isDelete){
                         result = auxRecord;
                         filePath = false;
                         addressRecord = auxiliarFile.tellg() - result.getSizeinFile();
                         break;
                    }         
               }
          
          }


          auxiliarFile.close();
          mainFile.close();
          metaFile.close();
          return make_tuple(addressRecord,filePath,result);
     }
     
     vector<Rtitles> rangeSearch(string begin_key, string end_key){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          //VERIFICAR QUE AMBAS KEYS EXISTAN
          Rtitles first = search(begin_key);
          Rtitles last = search(end_key);
          vector<Rtitles> range;

          if(!first.id.empty() && !last.id.empty()){
               while (true){
                    if(!first.isDelete){
                         range.push_back(first);
                    }
                    if(first.getKey() == last.getKey()){break;};
                    if(first.nextFile){
                         mainFile.seekg(first.next);
                         mainFile >> first;
                    }else{
                         auxiliarFile.seekg(first.next);
                         auxiliarFile >> first;
                    }    
               }
          }
          mainFile.close();
          auxiliarFile.close();
          return range;
     }
     bool remove(string key){
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          bool state;
          int addressRecord;
          Rtitles auxRecord;

          tie(addressRecord,state,auxRecord) = positionSearch(key);
          //Aseguromos que haya un registro con esa llave.

          if(!auxRecord.id.empty()){
               auxRecord.isDelete = true;
               auxRecord.show();

               //ACTUALIZAR PUNTEROS
               if(state){
                    mainFile.seekg(addressRecord);
                    mainFile << auxRecord;
               }else{
                    auxiliarFile.seekg(addressRecord);
                    auxiliarFile << auxRecord;
               }
          }else{throw("No se encontro un registro con esa llave.");state=false;}
          auxiliarFile.close();
          mainFile.close();
          return state;
     }
     void rebuilt(){
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::out | ios::binary);
          metaIndex newRecordIndex;

          auxiliarFile.seekg(0,ios::end);
          mainFile.seekg(0,ios::end);
          Rtitles auxRecord;
          Rtitles aux_1Record;
          aux_1Record.next = numeric_limits<int>::max();
          int counter = 0;

          //CASOS
          // - CASO 1 (MAIN VACIO)
          if(auxiliarFile.tellg() > 300 && mainFile.tellg() == 0){
               cout<<"RECONSTRUCCION... "<<endl;
               auxiliarFile.seekg(0);
               while(auxiliarFile >> auxRecord){
                    counter+=1;
                    if(auxRecord.next < aux_1Record.next && auxRecord.next != -1){
                         aux_1Record = auxRecord;
                    }
               }

               auxiliarFile.close();
               fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
               cout<<aux_1Record.id<<endl;

               for (int i = 0; i < counter; i++){
                    newRecordIndex.isDelete = aux_1Record.isDelete;
                    newRecordIndex.position = mainFile.tellg();

                    auxiliarFile.seekg(aux_1Record.next);
                    
                    if(aux_1Record.next != -1){
                         aux_1Record.next = mainFile.tellg() + aux_1Record.getSizeinFile();
                    }else{
                         aux_1Record.next = -1;
                    }
                    
                    aux_1Record.nextFile = true;

                    mainFile << aux_1Record;
                    
                    metaFile << newRecordIndex;

                    if(aux_1Record.next != -1){
                         auxiliarFile >> aux_1Record;
                    }else{break;}
                              
               }
               clearAuxiliar();
               
          }
          // - CASO 2 (MAIN CONTIENE)
          else{

          }
          
          //Borramos los datos que haya en el auxiliar
          
          

          metaFile.close();
          auxiliarFile.close();
          mainFile.close();

     }
     void showMain(){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          mainFile.seekg(0, ios::beg);
          Rtitles aux_1Record;
          while(mainFile >> aux_1Record){
                aux_1Record.show();         
          }
          mainFile.close();
     }
     void showAuxiliar(){
          fstream auxiliarFile(auxFileName,ios::in | ios::app | ios::binary);
          auxiliarFile.seekg(0, ios::beg);
          Rtitles aux_1Record;
          while(auxiliarFile >> aux_1Record){
                aux_1Record.show();         
          }
          auxiliarFile.close();
     }
     void showSequential(){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          Rtitles auxRecord;

          mainFile.seekg(0);
          mainFile >> auxRecord;
          while (true){
               if(!auxRecord.isDelete){
                    auxRecord.show();
               }
               if(auxRecord.next == -1){break;};
               if(auxRecord.nextFile){
                    mainFile.seekg(auxRecord.next);
                    mainFile >> auxRecord;
               }else{
                    auxiliarFile.seekg(auxRecord.next);
                    auxiliarFile >> auxRecord;
               }    
          }
          mainFile.close();
          auxiliarFile.close();
     }
     void clearAuxiliar(){
          ofstream auxiliarFile(auxFileName, ios::trunc);
          auxiliarFile.close();
     }
};
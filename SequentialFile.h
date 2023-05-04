#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <tuple>
#include <limits>
#include <cstdio>
#include <iomanip>
#include <cstdlib>

using namespace std;

void deleteFiles(){
     if(remove("tempMain.dat") != 0) {perror("Error deleting file");}else {puts("File successfully deleted");}   
     if(remove("tempMeta.dat") != 0) {perror("Error deleting file");}else {puts("File successfully deleted");}  
}

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
     string get_casting(){
          return id + " " + title + " " + type + " " + to_string(realease_year)  + " " + to_string(runtime)  + " " + to_string(imdb_score) + " " + to_string(imdb_votes);
     }




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
     string auxFileName = "auxiliar.dat";
     string metaFileName = "metadata.dat";
     
public:
     SequentialFile(string _dataFile) : dataFileName(_dataFile){
          ofstream mainFile(dataFileName, ios::trunc);
          ofstream auxiliarFile(auxFileName, ios::trunc);
          ofstream metaFile(metaFileName, ios::trunc);

          mainFile.close();
          auxiliarFile.close();
          metaFile.close();
     }

     SequentialFile(){};

     void openFile(string _dataFile){
          dataFileName = _dataFile;
          ofstream mainFile(dataFileName, ios::trunc);
          ofstream auxiliarFile(auxFileName, ios::trunc);
          ofstream metaFile(metaFileName, ios::trunc);

          mainFile.close();
          auxiliarFile.close();
          metaFile.close();
     }

     void load(string data){
          std::string line;
          ifstream file(data);

          while (std::getline(file, line)) {
               std::stringstream ss(line);
               Rtitles record;
               std::getline(ss, record.id, ',');
               std::getline(ss, record.title, ',');
               std::getline(ss, record.type, ',');
               ss >> record.realease_year;
               ss.ignore();
               ss >> record.runtime;
               ss.ignore();
               ss >> record.imdb_score;
               ss.ignore();
               ss >> record.imdb_votes;

               add(record);
               
                    
          }
     }
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
                    position = int(auxiliarFile.tellg()) - result.getSizeinFile();
               }
          }

          return make_tuple(position, result);
     }
     tuple<int, Rtitles> searchOnMain(Rtitles record){
          fstream mainFile(dataFileName, ios::in | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::binary);
          
          long addressRecord=0;
          Rtitles auxRecord, result;
          int position = -1;

          int left = 0;metaFile.seekg(0,ios::end);
          int right = metaFile.tellg()/5 - 1;
          while (left <= right) {
               int mid = left + (right - left) / 2;
               metaFile.seekg(mid*5, ios::beg);
               metaFile.read((char*)(&addressRecord), sizeof(long));
               mainFile.seekg(addressRecord,ios::beg);
               mainFile >> auxRecord;
               if(auxRecord.isDelete){mainFile >> auxRecord;}
               if (auxRecord.getKey() < record.getKey()) {
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
          if(!search(record.id).id.empty()){perror("Its not possible to add this element,it is repeat.");
          return false;}
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::app | ios::binary);
          metaIndex newRecordIndex = {0,record.isDelete};

          auxiliarFile.seekg(0,ios::end); 
          if(auxiliarFile.tellg() > 300){
               rebuilt();
          }

          //VEMOS EN QUE POSICION DEL AUXILIAR ENTRARA EL NUEVO REGISTRO
          auxiliarFile.seekg(0,ios::end);          
          int newRegisterPosition = auxiliarFile.tellg();
          
          //CASOS//
          auxiliarFile.seekg(0,ios::end);
          mainFile.seekg(0,ios::end);

          cout<<"[→] AGREGAR... "<<" Insert "<<record.getKey()<<", ";
          if(auxiliarFile.tellg() == 0 && mainFile.tellg() == 0){            
               auxiliarFile << record;
          }else if(auxiliarFile.tellg() != 0 && mainFile.tellg() == 0){
               //Main vacio
               cout<<"[Main is empty]"<<endl;
               int address;
               Rtitles prevRecord;
               tie(address, prevRecord) = searchOnAuxiliar(record);
               cout<<"[EVALUATION] Preview Record:"<<prevRecord.id<<endl;
               if(address == -1){

                    record.next = 0;
                    record.nextFile = false;

                    auxiliarFile << record;
               }else{
                    record.next = prevRecord.next;
                    record.nextFile = prevRecord.nextFile;
                    auxiliarFile << record;


                    auxiliarFile.seekp(address);
                    prevRecord.next = newRegisterPosition;
                    prevRecord.nextFile = false;
                    auxiliarFile << prevRecord;

               }

          }else if(auxiliarFile.tellg() == 0 && mainFile.tellg() != 0){
               //Auxiliar vacio
               cout<<"[Auxiliar is empty]"<<endl;
               int address;
               Rtitles prevRecord;
               tie(address, prevRecord) = searchOnMain(record);

               cout<<"[EVALUATION] Preview Record:"<<prevRecord.id<<endl;
               cout<<"[EVALUATION] Preview Address:"<<address<<endl;

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
               cout<<"[Any is empty]"<<endl;
               int addressLineal;
               Rtitles prevRecordLineal;

               int addressBinary;
               Rtitles prevRecordBinary;

               tie(addressBinary, prevRecordBinary) = searchOnMain(record);
               tie(addressLineal, prevRecordLineal) = searchOnAuxiliar(record);
               
               cout<<"[EVALUATION]"<<endl;
               cout<<"Address Lineal: "<<addressLineal<<", Preview Record: "<<prevRecordLineal.id<<endl;
               cout<<"Address Binary: "<<addressBinary<<", Preview Record Binary: "<<prevRecordBinary.id<<endl;
               
               if(prevRecordLineal.id > prevRecordBinary.id && (addressBinary != -1 || addressLineal != -1)){
                    cout<<"LIneal  mayor que binary"<<endl;
                    
                    if(addressLineal==-1){
                         record.next = 0;record.nextFile = true;
                    }else{
                         record.next = prevRecordLineal.next;
                         record.nextFile = prevRecordLineal.nextFile;
                    }
                    auxiliarFile << record;

                    auxiliarFile.seekp(addressLineal,ios::beg);
                    
                    prevRecordLineal.next = newRegisterPosition;
                    prevRecordLineal.nextFile = false;
                    auxiliarFile << prevRecordLineal;
                    
               }else if(prevRecordBinary.id > prevRecordLineal.id && (addressBinary != -1 || addressLineal != -1)){
                    cout<<"binary mayor que Lineal"<<endl;
                    record.next = prevRecordBinary.next;
                    record.nextFile = prevRecordBinary.nextFile;
                    auxiliarFile << record;

                    if(addressBinary == -1){
                         record.next = 0;record.nextFile = true;
                    }else{
                         record.next = prevRecordBinary.next;
                         record.nextFile = prevRecordBinary.nextFile;
                    }

                    mainFile.seekp(addressBinary,ios::beg);
                    prevRecordBinary.next = newRegisterPosition;
                    prevRecordBinary.nextFile = false;
                    mainFile << prevRecordBinary;

               }else{
                    if(getFirstRecordMain().getKey() < getFirstRecordAuxiliar().getKey()){
                         record.next = 0;
                         record.nextFile = true;
                         auxiliarFile << record;
                    }else{
                         record.next = 0;
                         record.nextFile = false;
                         auxiliarFile << record;
                    }
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
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::out | ios::binary);
          bool filePath = true;
          Rtitles auxRecord, result;
          long addressRecord;
          int left = 0;
          metaFile.seekg(0,ios::end);
          int right = metaFile.tellg()/5 - 1;


          while (left <= right) {
               int mid = left + (right - left) / 2;
               metaFile.seekg(mid*5, ios::beg);
               metaFile.read((char*)(&addressRecord), sizeof(long));
               mainFile.seekg(addressRecord,ios::beg);
               mainFile >> auxRecord;

               

               if (auxRecord.getKey() == key && !auxRecord.isDelete) {
                    result = auxRecord; break;
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
                         addressRecord = int(auxiliarFile.tellg()) - result.getSizeinFile();
                         break;
                    }         
               }
          
          }


          auxiliarFile.close();
          mainFile.close();
          metaFile.close();
          return make_tuple(addressRecord,filePath,result);
     }
     
     tuple<int, bool, Rtitles> searchPreviusRecord(int address, bool filePath, Rtitles record){
          bool state;
          int ansMainAddress,ansAuxiAddress;
          Rtitles ansMainRecord, ansAuxiRecord;

          tie(ansMainAddress, ansMainRecord) = searchOnMain(record);
          tie(ansAuxiAddress, ansAuxiRecord) = searchOnAuxiliar(record);

          if(ansMainRecord.next == address && ansMainAddress != -1 && ansMainRecord.nextFile == filePath){
               return make_tuple(ansMainAddress,true, ansMainRecord); 
          }else if(ansAuxiRecord.next == address && ansAuxiAddress != -1 && ansAuxiRecord.nextFile == filePath){
               return make_tuple(ansAuxiAddress,false, ansAuxiRecord); 
          }else{
               return make_tuple(-1,true, ansMainRecord); 
          }

     }
     
     vector<string> rangeSearch(string begin_key, string end_key){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          //VERIFICAR QUE AMBAS KEYS EXISTAN
          Rtitles first = search(begin_key);
          Rtitles last = search(end_key);
          vector<string> range;

          if(!first.id.empty() && !last.id.empty()){
               while (true){
                    if(!first.isDelete){
                         range.push_back(first.get_casting());
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
          bool state, reciveState;
          int addressRecord, reciveAddress;
          Rtitles auxRecord, reciveRecord;

          tie(addressRecord,state,auxRecord) = positionSearch(key);
          
          if(!auxRecord.id.empty()){
               cout<<"[→] REMOVE... "<<auxRecord.getKey()<<" at "<<addressRecord<<",  "<<state<<endl;    
               
               tie(reciveAddress,reciveState,reciveRecord)=searchPreviusRecord(addressRecord,state,auxRecord);
               
               if(reciveAddress != -1){
                    if(reciveState){
                         reciveRecord.next = auxRecord.next;
                         reciveRecord.nextFile = auxRecord.nextFile;
                         mainFile.seekg(reciveAddress);
                         mainFile << reciveRecord;
                    }else{
                         reciveRecord.next = auxRecord.next;
                         reciveRecord.nextFile = auxRecord.nextFile;
                         auxiliarFile.seekg(reciveAddress);
                         auxiliarFile << reciveRecord;
                    }
               }
               auxRecord.isDelete = true;
               
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

          
          //CASO 1 (MAIN VACIO)
          if(mainFile.tellg() == 0){
               cout<<"[→] RECONSTRUCCION...  "<<endl<<endl;
               auxiliarFile.seekg(0);
               while(auxiliarFile >> auxRecord){
                    counter+=1;
                    if(aux_1Record.getKey().empty() || auxRecord.getKey() < aux_1Record.getKey() && auxRecord.next != -1){
                         aux_1Record = auxRecord;
                    }
               }

               auxiliarFile.close();
               fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
               cout<<"[MAIN HASNT] BEGIN POINT: "<<setw(5)<<aux_1Record.getKey()<<endl;

               for (int i = 0; i < counter; i++){
                    newRecordIndex.isDelete = aux_1Record.isDelete;
                    newRecordIndex.position = mainFile.tellg();

                    auxiliarFile.seekg(aux_1Record.next);
                    
                    if(aux_1Record.next != -1){
                         aux_1Record.next = int(mainFile.tellg()) + aux_1Record.getSizeinFile();
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
          else if(mainFile.tellg() != 0){
               cout<<"[→] RECONSTRUCCION...  "<<endl<<endl;
               auxiliarFile.seekg(0);
               while(auxiliarFile >> auxRecord){
                    if((aux_1Record.getKey().empty() || auxRecord.getKey() < aux_1Record.getKey()) && !auxRecord.isDelete){
                         aux_1Record = auxRecord;
                    }
               }
               mainFile.seekg(0);
               while(mainFile >> auxRecord){
                    if(!auxRecord.isDelete){break;}
               }


               cout<<aux_1Record.id<<endl;
               cout<<auxRecord.id<<endl;
               (aux_1Record.getKey() < auxRecord.getKey()) ? aux_1Record = aux_1Record: aux_1Record = auxRecord;

               cout<<"[MAIN HAS RECORDS] BEGIN POINT: "<<setw(5)<<aux_1Record.getKey()<<endl;

               moveContent(aux_1Record);       

               clearAuxiliar();
          }
          
          metaFile.close();
          auxiliarFile.close();
          mainFile.close();

     }
     
     void showMain(){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          mainFile.seekg(0, ios::beg);
          Rtitles aux_1Record;int count;
          while(mainFile >> aux_1Record){
               count+=1;
               cout<<"["<<int(mainFile.tellg())-aux_1Record.getSizeinFile()<<"]"<<setw(10); aux_1Record.show();         
          }
          cout<<count<<endl;
          mainFile.close();
     }
     void showAuxiliar(){
          fstream auxiliarFile(auxFileName,ios::in | ios::app | ios::binary);
          auxiliarFile.seekg(0, ios::beg);
          Rtitles aux_1Record;int count;
          while(auxiliarFile >> aux_1Record){
               count+=1;
               cout<<"["<<int(auxiliarFile.tellg()) - aux_1Record.getSizeinFile()<<"]"<<setw(10);aux_1Record.show();         
          }
          cout<<count<<endl;
          auxiliarFile.close();
     }
     void showSequential(){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          Rtitles auxRecord, aux_1Record, minRecord;int counter =0;
          
          //SELECT FIRST RECORD IN MAIN
          while(mainFile >> auxRecord){
               if(!auxRecord.isDelete){break;}
          }
          //SELECT FIRST RECORD IN AUXILIAR
          while (auxiliarFile >> aux_1Record){
               if(aux_1Record.getKey() < minRecord.getKey() || minRecord.getKey().empty() && !aux_1Record.isDelete){minRecord = aux_1Record;}
          }

          if(minRecord.getKey() < auxRecord.getKey() || auxRecord.getKey().empty()){
               auxRecord = minRecord;
          }else if(minRecord.getKey() > auxRecord.getKey() || minRecord.getKey().empty()){
               auxRecord = auxRecord;
          }else{
               return;
          }
          auxiliarFile.close();
          auxiliarFile.open(auxFileName,ios::in | ios::out | ios::binary);

          while (true){
               try{
                    auxRecord.show();
                    counter+=1;
                    if(auxRecord.next == -1){break;};
                    if(auxRecord.nextFile){
                         mainFile.seekg(auxRecord.next);
                         mainFile >> auxRecord;
                    }else{
                         auxiliarFile.seekg(auxRecord.next);
                         auxiliarFile >> auxRecord;
                    }
               }catch(...){
                    throw("Ha ocurrido un error en la lectura de datos.");
                    break;
               }
                 
          }
          

          
          
          //FOLLOW POINTERS

          cout<<counter<<endl;
          mainFile.close();
          auxiliarFile.close();
     }
     
     vector<string> showSome(int n){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          Rtitles auxRecord, aux_1Record, minRecord;int counter =0;
          
          vector<string> nfirstRecords;

          //SELECT FIRST RECORD IN MAIN
          while(mainFile >> auxRecord){
               if(!auxRecord.isDelete){break;}
          }
          //SELECT FIRST RECORD IN AUXILIAR
          while (auxiliarFile >> aux_1Record){
               if(aux_1Record.getKey() < minRecord.getKey() || minRecord.getKey().empty() && !aux_1Record.isDelete){minRecord = aux_1Record;}
          }

          if(minRecord.getKey() < auxRecord.getKey() || auxRecord.getKey().empty()){
               auxRecord = minRecord;
          }else if(minRecord.getKey() > auxRecord.getKey() || minRecord.getKey().empty()){
               auxRecord = auxRecord;
          }else{
               return nfirstRecords;
          }
          auxiliarFile.close();
          auxiliarFile.open(auxFileName,ios::in | ios::out | ios::binary);

          while (true){
               try{
                    nfirstRecords.push_back(auxRecord.get_casting());
                    if(counter==n){
                         break;
                    }
                    counter+=1;
                    if(auxRecord.next == -1){break;};
                    if(auxRecord.nextFile){
                         mainFile.seekg(auxRecord.next);
                         mainFile >> auxRecord;
                    }else{
                         auxiliarFile.seekg(auxRecord.next);
                         auxiliarFile >> auxRecord;
                    }
               }catch(...){
                    throw("Ha ocurrido un error en la lectura de datos.");
                    break;
               }
                 
          }
          
          //FOLLOW POINTERS


          mainFile.close();
          auxiliarFile.close();
          return nfirstRecords;
     }

     void moveContent(Rtitles auxRecord){
          fstream tempMain("tempMain.dat", ios::app | ios::binary);
          fstream tempMeta("tempMeta.dat", ios::app | ios::binary);
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          fstream metaFile(metaFileName, ios::in | ios::out | ios::binary);
          metaIndex newRecordIndex;
          
          int next;

          while(auxRecord.next != -1){
               if(auxRecord.nextFile){
                    next = auxRecord.next;
                    newRecordIndex.position = tempMain.tellg();
                    newRecordIndex.isDelete = false;

                    auxRecord.next = int(tempMain.tellg()) + auxRecord.getSizeinFile();
                    auxRecord.nextFile = true;

                    mainFile.seekg(next);
                    
                    tempMain << auxRecord;
                    tempMeta << newRecordIndex;
                    mainFile >> auxRecord; 
               }else{
                    next = auxRecord.next;

                    auxRecord.next = int(tempMain.tellg()) + auxRecord.getSizeinFile();
                    auxRecord.nextFile = true;

                    newRecordIndex.position = tempMain.tellg();
                    newRecordIndex.isDelete = false;

                    auxiliarFile.seekg(next);

                    tempMain << auxRecord;
                    tempMeta << newRecordIndex;
                    auxiliarFile >> auxRecord;
               }
          }
          auxRecord.next = -1;
          auxRecord.nextFile = true;
          newRecordIndex.position = tempMain.tellg();
          newRecordIndex.isDelete = false;
          tempMain << auxRecord;
          tempMeta << newRecordIndex;
          
          tempMain.seekg(0,ios::end);

          tempMain.close();
          tempMeta.close();
          auxiliarFile.close();
          mainFile.close();
          metaFile.close();
          
          tempMain.open("tempMain.dat", ios::in | ios::out | ios::binary);
          tempMeta.open("tempMeta.dat", ios::in | ios::out | ios::out | ios::binary);
          auxiliarFile.open(auxFileName, ios::trunc | ios::in | ios::out | ios::binary);
          mainFile.open(dataFileName, ios::trunc | ios::in | ios::out | ios::binary);
          metaFile.open(metaFileName, ios::trunc | ios::in | ios::out | ios::binary);


          char buffer[512];          
          while(tempMain){
               tempMain.read((char*)(&buffer), 512);
               mainFile.write((char*)(&buffer), tempMain.gcount());
          }
          
          while(tempMeta){
               tempMeta.read((char*)(&buffer), 512);
               metaFile.write((char*)(&buffer), tempMeta.gcount());
          }


          tempMain.close();
          tempMeta.close();
          auxiliarFile.close();
          mainFile.close();
          metaFile.close();
          deleteFiles();
     }

     Rtitles getFirstRecordAuxiliar(){
          fstream auxiliarFile(auxFileName,ios::in | ios::out | ios::binary);
          Rtitles auxRecord;
          while(auxiliarFile >> auxRecord){if(!auxRecord.isDelete){break;}}

          auxiliarFile.close();
          return auxRecord;
     }
     Rtitles getFirstRecordMain(){
          fstream mainFile(dataFileName, ios::in | ios::out | ios::binary);
          Rtitles auxRecord;
          while(mainFile >> auxRecord){if(!auxRecord.isDelete){break;}}
          mainFile.close();
          return auxRecord;
     }

     void clearAuxiliar(){
          ofstream auxiliarFile(auxFileName, ios::trunc);
          auxiliarFile.close();
     }
};
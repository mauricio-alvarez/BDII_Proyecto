#ifndef PARSER_H
#define PARSER_H
#include <sstream>
#include <string>
#include <regex>
#include "ExtendibleHashFile.h"
#include "SequentialFile.h"
using namespace std;

template<typename T>
void show_hash(vector<T> records){

}

template <typename T>
T str_toupper(T s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return s;
}

template <typename T>
T return_key(T k){
    T res="";
    bool id=false;
    for (auto i=0; i<k.size();i++){
        if (k[i]==';')
            id=false;
        if(id)
            res+=k[i];
        if (k[i] == '=')
            id=true;
    }
    return res;
}

template <typename T>
void return_range(T condition, T& begin, T &end, size_t found, size_t sz){
    T temp=condition;
    T str="";
    condition.resize(found);
    // caso 1: id <= X AND id >= I
    begin = return_key(condition);
    for(auto i=found;i<sz;i++)
        str+=temp[i];
    end = return_key(str);
}



template <typename T>
class SQLParser {
private:
    ExtendibleHashFile hash;
    SequentialFile seq;
    bool index;
public:
    SQLParser() = default;
    vector<string> parse(const T& query) {
        vector<string> res;
        stringstream ss(query);
        T token;
        ss >> token;

        if (str_toupper(token) == "SELECT") {
            // Parse SELECT statement
            T table;
            ss >> token; // Read next token after "SELECT"
            if (token == "*") {
                // Select all columns
                this->seq.showSequential();
                cout<<"all columns"<<endl;
            }
            ss >> token; // Read next token after column names
            if (str_toupper(token) == "FROM") {
                // Parse table or file name
                ss >> table;
                cout<< table<<endl;
                // Check for additional conditions
                if (ss >> token && str_toupper(token) == "WHERE") {
                    // Parse condition
                    T condition;
                    getline(ss, condition);
                    size_t found = condition.find("AND");
                    if (found != string::npos){
                        // RANGE SEARCH
                        T begin, end;
                        return_range(condition, begin, end, found, condition.size());

                        cout<<"BEGIN: ";cout<<begin<<endl;
                        cout<<"END: ";cout<<end<<endl;
                    }else{
                        // SEARCH
                        cout<<return_key(condition)<<endl;
                    }
                    //Record record = sq.search(condition);
                    //QLabel
                }
            }
        } else if (str_toupper(token) == "DELETE") {
            // Parse DELETE statement
            T table;
            ss >> token; // Read next token after column names
            if (str_toupper(token) == "FROM") {
                // Parse table or file name
                ss >> table; cout<<"DELETE from ";
                cout<< table<<endl;
                // Check for additional conditions
                if (ss >> token && str_toupper(token) == "WHERE") {
                    // Parse condition
                    T condition;
                    getline(ss, condition);
                    // Apply condition to table or file (id = "tm1902")
                    //---- func to read ID ----
                    cout<<return_key(condition)<<endl;
                }
            }

        } else if (str_toupper(token) == "INSERT") {
            // Parse INSERT statement
            T table;
            ss >> token; // Read next token after column names
            if (str_toupper(token) == "INTO"){
                ss>>table;
                if (ss >> token && str_toupper(token) == "FROM"){
                    ss>>token;
                    if (str_toupper(token) == "FILE"){
                        T filename;
                        // Inicializar el indice desde el archivo correspondiente
                        // Guardamos el nombre de la tabla
                        ss>>filename;
                        cout<<filename<<endl;
                        T indx;
                        getline(ss, indx);
                        cout << indx<<endl;
                        size_t found = indx.find("hash");
                        // HASH == TRUE
                        if (found != string::npos){
                            this->index = true;

                        }else{
                            this->index = false;
                            this->seq = new SequentialFile(filename);
                        }
                    }
                }
                if (str_toupper(token) == "VALUES"){
                    T values;
                    getline(ss, values);
                    if (index=='s'){
                        // leer linea de valores y guardar en el registro correspondiente
                        //read_sq(values);
                        cout<<values<<endl;
                        // insertar Record en la tabla
                    }else {
                        //read_hash(values);
                        cout<<values<<endl;
                        // insertar Record en la tabla
                    }
                }

            }
        }
    }
};


#endif // PARSER_H

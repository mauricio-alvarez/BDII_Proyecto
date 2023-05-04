#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Credits.h"

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<Record> parseCSV(const string& filename) {
    vector<Record> records;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: could not open file " << filename << endl;
        return records;
    }

    string line;
    int count = 100;
    getline(file, line);

    while (getline(file, line) and --count) {
        vector<string> columns = split(line, ',');
        
        Record record = {
            stoi(columns[0]),
            columns[1],
            columns[2],
            columns[3],
            columns[4]
        };
        records.push_back(record);
    }
    file.close();
    return records;
}
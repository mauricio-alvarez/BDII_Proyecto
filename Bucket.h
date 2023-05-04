#include <iostream>
#include <vector>
#include "Credits.h"

using namespace std;

class Bucket 
{
    private:
        int depth, size;
        vector<Record> records;
        
    public:
        Bucket(int depth, int size);
        int insert(const Record& new_record);
        int remove(int key);
        int update(const Record& updated_record);
        Record search(int key);

        bool isFull();
        bool isEmpty();
        int getDepth();
        int increaseDepth();
        int decreaseDepth();
        vector<Record> copy();
        void clear();
        void display();
};

Bucket::Bucket(int depth, int size) : depth(depth), size(size) {}

int Bucket::insert(const Record& new_record)
{
    vector<Record>::iterator it = records.begin();
    cout << "new_record.getKey() = " << new_record.getKey() << endl;

    // Find record with the same key
    while (it != records.end()) {
        if (it->getKey() == new_record.getKey()) {
            return -1;
        }
        ++it;
    }

    if (isFull()) {
        return 0;
    }

    records.push_back(new_record);
    return 1;
}

int Bucket::remove(int key)
{
    vector<Record>::iterator it = records.begin();

    while (it != records.end()) {
        if (it->getKey() == key) {
            records.erase(it);
            return 1;
        }
        ++it;
    }
    cout << "Cannot remove : This key does not exists" << endl;
    return 0;
}

int Bucket::update(const Record& updated_record)
{
    vector<Record>::iterator it = records.begin();

    while (it != records.end()) {
        if (it->getKey() == updated_record.getKey()) {
            (*it) = updated_record; // Be careful, operator= is not declared
            return 1;
        }
        ++it;
    }
    cout << "Cannot update : This key does not exists" << endl;
    return 0;
}

Record Bucket::search(int key)
{
    vector<Record>::iterator it = records.begin();

    while (it != records.end()) {
        if (it->getKey() == key) {
            return *it;
        }
        ++it;
    }
    cout << "This key does not exists" << endl;
    return Record();
}

bool Bucket::isFull()
{
    return records.size() == size;
}

bool Bucket::isEmpty()
{
    return records.size() == 0;
}

int Bucket::getDepth()
{
    return depth;
}

int Bucket::increaseDepth()
{
    depth++;
    return depth;
}

int Bucket::decreaseDepth()
{
    depth--;
    return depth;
}

vector<Record> Bucket::copy()
{
    vector<Record> temp(records.begin(), records.end());
    return temp;
}

void Bucket::clear()
{
    records.clear();
}

void Bucket::display()
{
    vector<Record>::iterator it;
    for(it = records.begin(); it != records.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}
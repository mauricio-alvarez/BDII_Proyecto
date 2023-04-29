#include <iostream>
#include <map>

using namespace std;

class Bucket 
{
    private:
        int depth, size;
        map<int, string> values;
        
    public:
        Bucket(int depth, int size);
        int insert(int key, string value);
        int remove(int key);
        int update(int key, string value);
        void search(int key);
        bool isFull();
        bool isEmpty();
        int getDepth();
        int increaseDepth();
        int decreaseDepth();
        map<int, string> copy();
        void clear();
        void display();
};

Bucket::Bucket(int depth, int size) : depth(depth), size(size) {}

int Bucket::insert(int key, string value)
{
    map<int,string>::iterator it;
    it = values.find(key);
    if(it!=values.end())
        return -1;
    if(isFull())
        return 0;
    values[key] = value;
    return 1;
}

int Bucket::remove(int key)
{
    map<int,string>::iterator it;
    it = values.find(key);
    if(it!=values.end())
    {
        values.erase(it);
        return 1;
    }
    else
    {
        cout<<"Cannot remove : This key does not exists"<<endl;
        return 0;
    }
}

int Bucket::update(int key, string value)
{
    map<int,string>::iterator it;
    it = values.find(key);
    if(it!=values.end())
    {
        values[key] = value;
        cout<<"Value updated"<<endl;
        return 1;
    }
    else
    {
        cout<<"Cannot update : This key does not exists"<<endl;
        return 0;
    }
}

void Bucket::search(int key)
{
    map<int,string>::iterator it;
    it = values.find(key);
    if(it!=values.end())
    {
        cout<<"Value = "<<it->second<<endl;
    }
    else
    {
        cout<<"This key does not exists"<<endl;
    }
}

bool Bucket::isFull()
{
    return values.size() == size;
}

bool Bucket::isEmpty()
{
    return values.size() == 0;
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

map<int, string> Bucket::copy()
{
    map<int, string> temp(values.begin(),values.end());
    return temp;
}

void Bucket::clear()
{
    values.clear();
}

void Bucket::display()
{
    map<int,string>::iterator it;
    for(it=values.begin();it!=values.end();it++)
        cout<<it->first<<" ";
    cout<<endl;
}
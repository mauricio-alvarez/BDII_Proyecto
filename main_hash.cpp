#include "ExtendibleHashFile.h"

// struct Record
// {
//     int id;
//     // ...
//     Record() {}
//     Record(int id) : id(id) {}

//     int getKey() const {
//         return id;
//     }
// };

void menu()
{
    cout<<"--------------------"<<endl;
    cout<<"Enter queries in the following format :"<<endl;
    cout<<"insert <key> <value>     (key: integer, value: string)"<<endl;
    cout<<"delete <key> <mode>      (mode: 0-Lazy / 1-Merge empty buckets / 2-Merge buckets and shrink )"<<endl;
    cout<<"update <key> <new value>"<<endl;
    cout<<"search <key>"<<endl;
    cout<<"display"<<endl;
    cout<<"exit"<<endl;
    cout<<"--------------------"<<endl;
}

int main()
{
    bool show_duplicate_buckets;
    int bucket_size, initial_global_depth;
    int key, mode;
    string choice, value;

    // Set show_duplicate_buckets to 1 to see all pointers instead of unique ones
    show_duplicate_buckets = 0;

    cout << "Bucket size : ";
    cin >> bucket_size;
    cout << "Initial global depth : ";
    cin >> initial_global_depth;

    ExtendibleHashFile hash(initial_global_depth, bucket_size);
    cout << endl << "Initialized ExtendibleHashFile structure"<<endl;


    menu();    

    do
    {
        cout << endl;
        cout << ">>> ";
        cin >> choice;
        if(choice == "insert")
        {
            cin>>key>>value;
            cout<<endl; 
            hash.insert(key,value,0);
        }
        else if(choice == "delete")
        {
            cin >> key >> mode;
            cout << endl; 
            hash.remove(key,mode);
        }
        else if(choice == "update")
        {
            cin>>key>>value;
            cout<<endl; 
            hash.update(key,value);
        }
        else if(choice == "search")
        {
            cin>>key;
            cout<<endl; 
            hash.search(key);
        }
        else if(choice == "display")
        {
            cout << endl; 
            hash.display(show_duplicate_buckets);
        }
    } while(choice!="exit");

    return 0;
}




#include "ExtendibleHashFile.h"
#include "ParserCSV.h"

using namespace std;

// void menu(ExtendibleHashFile& hash)
// {
//     cout<<"--------------------"<<endl;
//     cout<<"Enter queries in the following format :"<<endl;
//     cout<<"insert <key> <value>     (key: integer, value: string)"<<endl;
//     cout<<"delete <key> <mode>      (mode: 0-Lazy / 1-Merge empty buckets / 2-Merge buckets and shrink )"<<endl;
//     cout<<"update <key> <new value>"<<endl;
//     cout<<"search <key>"<<endl;
//     cout<<"display"<<endl;
//     cout<<"exit"<<endl;
//     cout<<"--------------------"<<endl;

//     // Set show_duplicate_buckets to 1 to see all pointers instead of unique ones
//     bool show_duplicate_buckets = 0;
//     int mode;
//     string choice;

//     Record record;

//     do
//     {
//         cout << endl;
//         cout << ">>> ";
//         cin >> choice;
//         if(choice == "insert")
//         {
//             cin >> record.id >> record.name;
//             cout << endl; 
//             hash.insert(record, 0);
//         }
//         else if(choice == "delete")
//         {
//             cin >> record.id >> mode;
//             cout << endl; 
//             hash.remove(record.id, mode);
//         }
//         else if(choice == "update")
//         {
//             cin >> record.id >> record.name;
//             cout<<endl; 
//             hash.update(record);
//         }
//         else if(choice == "search")
//         {
//             cin >> record.id;
//             cout << endl; 
//             hash.search(record.id);
//         }
//         else if(choice == "display")
//         {
//             cout << endl; 
//             hash.display(show_duplicate_buckets);
//         }
//     } while(choice != "exit");
// }

int main()
{
    // INSERT INTO TABLE credits from file('credits.csv') using index hash;
    int bucket_size = 5; 
    int initial_global_depth = 1;

    ExtendibleHashFile hash(initial_global_depth, bucket_size);

    vector<Record> records = parseCSV("credits.csv");
    for (const Record& record : records) {
        hash.insert(record, 0);
    }

    // SELECT * FROM credits;
    for (const Record& record : records) {
        cout << record << endl;
    }
    cout << endl;

    // SELECT * FROM credits WHERE person_id = 7302;
    int person_id;
    cout << "Search: "; 
    cin >> person_id;

    hash.search(person_id);
    

    return 0;
}




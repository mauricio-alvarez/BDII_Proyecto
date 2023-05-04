#include "ExtendibleHashFile.h"
#include "ParserCSV.h"

using namespace std;

void menu(ExtendibleHashFile& hash);

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

    // SELECT * FROM credits WHERE person_id = 14740;
    // INSERT INTO TABLE credits (...);
    // DELETE FROM TABLE credits WHERE person_id = 14740;
    menu(hash);

    return 0;
}

void menu(ExtendibleHashFile& hash)
{
    cout << "----------------------------------------------------------" << endl;
    cout << "Enter queries in the following format: " << endl;
    cout << "INSERT <person_id> <id> <name> <character> <role>" << endl;
    cout << "DELETE <person_id>" << endl;
    cout << "SEARCH <person_id>" << endl;
    cout << "DISPLAY" << endl;
    cout << "EXIT" << endl;
    cout << "----------------------------------------------------------"<<endl;

    // Set show_duplicate_buckets to 1 to see all pointers instead of unique ones
    bool show_duplicate_buckets = 0;
    string choice;
    int person_id;

    do
    {
        cout << endl;
        cout << ">>> ";
        cin >> choice;
        if(choice == "INSERT")
        {
            Record record;
            record.setData();
            cout << endl; 
            hash.insert(record, 0);
        }
        else if(choice == "DELETE")
        {
            cin >> person_id;
            cout << endl; 
            hash.remove(person_id, 0); // (mode: 0-Lazy / 1-Merge empty buckets / 2-Merge buckets and shrink)
        }
        else if(choice == "SEARCH")
        {
            cin >> person_id;
            cout << endl; 
            Record record = hash.search(person_id);
            cout << record << endl;
        }
        else if(choice == "DISPLAY")
        {
            cout << endl; 
            hash.display(show_duplicate_buckets);
        }
    } while (choice != "EXIT");
}




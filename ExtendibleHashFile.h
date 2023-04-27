#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>

using namespace std;

const size_t global_depth = 2;
const size_t buckets = pow(2, global_depth);
const size_t block_factor = 4;

struct record
{
    int id;
    // ...
    record() {}
    record(int id) : id(id) {}

    int getKey() {
        return id;
    }

    void display() {
        cout << id << " ";
    }
};

struct Bucket
{
    size_t local_depth;
    size_t count;
    record records[block_factor];
    Bucket* next; // long

    void display() {
        for (size_t i = 0; i < count; i++) {
            records[i].display();
        }
    }
};

class ExtendibleHashFile
{
    private:
    // Array of pointers to buckets
    Bucket** directory; 


    public:

    ExtendibleHashFile() {
        directory = new Bucket*[buckets];

        // 0 -> 00 
        Bucket* bucket_A = new Bucket();
        bucket_A->local_depth = 2;
        bucket_A->records[0] = record(4); // 100
        bucket_A->records[1] = record(12); // 1100
        bucket_A->records[2] = record(32); // 100000
        bucket_A->records[3] = record(16); // 10000
        bucket_A->next = nullptr;

        bucket_A->count = 4;
        
        // 1 -> 01
        Bucket* bucket_B = new Bucket();
        bucket_B->local_depth = 2;
        bucket_B->records[0] = record(1); // 01
        bucket_B->records[1] = record(5); // 101
        bucket_B->records[2] = record(21); // 10101
        bucket_B->next = nullptr;
        bucket_B->count = 3;

        // 2 -> 10
        Bucket* bucket_C = new Bucket();
        bucket_C->local_depth = 2;
        bucket_C->records[0] = record(10); // 1010
        bucket_C->next = nullptr;
        bucket_C->count = 1;

        // 3 -> 11
        Bucket* bucket_D = new Bucket();
        bucket_D->local_depth = 2;
        bucket_D->records[0] = record(15); // 1111
        bucket_D->records[1] = record(7); // 111
        bucket_D->records[2] = record(19); // 10011
        bucket_D->next = nullptr;
        bucket_D->count = 3;

        directory[0] = bucket_A;
        directory[1] = bucket_B;
        directory[2] = bucket_C;
        directory[3] = bucket_D;
    }

    string hash_function(int key) {
        int index_bucket = key % buckets;
        return bitset<global_depth>(index_bucket).to_string();
    }

    record search(int key) {
        // Calculate the hash value of the key
        string hash_value = hash_function(key);

        // Find the bucket in the directory array
        int bucket_index = bitset<global_depth>(hash_value).to_ulong();
        Bucket* current_bucket = directory[bucket_index];

        // Traverse the linked list of the bucket and search for the record with the given key
        while (current_bucket != nullptr) {
            for (int i = 0; i < block_factor; i++) {
                if (current_bucket->records[i].getKey() == key) {
                    // Record found, return it
                    return current_bucket->records[i];
                }
            }
            // Move to the next bucket in the linked list
            current_bucket = current_bucket->next;
        }

        // Record not found, return a default record
        return record();
    }


    void display() {
        for (int i = 0; i < buckets; ++i) {
            cout << bitset<global_depth>(i).to_string() << " -> ";
            directory[i]->display();
            cout << "\n";
        }
    }
};



int main() {
    vector<int> keys = {
        23, 2, 4, 5, 9,
        35, 8, 13, 16, 1, 20,
        65, 40, 73, 74
    };

    ExtendibleHashFile hasher;

    hasher.display();


    return 0;
}
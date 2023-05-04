#include "Bucket.h"
#include <vector>
#include <set>

class ExtendibleHashFile 
{
    private:
        int global_depth, bucket_size;
        vector<Bucket*> buckets; // Directory
        int hash(int key);
        int pairIndex(int bucket_no, int depth);
        void grow();
        void shrink();
        void split(int bucket_no);
        void merge(int bucket_no);
        string bucket_id(int n);
    public:
        ExtendibleHashFile(int depth, int bucket_size);
        void insert(const Record& new_record, bool reinserted);
        void remove(int key, int mode);
        void update(const Record& updated_record);
        void search(int key);
        void display(bool duplicates);
};


ExtendibleHashFile::ExtendibleHashFile(int global_depth, int bucket_size) : global_depth(global_depth), bucket_size(bucket_size)
{
    for (int i = 0 ; i < 1 << global_depth ; i++) {
        buckets.push_back(new Bucket(global_depth, bucket_size));
    }
}

int ExtendibleHashFile::hash(int key)
{
    return key & ((1 << global_depth) - 1); // key % pow(2, global_depth);
}

// For merge and split
int ExtendibleHashFile::pairIndex(int bucket_no, int depth)
{
    return bucket_no ^ (1 << (depth - 1));
}

void ExtendibleHashFile::grow()
{
    for (int i = 0; i < 1 << global_depth; i++) {
        buckets.push_back(buckets[i]);
    }
    global_depth++;
}

void ExtendibleHashFile::shrink()
{
    int i, flag = 1;
    for(i = 0; i < buckets.size(); i++) {
        if(buckets[i]->getDepth() == global_depth){
            flag = 0;
            return;
        }
    }
    global_depth--;
    
    for (i = 0 ; i < 1 << global_depth ; i++ ) {
        buckets.pop_back();
    }
}

void ExtendibleHashFile::split(int bucket_no)
{
    int local_depth,pair_index,index_diff,dir_size,i;
    vector<Record> temp;
    vector<Record>::iterator it;

    local_depth = buckets[bucket_no]->increaseDepth();

    if (local_depth > global_depth) {
        grow();
    }
        
    pair_index = pairIndex(bucket_no,local_depth);
    buckets[pair_index] = new Bucket(local_depth,bucket_size);

    temp = buckets[bucket_no]->copy();

    buckets[bucket_no]->clear();
    index_diff = 1 << local_depth;
    dir_size = 1 << global_depth;

    for (i = pair_index - index_diff; i >= 0; i -= index_diff) {
        buckets[i] = buckets[pair_index];
    }
        
    for(i = pair_index + index_diff; i < dir_size; i += index_diff) {
        buckets[i] = buckets[pair_index];
    }
        
    for(it = temp.begin(); it != temp.end(); it++) {
        insert((*it), 1);
    }  
}

void ExtendibleHashFile::merge(int bucket_no)
{
    int local_depth,pair_index,index_diff,dir_size,i;

    local_depth = buckets[bucket_no]->getDepth();
    pair_index = pairIndex(bucket_no,local_depth);
    index_diff = 1<<local_depth;
    dir_size = 1<<global_depth;

    if( buckets[pair_index]->getDepth() == local_depth )
    {
        buckets[pair_index]->decreaseDepth();
        delete(buckets[bucket_no]);
        buckets[bucket_no] = buckets[pair_index];
        for( i=bucket_no-index_diff ; i>=0 ; i-=index_diff )
            buckets[i] = buckets[pair_index];
        for( i=bucket_no+index_diff ; i<dir_size ; i+=index_diff )
            buckets[i] = buckets[pair_index];
    }
}

string ExtendibleHashFile::bucket_id(int n)
{
    int d = buckets[n]->getDepth();
    string s = "";
    
    while (n > 0 && d > 0) {
        s = (n % 2 == 0 ? "0" : "1") + s;
        n /= 2;
        d--;
    }
    while (d > 0) {
        s = "0" + s;
        d--;
    }
    return s;
}

void ExtendibleHashFile::insert(const Record& new_record, bool reinserted)
{
    int bucket_no = hash(new_record.getKey());
    int status = buckets[bucket_no]->insert(new_record);

    if (status == 1) {
        if (!reinserted) {
            cout << "Inserted key " << new_record.getKey() << " in bucket " << bucket_id(bucket_no) << endl;
        } else {
            cout << "Moved key " << new_record.getKey() << " to bucket " << bucket_id(bucket_no) << endl;
        }
    } else if (status == 0) {
        split(bucket_no);
        insert(new_record, reinserted);
    } else {
        cout << "Key " << new_record.getKey() << " already exists in bucket " << bucket_id(bucket_no) << endl;
    }
}

void ExtendibleHashFile::remove(int key, int mode)
{
    int bucket_no = hash(key);
    if(buckets[bucket_no]->remove(key))
        cout<<"Deleted key "<<key<<" from bucket "<<bucket_id(bucket_no)<<endl;
    if(mode>0)
    {
        if(buckets[bucket_no]->isEmpty() && buckets[bucket_no]->getDepth()>1)
            merge(bucket_no);
    }
    if(mode>1)
    {
        shrink();
    }
}

void ExtendibleHashFile::update(const Record& updated_record)
{
    int bucket_no = hash(updated_record.getKey());
    buckets[bucket_no]->update(updated_record);
}

void ExtendibleHashFile::search(int key)
{
    int bucket_no = hash(key);
    cout << "Searching key " << key << " in bucket " << bucket_id(bucket_no) << endl;
    buckets[bucket_no]->search(key);
}

void ExtendibleHashFile::display(bool duplicates)
{
    int i, j, d;
    string s;
    set<string> shown;
    cout << "Global depth : " << global_depth << endl;
    for(i = 0; i < buckets.size(); i++)
    {
        d = buckets[i]->getDepth();
        s = bucket_id(i);
        if(duplicates || shown.find(s)==shown.end())
        {
            shown.insert(s);
            for(j=d;j<=global_depth;j++)
                cout<<" ";
            cout<<s<<" => ";
            buckets[i]->display();
        }
    }
}

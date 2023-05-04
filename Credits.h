# ifndef CREDITS_H
# define CREDITS_H

#include <iostream>

using namespace std;

// Credits
struct Record
{
    int person_id;
    string id;
    string name;
    string character;
    string role;

    Record() = default;

    void setData() {
        cin >> person_id >> id >> name >> character >> role;
    }

    int getKey() const {
        return person_id;
    }
};

ostream& operator<<(ostream& os, const Record& record) {
    os << "[" << record.getKey() << ", " << record.id << ", " << record.name << ", " << record.character << ", " << record.role << "]";
    return os;
}

# endif // CREDITS_H
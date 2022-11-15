#include <string>
#include <iostream>
#include <algorithm>

#include "Members.h"

using namespace std;

void Members::addMember(const string& id) {
    members.emplace_back(id);
}

void Members::removeMember(const string& id) {
    members.erase(remove(members.begin(), members.end(), id), members.end());
}

void Members::describe() {
    cout << members.size() << " members:" << endl;
    for (auto &id: members) {
        cout << " - " << id << endl;
    }
}

#pragma once
#include <string>
#include "DynamicArray.h"
using namespace std;

struct MutationInfo {
    string gene_name;
    string accession_number;
    string url;
};

struct Entry {
    string key;
    DynamicArray<MutationInfo> values;
    Entry* next;
};

class HashMap {
private:
    static const int TABLE_SIZE = 1009;
    Entry* table[TABLE_SIZE];

    int hash(const string& key) const {
        unsigned long hash = 5381;
        for (char c : key)
            hash = ((hash << 5) + hash) + c;
        return hash % TABLE_SIZE;
    }

public:
    HashMap() {
        for (int i = 0; i < TABLE_SIZE; ++i) table[i] = nullptr;
    }

    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            Entry* current = table[i];
            while (current) {
                Entry* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
    }

    void insert(const string& key, const MutationInfo& value) {
        int index = hash(key);
        Entry* current = table[index];

        while (current) {
            if (current->key == key) {
                current->values.push_back(value);
                return;
            }
            current = current->next;
        }

        Entry* newEntry = new Entry{key};
        newEntry->values.push_back(value);
        newEntry->next = table[index];
        table[index] = newEntry;
    }

    DynamicArray<MutationInfo>* get(const string& key) {
        int index = hash(key);
        Entry* current = table[index];

        while (current) {
            if (current->key == key) {
                return &current->values;
            }
            current = current->next;
        }
        return nullptr;
    }

    int getHash(const string& key) const {
        return hash(key);
    }
};

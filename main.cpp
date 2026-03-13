#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <map>
#include <set>

using namespace std;

const string DATA_FILE = "storage.dat";
const int MAX_INDEX_LEN = 65;

struct Entry {
    char index[MAX_INDEX_LEN];
    int value;

    Entry() : value(0) {
        memset(index, 0, sizeof(index));
    }

    Entry(const string& idx, int val) : value(val) {
        memset(index, 0, sizeof(index));
        strncpy(index, idx.c_str(), MAX_INDEX_LEN - 1);
    }

    bool operator<(const Entry& other) const {
        int cmp = strcmp(index, other.index);
        if (cmp != 0) return cmp < 0;
        return value < other.value;
    }

    bool operator==(const Entry& other) const {
        return strcmp(index, other.index) == 0 && value == other.value;
    }
};

class FileStorage {
private:
    string filename;

    vector<Entry> loadAllEntries() {
        vector<Entry> entries;
        ifstream file(filename, ios::binary);
        if (!file) return entries;

        Entry entry;
        while (file.read(reinterpret_cast<char*>(&entry), sizeof(Entry))) {
            entries.push_back(entry);
        }
        file.close();
        return entries;
    }

    void saveAllEntries(const vector<Entry>& entries) {
        ofstream file(filename, ios::binary | ios::trunc);
        for (const auto& entry : entries) {
            file.write(reinterpret_cast<const char*>(&entry), sizeof(Entry));
        }
        file.close();
    }

public:
    FileStorage(const string& fname) : filename(fname) {}

    void insert(const string& index, int value) {
        vector<Entry> entries = loadAllEntries();
        Entry newEntry(index, value);

        // Check if already exists
        for (const auto& entry : entries) {
            if (entry == newEntry) {
                return; // Already exists, don't insert
            }
        }

        entries.push_back(newEntry);
        sort(entries.begin(), entries.end());
        saveAllEntries(entries);
    }

    void remove(const string& index, int value) {
        vector<Entry> entries = loadAllEntries();
        Entry toRemove(index, value);

        auto it = remove_if(entries.begin(), entries.end(),
            [&toRemove](const Entry& e) { return e == toRemove; });

        if (it != entries.end()) {
            entries.erase(it, entries.end());
            saveAllEntries(entries);
        }
    }

    vector<int> find(const string& index) {
        vector<Entry> entries = loadAllEntries();
        vector<int> result;

        for (const auto& entry : entries) {
            if (strcmp(entry.index, index.c_str()) == 0) {
                result.push_back(entry.value);
            }
        }

        sort(result.begin(), result.end());
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    FileStorage storage(DATA_FILE);

    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;

        if (command == "insert") {
            string index;
            int value;
            cin >> index >> value;
            storage.insert(index, value);
        } else if (command == "delete") {
            string index;
            int value;
            cin >> index >> value;
            storage.remove(index, value);
        } else if (command == "find") {
            string index;
            cin >> index;
            vector<int> result = storage.find(index);

            if (result.empty()) {
                cout << "null\n";
            } else {
                for (size_t j = 0; j < result.size(); j++) {
                    if (j > 0) cout << " ";
                    cout << result[j];
                }
                cout << "\n";
            }
        }
    }

    return 0;
}

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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Load existing data from file
    map<string, set<int>> storage;

    ifstream inFile(DATA_FILE, ios::binary);
    if (inFile) {
        string index;
        int value;
        char indexBuf[65];

        while (inFile.read(indexBuf, 64)) {
            indexBuf[64] = '\0';
            inFile.read(reinterpret_cast<char*>(&value), sizeof(int));

            // Find actual string length
            int len = 0;
            while (len < 64 && indexBuf[len] != '\0') len++;

            if (len > 0) {
                index = string(indexBuf, len);
                storage[index].insert(value);
            }
        }
        inFile.close();
    }

    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        string command;
        cin >> command;

        if (command == "insert") {
            string index;
            int value;
            cin >> index >> value;
            storage[index].insert(value);
        } else if (command == "delete") {
            string index;
            int value;
            cin >> index >> value;

            auto it = storage.find(index);
            if (it != storage.end()) {
                it->second.erase(value);
                if (it->second.empty()) {
                    storage.erase(it);
                }
            }
        } else if (command == "find") {
            string index;
            cin >> index;

            auto it = storage.find(index);
            if (it == storage.end() || it->second.empty()) {
                cout << "null\n";
            } else {
                bool first = true;
                for (int val : it->second) {
                    if (!first) cout << " ";
                    cout << val;
                    first = false;
                }
                cout << "\n";
            }
        }
    }

    // Save data back to file
    ofstream outFile(DATA_FILE, ios::binary | ios::trunc);
    char indexBuf[65];

    for (const auto& [index, values] : storage) {
        for (int value : values) {
            memset(indexBuf, 0, 65);
            strncpy(indexBuf, index.c_str(), 64);
            outFile.write(indexBuf, 64);
            outFile.write(reinterpret_cast<const char*>(&value), sizeof(int));
        }
    }
    outFile.close();

    return 0;
}

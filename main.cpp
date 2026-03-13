#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;

const string DATA_FILE = "storage.dat";

struct Record {
    char index[65];
    int value;

    Record() : value(0) {
        memset(index, 0, 65);
    }

    Record(const string& idx, int val) : value(val) {
        memset(index, 0, 65);
        strncpy(index, idx.c_str(), 64);
    }

    bool operator<(const Record& other) const {
        int cmp = strcmp(index, other.index);
        if (cmp != 0) return cmp < 0;
        return value < other.value;
    }

    bool operator==(const Record& other) const {
        return strcmp(index, other.index) == 0 && value == other.value;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Load existing data
    vector<Record> records;

    ifstream inFile(DATA_FILE, ios::binary);
    if (inFile) {
        Record rec;
        while (inFile.read(reinterpret_cast<char*>(&rec), sizeof(Record))) {
            if (rec.index[0] != '\0') {
                records.push_back(rec);
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

            Record newRec(index, value);

            // Check if already exists
            bool exists = false;
            for (const auto& rec : records) {
                if (rec == newRec) {
                    exists = true;
                    break;
                }
            }

            if (!exists) {
                records.push_back(newRec);
            }

        } else if (command == "delete") {
            string index;
            int value;
            cin >> index >> value;

            Record toRemove(index, value);

            auto it = remove(records.begin(), records.end(), toRemove);
            records.erase(it, records.end());

        } else if (command == "find") {
            string index;
            cin >> index;

            vector<int> result;
            for (const auto& rec : records) {
                if (strcmp(rec.index, index.c_str()) == 0) {
                    result.push_back(rec.value);
                }
            }

            if (result.empty()) {
                cout << "null\n";
            } else {
                sort(result.begin(), result.end());
                for (size_t j = 0; j < result.size(); j++) {
                    if (j > 0) cout << " ";
                    cout << result[j];
                }
                cout << "\n";
            }
        }
    }

    // Save data back
    ofstream outFile(DATA_FILE, ios::binary | ios::trunc);
    for (const auto& rec : records) {
        outFile.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }
    outFile.close();

    return 0;
}

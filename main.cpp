#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "DynamicArray.h"
#include "HashMap.h"
using namespace std;

string readFasta(string filename) {
    cout << "Attempting to open file: " << filename << endl;
    ifstream file(filename);
    string line, sequence = "";
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line[0] != '>') {
                sequence += line;
            }
        }
        file.close();
        cout << "File opened and sequence read successfully." << endl;
    } else {
        cout << "Could not open the file: " << filename << endl;
        return "";
    }
    return sequence;
}

string processSeq(string sequence) {
    for (char& c : sequence) {
        if (c != 'T' && c != 'C' && c != 'G' && c != 'A') {
            c = 'X';
        }
    }
    cout << "Sequence preprocessed successfully." << endl;
    return sequence;
}

int LevensheteinSequences(const string& seq1, const string& seq2, string*& mutations, int& mutationsSize) {
    int n = seq1.length();
    int m = seq2.length();

    if (n == 0 || m == 0) return -1;

    int* prev = new int[m + 1];
    int* curr = new int[m + 1];
    char** path = createCharMatrix(n + 1, m + 1);

    if (!prev || !curr || !path) {
        cerr << "Memory allocation error.\n";
        return -1;
    }

    for (int j = 0; j <= m; ++j) {
        prev[j] = j;
        if (j > 0) path[0][j] = 'L';
    }

    for (int i = 1; i <= n; ++i) {
        curr[0] = i;
        path[i][0] = 'U';
        for (int j = 1; j <= m; ++j) {
            int cost = (seq1[i - 1] == seq2[j - 1]) ? 0 : 1;
            curr[j] = prev[j - 1] + cost;
            path[i][j] = 'D';

            if (prev[j] + 1 < curr[j]) {
                curr[j] = prev[j] + 1;
                path[i][j] = 'U';
            }

            if (curr[j - 1] + 1 < curr[j]) {
                curr[j] = curr[j - 1] + 1;
                path[i][j] = 'L';
            }
        }
        swap(prev, curr);
    }

    int i = n, j = m;
    mutationsSize = 0;
    int maxMutationsSize = 10;

    while (i > 0 || j > 0) {
        char move = path[i][j];
        if (move == 'D') {
            if (seq1[i - 1] != seq2[j - 1]) {
                addStringToArray(mutations, mutationsSize, maxMutationsSize,
                    "c." + to_string(i) + seq1[i - 1] + ">" + seq2[j - 1]);
            }
            i--; j--;
        } else if (move == 'U') {
            addStringToArray(mutations, mutationsSize, maxMutationsSize,
                "c." + to_string(i) + "del");
            i--;
        } else if (move == 'L') {
            addStringToArray(mutations, mutationsSize, maxMutationsSize,
                "c." + to_string(j) + "ins" + seq2[j - 1]);
            j--;
        }
    }

    for (int k = 0; k < mutationsSize / 2; ++k) {
        swap(mutations[k], mutations[mutationsSize - 1 - k]);
    }

    int result = prev[m];
    delete[] prev;
    delete[] curr;
    deleteCharMatrix(path, n + 1);
    return result;
}

void loadMutationDatabase(HashMap& map, const string& filename, string* mutations, int mutationsSize) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open mutation database file: " << filename << endl;
        return;
    }
    string line;
    getline(file, line);
    HashMap tempMap;
    while (getline(file, line)) {
        stringstream ss(line);
        string gene, accession, url, cds;
        getline(ss, gene, ',');
        getline(ss, accession, ',');
        getline(ss, url, ',');
        getline(ss, cds, ',');
        tempMap.insert(cds, {gene, accession, url});
    }
    file.close();
    for (int i = 0; i < mutationsSize; ++i) {
        DynamicArray<MutationInfo>* result = tempMap.get(mutations[i]);
        if (result && result->size() > 0) {
            for (int j = 0; j < result->size(); ++j) {
                map.insert(mutations[i], (*result)[j]);
            }
        }
    }
    cout << "Mutation database filtered and loaded successfully.\n";
}

int main() {
    string normal_file, mutant_file;
    cout << "Please input the normal genome file:" << endl;
    cin >> normal_file;
    string normal_sequence = readFasta(normal_file);
    cout << "Please input the mutant genome file:" << endl;
    cin >> mutant_file;
    string mutant_sequence = readFasta(mutant_file);
    cout << "Normal sequence length: " << normal_sequence.length() << endl;
    cout << "Mutant sequence length: " << mutant_sequence.length() << endl;
    normal_sequence = processSeq(normal_sequence);
    mutant_sequence = processSeq(mutant_sequence);
    cout << "Performing sequence alignment..." << endl;

    int mutationsSize;
    string* mutations = createStringArray(10);
    int distance = LevensheteinSequences(normal_sequence, mutant_sequence, mutations, mutationsSize);

    cout << "Levenshtein distance: " << distance << endl;
    cout << "Mutations:" << endl;

    ofstream outfile("mutations_output.txt");
    outfile << "Levenshtein distance: " << distance << endl;
    outfile << "Mutations:" << endl;
    for (int i = 0; i < mutationsSize; ++i) {
        cout << mutations[i] << endl;
        outfile << mutations[i] << endl;
    }
    outfile.close();

    char choice;
    cout << "\nProceed with mutation lookup using HashMap? (y/n): ";
    cin >> choice;
    if (choice != 'y' && choice != 'Y') {
        cout << "Operation skipped." << endl;
        deleteStringArray(mutations);
        return 0;
    }

    HashMap mutationMap;
    loadMutationDatabase(mutationMap, "C:/Users/TOBIA/CLionProjects/untitled8/mutation_database.csv",
        mutations, mutationsSize);

    ofstream vizFile("mutation_hashmap_structure.csv");
    vizFile << "Mutation,HashValue,Gene,Accession,URL\n";

    for (int i = 0; i < mutationsSize; ++i) {
        DynamicArray<MutationInfo>* results = mutationMap.get(mutations[i]);
        int hashValue = mutationMap.getHash(mutations[i]);
        if (results && results->size() > 0) {
            for (int j = 0; j < results->size(); ++j) {
                MutationInfo res = (*results)[j];
                vizFile << mutations[i] << "," << hashValue << ","
                        << res.gene_name << ","
                        << res.accession_number << ","
                        << res.url << "\n";
            }
        }
    }
    vizFile.close();

    deleteStringArray(mutations);
    return 0;
}

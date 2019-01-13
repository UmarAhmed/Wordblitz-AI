#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>


using namespace std;


const vector<vector <int>> dir = {{0, 1}, {1, 1}, {1, 0}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {0, -1}};


// For square matrices
bool in_matrix(const int i, const int j, const int dimension) {
    return ((0 <= i) && 
            (i < dimension) &&
            (0 <= j) && 
            (j < dimension));
}


void make_unique(vector<vector<string>> &matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            matrix[i][j] += to_string(i) + to_string(j);
        }
    }
}


string vectorToString(const vector<string> &s) {
    string ret = "";
    for (int i = 0; i < s.size(); ++i) {
        ret += s[i];
    }
    return ret;
}


void getPath(const vector<vector<string>> &matrix, const int i, const int j,
        set <string> &words, vector<string> current = {}) {
    if (current.size() > 7) {
        return;
    }
    if (!in_matrix(i, j, matrix.size())) {
        return;
    }
    if (count(current.begin(), current.end(), matrix[i][j])) {
        return;
    }
    for (auto &x: dir) {
        current.emplace_back(matrix[i][j]);
        words.insert(vectorToString(current));
        getPath(matrix, i + x[0], j + x[1], words, current);
        current.pop_back();
    }
}


string getWord(const string s) {
    string ret = "";
    int i = 0;
    while (i < s.size()) {
        ret.push_back(s[i]);
        i += 3;
    }
    return ret;
}


set<string> getAllWords(vector<vector<string>> &matrix) {
    set<string> words;
    make_unique(matrix);
    getPath(matrix, 0, 0, words);
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            getPath(matrix, i, j, words);
        }
    }
    set<string> clean_words;
    for (auto i: words) {
        string word = getWord(i);
        if (word.size() > 3) {
            clean_words.insert(word);
        }
    }
    return clean_words;
}


void print_matrix(const vector<vector<string>> &matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << matrix[i][j];
        }
        cout << endl;
    }
}


int main() {
    // Load dictionary into set
    ifstream fin {"dict.txt"};
    set<string> dict;
    string word = "";
    while (fin >> word) {
        dict.insert(word);
    }
    // Load input into matrix of strings
    vector <string> current;
    vector <vector <string>> input_matrix;
    for (int i = 0; i < 4; ++i) {
        cin >> word;
        for (int j = 0; j < 4; ++j) {
            string s (1, word[j]);
            current.emplace_back(s);
        }
        input_matrix.emplace_back(current);
        current.clear();
    }
    set<string> words = getAllWords(input_matrix);
    int k = 0;
    for (auto &i: words) {
        if (dict.find(i) != dict.end()) {
            cout << i << "  ";
            k += 1;
            if (k % 8 == 0) {
                cout << endl;
            }
        }
    }
}


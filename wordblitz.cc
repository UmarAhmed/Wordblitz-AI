#include <X11/Xlib.h>
#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>


using namespace std;


const vector<vector <int>> DIREC = {{0, 1}, {1, 1}, {1, 0}, {-1, 0}, {-1, -1}, {1, -1}, {-1, 1}, {0, -1}};


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
    if (current.size() > 6) { // TODO remove hardcoded upper bound
        return;
    }
    if (!in_matrix(i, j, matrix.size())) {
        return;
    }
    if (count(current.begin(), current.end(), matrix[i][j])) {
        return;
    }
    for (auto &x: DIREC) {
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


// Click at position given by button
void mouseClick(int button)
{
    Display *display = XOpenDisplay(NULL);

    XEvent event;

    if(display == NULL)
    {
        fprintf(stderr, "Cannot initialize the display\n");
        exit(EXIT_FAILURE);
    }

    memset(&event, 0x00, sizeof(event));

    event.type = ButtonPress;
    event.xbutton.button = button;
    event.xbutton.same_screen = True;

    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

    event.xbutton.subwindow = event.xbutton.window;

    while(event.xbutton.subwindow)
    {
        event.xbutton.window = event.xbutton.subwindow;

        XQueryPointer(display, event.xbutton.window, &event.xbutton.root, &event.xbutton.subwindow, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    }

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    usleep(100000);

    event.type = ButtonRelease;
    event.xbutton.state = 0x100;

    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) fprintf(stderr, "Error\n");

    XFlush(display);

    XCloseDisplay(display);
}


void getAllWords(vector<vector<string>> &matrix, set<string> * dict) {
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
        if (dict->count(word) > 0) {
            std::cout << i << endl;
            //play(i);
        }
    }
}


void print_matrix(const vector<vector<string>> &matrix) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << matrix[i][j];
        }
        std::cout << endl;
    }
}


int main() {
    // Load dictionary into set
    ifstream fin {"dict2.txt"};
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
    getAllWords(input_matrix, &dict);
}

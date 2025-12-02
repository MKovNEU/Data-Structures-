#include<iostream>
#include <fstream>
#include <vector>
using namespace std;

enum Command {INSERT, DELETE_NODE, LOOKUP, INVALID};

const int MAX_SIZE = 30;

Command getCommand(const string &cmd) {
    if (cmd == "insert") return INSERT;
    else if (cmd == "delete") return DELETE_NODE;
    else if (cmd == "lookup") return LOOKUP;
    else return INVALID;
}

struct Node {
    int key;
    Node *right;
    Node *down;

    Node(int v) : key(v), right(nullptr), down(nullptr) {};

    Node(int v, Node *r) : key(v), right(r), down(nullptr) {};

    Node(int v, Node *r, Node *d) : key(v), right(r), down(d) {};
};

bool flip() {
    int randomNum = rand() % 2;
    return (randomNum % 2 == 0);
}

void insert(vector<Node *> &v, int key) {
    int stackSize = 1;
    while(flip() && stackSize < MAX_SIZE) {
        stackSize++;
    }
    vector<Node *> lastVisited(MAX_SIZE, nullptr);
    Node *y = v[0];
    for(int i = 0; i < MAX_SIZE; i++) {
        while(y->right != nullptr && y->right->key < key){
            y = y->right;
        }
        lastVisited[i] = y;
        y = y->down;
    }

    lastVisited[MAX_SIZE - 1]->right = new Node(key, lastVisited[MAX_SIZE - 1]->right);

    for (int i = 2; i <= stackSize; i++) {
        lastVisited[MAX_SIZE - i]->right = new Node(key, lastVisited[MAX_SIZE - i]->right, lastVisited[MAX_SIZE - (i - 1)]->right);
    }
}

void Delete(vector<Node *> &v, int key) {
    vector<Node *> lastVisited(MAX_SIZE, nullptr);
    Node *y = v[0];
    for(int i = 0; i < MAX_SIZE; i++) {
        while(y->right != nullptr && y->right->key < key){
            y = y->right;
        }
        lastVisited[i] = y;
        y = y->down;
    }
    for (int i = 1; i <= MAX_SIZE; i++) {
        if (lastVisited[MAX_SIZE - i]->right != nullptr && lastVisited[MAX_SIZE - i]->right->key == key){
            lastVisited[MAX_SIZE - i]->right = lastVisited[MAX_SIZE - i]->right->right;
        }
    }
}

Node *lookup(vector<Node *> &v, int key) {
    Node *y = v[0];
    for(int i = 0; i < MAX_SIZE; i++) {
        while(y->right != nullptr && y->right->key < key){
            y = y->right;
            cout << "right" << " ";
        }
        if (i != MAX_SIZE - 1) {
            y = y->down;
            cout << "down" << " ";
        }
    }
    if (y->right != nullptr && y->right->key == key) return y->right;
    else return nullptr;
}

int main() {
    vector<Node *> v;
    v.push_back(new Node(-1));
    for (int i = 1; i < MAX_SIZE; i++) {
        v.push_back(new Node(-1));
        v[i-1]->down = v[i];
    }

    while (true) {
        cout << "> ";
        string cmd;
        cin >> cmd;
        Command type = getCommand(cmd);
        switch (type) {
            case INSERT: {
                int val;
                cin >> val;
                insert(v, val);
                cout << "Inserted " << val << "\n";
                break;
            }

            case DELETE_NODE: {
                int val;
                cin >> val;
                if (lookup(v, val) != nullptr){
                    Delete(v, val);
                    cout << "Deleted " << val << "\n";
                } else {
                    cout << "Not in list " << val << "\n";
                }
                break;
            }

            case LOOKUP: {
                int val;
                cin >> val;
                if (lookup(v, val) != nullptr){
                    cout << "Found " << val << "\n";
                } else {
                    cout << "Not in list " << val << "\n";
                }
                break;
            }

            case INVALID:
                cout << "Invalid command.\n";
                break;
        }
    }
    return 0;
}
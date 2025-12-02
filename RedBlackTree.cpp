#include<iostream>
#include <fstream>
#include <vector>
#include <queue> 
using namespace std;

enum Command {SORT, SEARCH, MAX, MIN, SUCCESSOR, PREDECESSOR, LEFTROTATION, RIGHTROTATION, INSERT, DELETE_NODE, INVALID, PRINT};

Command getCommand(const string &cmd) {
    if (cmd == "sort") return SORT;
    else if (cmd == "search") return SEARCH;
    else if (cmd == "max") return MAX;
    else if (cmd == "min") return MIN;
    else if (cmd == "successor") return SUCCESSOR;
    else if (cmd == "predecessor") return PREDECESSOR;
    else if (cmd == "leftrotation") return LEFTROTATION;
    else if (cmd == "rightrotation") return RIGHTROTATION;
    else if (cmd == "insert") return INSERT;
    else if (cmd == "delete") return DELETE_NODE;
    else if (cmd == "print") return PRINT;
    else return INVALID;
}

struct Node {
    int key;
    bool color; //black false, red true
    Node *p;
    Node *leftChild;
    Node *rightChild;

    Node(int keyValue) : key(keyValue), color(false),
        p(nullptr), leftChild(nullptr), rightChild(nullptr){}
};

void sort(Node *head) {
    if (head != nullptr){
        sort(head->leftChild);
        cout << head->key << " ";
        sort(head->rightChild);
    }
}

Node *search(Node *head, int key) {
    if (head == nullptr){
       return nullptr;
    }
    if (key < head->key) {
        return search(head->leftChild, key);
    } else if (key > head->key) {
        return search(head->rightChild, key);
    }
    return head;
}

Node *max(Node *head){
    if (head == nullptr) return nullptr;
    while(head->rightChild != nullptr) {
        head = head->rightChild;
    }
    return head;
}

Node *min(Node *head){
    if (head == nullptr) return nullptr;
    while(head->leftChild != nullptr) {
        head = head->leftChild;
    }
    return head;
}

Node *successor(Node *x) {
    if (x->rightChild != nullptr) {
        return min(x->rightChild);
    } else {
        Node *y = x->p;
        while(y != nullptr && x == y->rightChild) {
            x = y;
            y = y->p;
        }
        return y;
    }
}

Node *predecessor(Node *x) {
    if (x->leftChild != nullptr) {
        return max(x->leftChild);
    } else {
        Node *y = x->p;
        while(y != nullptr && x == y->leftChild) {
            x = y;
            y = y->p;
        }
        return y;
    }
}

void leftRotation(Node *&head, Node *x) {
    if (x == nullptr || x->rightChild == nullptr) {
        return;
    }
    Node *y = x->rightChild;
    x->rightChild = y->leftChild;
    if (y->leftChild != nullptr) {
        y->leftChild->p = x;
    }
    y->p = x->p;
    if (x->p == nullptr){
        head = y;
    } else if (x == x->p->leftChild) {
        x->p->leftChild = y;
    } else {
        x->p->rightChild = y;
    }
    y->leftChild = x;
    x->p = y;
}

void rightRotation(Node *&head, Node *x) {
    if (x == nullptr || x->leftChild == nullptr) {
        return;
    }
    Node *y = x->leftChild;
    x->leftChild = y->rightChild;
    if (y->rightChild != nullptr) {
        y->rightChild->p = x;
    }
    y->p = x->p;
    if (x->p == nullptr){
        head = y;
    } else if (x == x->p->leftChild) {
        x->p->leftChild = y;
    } else {
        x->p->rightChild = y;
    }
    y->rightChild = x;
    x->p = y;
}

void colorFixInsert(Node *&head, Node *z) {
    while(z->p != nullptr && z->p->p != nullptr && z->p->color) {
        if (z->p == z->p->p->leftChild) {
            Node *y = z->p->p->rightChild;
            if (y != nullptr && y->color) {
                z->p->color = false;
                y->color = false;
                z->p->p->color = true;
                z = z->p->p;
            }
            else {
                if (z == z->p->rightChild) {
                    z = z->p;
                    leftRotation(head, z);
                }
                z->p->color = false;
                z->p->p->color = true;
                rightRotation(head, z->p->p);
            }
        }
        else {
            Node *y = z->p->p->leftChild;
            if (y != nullptr && y->color) {
                z->p->color = false;
                y->color = false;
                z->p->p->color = true;
                z = z->p->p;
            }
            else {
                if (z == z->p->leftChild) {
                    z = z->p;
                    rightRotation(head, z);
                }
                z->p->color = false;
                z->p->p->color = true;
                leftRotation(head, z->p->p);
            }
        }
    }
    head->color = false;
}

void insert(Node *&head, Node *z){
    Node *x = head;
    Node *y = nullptr;
    while(x != nullptr) {
        y = x;
        if (z->key < x->key) {
            x = x->leftChild;
        }
        else {
            x = x->rightChild;
        }
    }
    z->p = y;
    if (y == nullptr) {
        head = z;
        z->color = false;
        return;
    }else if (z->key < y->key) {
        y->leftChild = z;
    } else {
        y->rightChild = z;
    }
    z->color = true;
    colorFixInsert(head, z);
}

void transplant(Node *&head, Node *u, Node *v) {
    if (u->p == nullptr) {
        head = v;
    } else if (u == u->p->leftChild) { 
        u->p->leftChild = v;
    } else {
        u->p->rightChild = v;
    }
    if (v != nullptr) {
        v->p = u->p;
    }
}

void Delete(Node *&head, Node *z) {
    if (z->leftChild == nullptr) {
        transplant(head, z, z->rightChild);
    } else if (z->rightChild == nullptr) {
        transplant(head, z, z->leftChild);
    } else {
        Node *y = min(z->rightChild);
        if (y != z->rightChild) {
            transplant(head, y, y->rightChild);
            y->rightChild = z->rightChild;
            y->rightChild->p = y;
        }
        transplant(head, z, y);
        y->leftChild = z->leftChild;
        y->leftChild->p = y;
    }
}

int height(Node *head) {
    if (head == nullptr) return 0;
    return 1 + max(height(head->leftChild), height(head->rightChild));
}

void printTree(Node* root) {
    if (!root) {
        cout << "(empty tree)\n";
        return;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();

        cout << "[ ";
        for (int i = 0; i < levelSize; i++) {
            Node* cur = q.front();
            q.pop();

            if (cur) {
                cout << (cur->color ? "R-" : "B-") << cur->key;
                q.push(cur->leftChild);
                q.push(cur->rightChild);
            } else {
                cout << "null";
                q.push(nullptr);
                q.push(nullptr);
            }

            if (i + 1 < levelSize) cout << ", ";
        }
        cout << " ]\n";

        bool allNull = true;
        queue<Node*> copy = q;
        while (!copy.empty()) {
            if (copy.front() != nullptr) { allNull = false; break; }
            copy.pop();
        }
        if (allNull) break;
    }
}


int main(){
    ifstream file("input.txt");

    int x;
    vector<int> v;
    while(file >> x){
        v.push_back(x);
    }
    Node *RBTHead = nullptr;
    for (int key : v){
        Node *node = new Node(key);
        insert(RBTHead, node);
    }
    while (true) {
        cout << "> ";
        string cmd;
        cin >> cmd;
        Command type = getCommand(cmd);
        switch (type) {
            case SORT:
                sort(RBTHead);
                cout << "\n";
                break;

            case SEARCH: {
                int val;
                cin >> val;
                Node *n = search(RBTHead, val);
                if (n) cout << "Found: " << n->key << "\n";
                else cout << "Not found\n";
                break;
            }

            case MAX: {
                Node *m = max(RBTHead);
                if (m) cout << "Max: " << m->key << "\n";
                else cout << "Tree empty\n";
                break;
            }

            case MIN: {
                Node *m = min(RBTHead);
                if (m) cout << "Min: " << m->key << "\n";
                else cout << "Tree empty\n";
                break;
            }

            case SUCCESSOR: {
                int val;
                cin >> val;
                Node *n = search(RBTHead, val);
                if (n) {
                    Node *s = successor(n);
                    if (s) cout << "Successor of " << val << " is " << s->key << "\n";
                    else cout << "No successor\n";
                } else cout << "Not found\n";
                break;
            }

            case PREDECESSOR: {
                int val;
                cin >> val;
                Node *n = search(RBTHead, val);
                if (n) {
                    Node *p = predecessor(n);
                    if (p) cout << "Predecessor of " << val << " is " << p->key << "\n";
                    else cout << "No predecessor\n";
                } else cout << "Not found\n";
                break;
            }

            case LEFTROTATION: {
                int val;
                cin >> val;
                Node *n = search(RBTHead, val);
                if (n) leftRotation(RBTHead, n);
                else cout << "Node not found\n";
                break;
            }

            case RIGHTROTATION: {
                int val;
                cin >> val;
                Node *n = search(RBTHead, val);
                if (n) rightRotation(RBTHead, n);
                else cout << "Node not found\n";
                break;
            }

            case INSERT: {
                int val;
                cin >> val;
                Node *n = new Node(val);
                insert(RBTHead, n);
                cout << "Inserted " << val << "\n";
                break;
            }

            case DELETE_NODE: {
                int val;
                cin >> val;
                Node *n = search(RBTHead, val);
                if (n) {
                    Delete(RBTHead, n);
                    cout << "Deleted " << val << "\n";
                } else cout << "Node not found\n";
                break;
            }

            case PRINT:
                printTree(RBTHead);
                break;

            case INVALID:
                cout << "Invalid command.\n";
                break;
        }
    cout << "Tree height: " << height(RBTHead) << "\n";
    }
    return 0;
}
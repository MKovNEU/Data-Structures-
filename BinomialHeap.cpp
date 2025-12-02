#include<iostream>
#include <fstream>
#include <vector>
using namespace std;

enum Command {
    MAKE_HEAP,
    INSERT,
    MINIMUM,
    EXTRACT_MIN,
    UNION_HEAPS,
    DECREASE_KEY,
    DELETE_NODE,
    PRINT,
    INVALID
};

Command getCommand(const string &cmd) {
    if (cmd == "makeheap") return MAKE_HEAP;
    else if (cmd == "insert") return INSERT;
    else if (cmd == "minimum") return MINIMUM;
    else if (cmd == "extractmin") return EXTRACT_MIN;
    else if (cmd == "union") return UNION_HEAPS;
    else if (cmd == "decreasekey") return DECREASE_KEY;
    else if (cmd == "delete") return DELETE_NODE;
    else if (cmd == "print") return PRINT;
    else return INVALID;
}

struct Node {
    int key;
    int deg; //number of children the node has
    Node *p;
    Node *child; //left most child
    Node *rightSibling; //the sibling to the direct right, ie they share the same parent.

    Node(int v) : key(v), deg(0), p(nullptr), child(nullptr), rightSibling(nullptr) {}
};

Node **makeHeap() {
    return new Node*(nullptr);
}

Node *min(Node **head) {
    Node *y = nullptr;
    Node *x = (*head);
    int min = INT_MAX;
    while(x != nullptr) {
        if (x->key < min) {
            min = x->key;
            y = x;
        }
        x = x->rightSibling;
    }
    return y;
}

Node **heapMerge(Node **h1, Node **h2) {
    Node **head = makeHeap();
    Node *x = *h1;
    Node *y = *h2;
    Node *headNode = nullptr;
    Node *tail = nullptr; 
    while(x != nullptr || y != nullptr) {
        Node *nextNode = nullptr;
        if (x == nullptr) {
            nextNode = y; 
            y = y->rightSibling;
        } else if (y == nullptr) {
            nextNode = x; 
            x = x->rightSibling;
        } else if (x->deg <= y->deg) {
            nextNode = x; 
            x = x->rightSibling;
        } else {
            nextNode = y; 
            y = y->rightSibling;
        }

        if (tail == nullptr) {
            headNode = nextNode;
        } else {
            tail->rightSibling = nextNode;
        }
        tail = nextNode;
    }
    if (tail != nullptr) {
        tail->rightSibling = nullptr;
    }
    *head = headNode;
    return head;
}

void treeLink(Node *x, Node *y) {
    x->rightSibling = y->child;
    y->child = x;
    x->p = y;
    y->deg++;
}

Node **heapUnion(Node **h1, Node **h2) {
    Node **head = heapMerge(h1, h2);
    if (*head == nullptr) return nullptr;
    Node *prev = nullptr;
    Node *x = *head;
    Node *next = x->rightSibling;
    while(next != nullptr) {
        if (x->deg != next->deg || (next->rightSibling != nullptr && next->rightSibling->deg == x->deg)) {
            prev = x;
            x = next;
        } else if (x->key <= next->key) {
            x->rightSibling = next->rightSibling;
            treeLink(next, x);
        } else {
            if (prev == nullptr) {
                *head = next;
            } else {
                prev->rightSibling = next;
            }
            treeLink(x, next); 
            x = next;
        }
        next = next->rightSibling;
    }
    return head;
}

void insert(Node **head, Node *x) {
    Node **head2 = makeHeap();
    *head2 = x;
    *head = *heapUnion(head, head2);
}

void extractMin(Node **head) {
    Node *y = nullptr;
    Node *z = nullptr;
    Node *x = (*head);
    int min = INT_MAX;
    while(x != nullptr) {
        if (x->key < min) {
            min = x->key;
            y = z;
        }
        z = x;
        x = x->rightSibling;
    }
    if (!y) {
        x = (*head)->child;
        *head = (*head)->rightSibling;
    } else {
        x = y->rightSibling->child;
        y->rightSibling = y->rightSibling->rightSibling;
    }
    Node **head2 = makeHeap();
    Node *prev = nullptr;
    Node *next = nullptr;
    while (x != nullptr) {
        next = x->rightSibling;
        x->rightSibling = prev;
        prev = x;
        x = next;
    }
    *head2 = prev;
    *head = *heapUnion(head, head2);

}

void decreaseKey(Node **head, Node *x, int dec) {
    if (dec > x->key) {
        cout << "dec bigger than key";
        return;
    }
    x->key = dec;
    Node *y = x;
    Node *z = y->p;
    while(z != nullptr && y->key < z->key) {
        int temp = y->key;
        y->key = z->key;
        z->key = temp;
        y = z;
        z = y->p;
    }
}

void Delete(Node **head, Node *x) {
    decreaseKey(head, x, INT_MIN);
    extractMin(head);
}

void printTree(Node* root, int depth = 0) {
    if (!root) return;
    for (int i = 0; i < depth; i++) cout << "  ";
    cout << root->key << " (deg=" << root->deg << ")\n";
    printTree(root->child, depth + 1);
    if (depth != 0) {printTree(root->rightSibling, depth);}
}

void printHeap(Node** head) {
    Node* curr = *head;
    if (!curr) {
        cout << "Heap is empty.\n";
        return;
    }
    cout << "Binomial Heap:\n";
    while (curr) {
        cout << "B" << curr->deg << ":\n";
        printTree(curr);
        curr = curr->rightSibling;
        cout << "\n";
    }
}


int main() {
    Node** H = makeHeap();

    while (true) {
        cout << "> ";
        string cmd;
        cin >> cmd;
        Command c = getCommand(cmd);

        switch (c) {
            case MAKE_HEAP:
                delete H;
                H = makeHeap();
                cout << "New heap created.\n";
                break;

            case INSERT: {
                int val;
                cin >> val;
                insert(H, new Node(val));
                cout << "Inserted " << val << ".\n";
                break;
            }

            case MINIMUM: {
                Node* m = min(H);
                if (m) cout << "Minimum: " << m->key << "\n";
                else cout << "Heap is empty.\n";
                break;
            }

            case EXTRACT_MIN:
                extractMin(H);
                cout << "Extracted minimum.\n";
                break;

            case DECREASE_KEY: {
                int oldVal, newVal;
                cin >> oldVal >> newVal;
                Node* curr = *H;
                while (curr && curr->key != oldVal)
                    curr = curr->rightSibling;
                if (curr) {
                    decreaseKey(H, curr, newVal);
                    cout << "Decreased " << oldVal << " to " << newVal << ".\n";
                } else {
                    cout << "Key not found.\n";
                }
                break;
            }

            case DELETE_NODE: {
                int val;
                cin >> val;
                Node* curr = *H;
                while (curr && curr->key != val)
                    curr = curr->rightSibling;
                if (curr) {
                    Delete(H, curr);
                    cout << "Deleted " << val << ".\n";
                } else {
                    cout << "Key not found.\n";
                }
                break;
            }

            case PRINT:
                printHeap(H);
                break;

            case INVALID:
                cout << "Invalid command.\n";
                break;

            default:
                cout << "Unimplemented command.\n";
                break;
        }
    }
    return 0;
}
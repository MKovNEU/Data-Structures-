#include<iostream>
#include <fstream>
#include <vector>
#include <forward_list>

using namespace std;


const int M = 1000;

int myHash(const string &str) {
    long long sum = 0;
    long long  mult = 1;
    for (unsigned char c : str) {
        int val = tolower(c);
        sum = (sum + mult * val) % 1009; 
        mult = (mult * 53) % 1009;
    }
    return sum % M;
}



bool find(vector<forward_list<pair<string, int> >*> &v, const string &key) {
    forward_list<pair<string, int>> *list = v[myHash(key)];
    for (auto it = list->begin(); it != list->end(); ++it) {
        if (it->first == key) return true;
    }
    return false;
}

forward_list<pair<string, int>>::iterator findIt(vector<forward_list<pair<string, int> >*> &v, const string &key) {
    forward_list<pair<string, int>> *list = v[myHash(key)];
    for (auto it = list->begin(); it != list->end(); ++it) {
        if (it->first == key) return it;
    }
    return list->end();
}

void insert(vector<forward_list<pair<string, int> >*> &v, const string &key, int value) {
    forward_list<pair<string, int>>::iterator it = findIt(v, key);
    int h = myHash(key);
    if (it == v[h]->end()) {
        v[h]->push_front(make_pair(key, value));
    }
    else {
        it->second = value;
    }
}

void Delete(vector<forward_list<pair<string, int> >*> &v, const string &key, int value) {
    forward_list<pair<string, int>>::iterator it = findIt(v, key);
    int h = myHash(key);
    if (it != v[h]->end()) {
        v[h]->remove(*it);
    }
}

void increase(vector<forward_list<pair<string, int> >*> &v, const string &key) {
    forward_list<pair<string, int>>::iterator it = findIt(v, key);
    int h = myHash(key);
    if (it != v[h]->end()) {
        it->second++;
    }
}

void list_all_keys(vector<forward_list<pair<string, int> >*> &v){
    for (forward_list<pair<string, int>> *list : v) {
        for (auto it = list->begin(); it != list->end(); ++it) {
            cout << it->first << "\n";
        }
    }
}


int main() {
    vector<forward_list<pair<string, int> >*> v;
    for (int i = 0; i < M; i++) {
        v.push_back(new forward_list<pair<string, int>>);
    }
    
    ifstream file("input.txt");
    string x;
    while (file >> x) {
        if (find(v, x)){
            increase(v, x);
        } else {
            insert(v, x, 1);
        }
    }
    vector<int> dist; 
    double sum = 0.0;
    for (int i = 0; i < M; i++) {
        dist.push_back(distance(v[i]->begin(), v[i]->end()));
        sum += dist[i];
    }
    double mean = sum / M;
    double variance = 0.0; 
    for (int i = 0; i < M; i++) {
        variance += pow((mean - dist[i]), 2.0);
    }
    variance /= M;
    cout << "Variance for size: " << M << " is : " << variance << "\n";
    for (int i = 0; i < M; i++) {
        //cout << dist[i] << " ";
    }
    list_all_keys(v);
    cout << "\n";
}
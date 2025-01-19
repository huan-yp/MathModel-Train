#ifndef PLAIN_H
#define PLAIN_H

#include <bits/stdc++.h>
using namespace std;

class Node{
public:
    double c, w, it;
    int x, y;
    Node(){

    }
    Node(double c, double w, double it, int x, int y){
        this->c = c;
        this->w = w;
        this->x = x;
        this->y = y;
        this->it = it;
    }
};

class Plain{
public:
    vector<vector<Node>> a;
    Plain(int n, int m){
        a.resize(n, vector<Node>(m));
    }
};

#endif
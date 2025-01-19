#ifndef PLAIN_H
#define PLAIN_H

#include <bits/stdc++.h>

using namespace std;

const int MAX_W = 30;

class Node{
public:
    double c, w, it;
    int x, y;
    Node(){

    }
    Node(int x, int y){
        this->x = x;
        this->y = y;
    }
    Node(int x, int y, double c, double w, double it){
        this->c = c;
        this->w = w;
        this->x = x;
        this->y = y;
        this->it = it;
    }
    pair<double, double> center(){
        return make_pair((x + 0.5) * 0.25, (y + 0.5) * 0.25);
    }
};

class Plain{
public:
    vector<vector<Node>> a;
    Plain(){
        
    }
    void read(istream &in){
        int n, m;
        cin >> n >> m;
        a.resize(n, vector<Node>(m));
        for(int i = 0; i < a.size(); i++){
            for(int j = 0; j < a[i].size(); j++){
                in >> a[i][j].c >> a[i][j].it;
                a[i][j].x = i, a[i][j].y = j, a[i][j].w = MAX_W;
            }
        }
    }
    void modify_weight(vector<vector<bool>> cover){
        for(int i = 0; i < a.size(); i++){
            for(int j = 0; j < a[i].size(); j++){
                if(!cover[i][j]){
                    a[i][j].w--;
                }
            }
        }
    }
    int rows(){
        return a.size();
    }
    int cols(){
        return a[0].size();
    }
};

#endif
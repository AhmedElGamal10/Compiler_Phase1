//
// Created by ahmed on 24/03/17.
//

#ifndef CPP_STATE_H
#define CPP_STATE_H

#include <vector>
#include <map>
#include "bits/stdc++.h
#include "iostream"

using namespace std;

class State {
public:
    int type;   //0 -> start    1->intermediate     2-accepting
    string forTransition;   //"letter"  "digit"     "digits"
    map<char, vector<State>> next;
};

#endif //CPP_STATE_H

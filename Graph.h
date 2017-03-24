//
// Created by ahmed on 21/03/17.
//
#include "bits/stdc++.h"
#include "State.h"

#ifndef CPP_GRAPH_H
#define CPP_GRAPH_H
const string EPS = "#";

class Graph {
public:
    Graph();

    bool closureApplied;
    State start;
    vector<State> intermediate;
    State accepting;
};

#endif //CPP_GRAPH_H

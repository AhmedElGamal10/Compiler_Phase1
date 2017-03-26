//
// Created by ahmed on 21/03/17.
//
#include "bits/stdc++.h"
#include "State.h"

#ifndef CPP_GRAPH_H
#define CPP_GRAPH_H
const string EPS = "#";
const string LOGICAL = "relop";
class Graph {
public:
    Graph();

    Graph(State start, State accepting);

    bool closureApplied;
    State start;
    State accepting;
};

#endif //CPP_GRAPH_H

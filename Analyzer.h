//
// Created by ahmed on 21/03/17.
//
#include "bits/stdc++.h"
#include "Graph.h"

#ifndef CPP_ANALYZER_H
#define CPP_ANALYZER_H

//private:
//stack<Graph> subgraphes;


class Analyzer {

private:
    std::stack<Graph> subGraphes;


public:
    Analyzer();
    void constructGraph(std::string expression);

};

#endif //CPP_ANALYZER_H

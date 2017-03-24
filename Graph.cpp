//
// Created by ahmed on 21/03/17.
//

#include "Graph.h"


Graph::Graph() {
    start.type = 0;
    accepting.type = 2;

    closureApplied = false;

    State intermediateState;
    intermediateState.type = 1;

    intermediateState.next[EPS].push_back(accepting);
    start.next[EPS].push_back(intermediateState);

    start.onEntringEdge;

    intermediate.push_back(intermediateState);
}

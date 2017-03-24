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

    start.next[EPS] = intermediateState;
    intermediateState.next[EPS] = accepting;

    intermediate.push_back(intermediateState);
}

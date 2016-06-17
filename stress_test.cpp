#include <iostream>
#include <cassert>
#include <utility>
#include <algorithm>
#include <set>
#include "graph.h"

using namespace std;

void stress_test(IMPL impl) {
    uint nodes_q = 7500;//, edges_q = nodes_q*(nodes_q-1) / 2;
    set<pair<uint,uint> > edges_suite;

    for (uint from = 0; from < nodes_q; ++from) {
        for (uint to = 0; to < nodes_q; ++to) {
            if(from == to) continue;
            
            edges_suite.insert(make_pair(from, to));
        }
    }

    srand(time(NULL));
    Graph g(nodes_q, true, impl);
    for (auto it = edges_suite.begin(); it != edges_suite.end(); ++it) {
        g.applyEdge(it->first, it->second, rand());
    }
}

int main() {
    printf("stress_test (ADJACENCIES_LIST) started... ");
    stress_test(ADJACENCIES_LIST);
    printf("passed!\n");

    printf("stress_test (ADJACENCIES_MATRIX) started... ");
    stress_test(ADJACENCIES_MATRIX);
    printf("passed!\n");

    printf("\nAll test passed!!!!!!!\n\n");
}
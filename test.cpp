#include <iostream>
#include <cassert>
#include <utility>
#include <algorithm>
#include <set>
#include "graph.h"

typedef unsigned int uint;

using namespace std;

void test_addAndGetNodes(IMPL impl) {
    uint nodes = 200, extraNodes = 300;
    Graph g(nodes, impl);

    assert(g.getNodesCount() == nodes);

    for (uint i = 1; i <= extraNodes; ++i) {
        g.addVertex();
        assert(g.getNodesCount() == nodes+i);
    }

    for (uint i = 0; i < nodes + extraNodes; ++i) {
        assert(g.isIsolatedNode(i));
    }
}

void test_addAndGetEdges(IMPL impl) {
    srand(time(NULL));
    set<pair<uint, uint> > savedEdges;
    uint nodes = 200,
         edges_count = (nodes*(nodes-1)/2) * 0.5; //50% of possible edges

    Graph g(nodes, impl);

    //Add some edges, checking adjacency, and store them in an auxiliar set
    for (uint i = 0; i < edges_count; ++i) {
        uint from, to;

        do {
            from = rand()%nodes;
            to = rand()%nodes;
        } while(to == from || g.areAdjacent(from, to));

        g.applyEdge(from, to);
        assert(g.areAdjacent(from, to));
        
        savedEdges.insert(make_pair(from, to));
    }

    //Compare edges vs edges stored in the auxiliar structure
    list<Graph::Edge>& edges = g.getEdges();
    assert(g.getEdgesCount() == edges_count);
    assert(edges.size() == savedEdges.size());
    for (auto it = edges.begin(); it != edges.end(); it++) {
        auto p = make_pair(it->from, it->to);
        assert(savedEdges.find(p) != savedEdges.end());
    }
}

void test_edgeWeight(IMPL impl) {
    uint nodes = 200,
         edges_count = (nodes*(nodes-1)/2) * 0.5; //50% of possible edges

    Graph g(nodes, impl);

    //Add some edges, checking adjacency, and store them in an auxiliar set
    for (uint i = 0; i < edges_count; ++i) {
        uint from, to;
        float weight;

        do {
            from = rand()%nodes;
            to = rand()%nodes;
            weight = rand()%100 / rand()%100;
        } while(to == from || g.areAdjacent(from, to));

        g.applyEdge(from, to, weight);

        assert(g.areAdjacent(from, to));
        assert(g.getEdgeWeight(from, to) == weight);
    }

    assert(g.getEdgesCount() == edges_count);
}

void test_neighbors(IMPL impl) {
    uint nodes = 200;
    Graph g(nodes, impl, true);

    for (uint current = 0; current < nodes; current++) {
        for (uint to = current+1; to < nodes; to++) {
            g.applyEdge(current, to);
        }
    }

    for (uint current = 0; current < nodes; current++) {
        uint expected = current+1;
        for (auto it = g.adjacentsOf(current); it.thereIsMore(); it.advance()) {
            assert(it.next().first == expected);
            expected++;
        }
    }
}

void stress_test(IMPL impl) {
    uint nodes_q = 100;//, edges_q = nodes_q*(nodes_q-1) / 2;
    set<pair<uint,uint> > edges_suite;
    Graph g(nodes_q, impl, true);

    for (uint from = 0; from < nodes_q; ++from) {
        for (uint to = 0; to < nodes_q; ++to) {
            if(from == to) continue;
            
            edges_suite.insert(make_pair(from, to));
        }
    }

    srand(time(NULL));
    for (auto it = edges_suite.begin(); it != edges_suite.end(); ++it) {
        g.applyEdge(it->first, it->second, rand());
    }
}

int main() {
    test_addAndGetNodes(ADJACENCIES_MATRIX);
    test_addAndGetNodes(ADJACENCIES_LIST);
    printf("Test addAndGetNodes passed!\n");

    test_addAndGetEdges(ADJACENCIES_MATRIX);
    test_addAndGetNodes(ADJACENCIES_LIST);
    printf("Test addAndGetEdges passed!\n");

    test_edgeWeight(ADJACENCIES_MATRIX);
    test_edgeWeight(ADJACENCIES_LIST);
    printf("Test edgeWeight passed!\n");

    test_neighbors(ADJACENCIES_MATRIX);
    test_neighbors(ADJACENCIES_LIST);
    printf("Test neighbors passed!\n");

    stress_test(ADJACENCIES_LIST);
    printf("stress_test (ADJACENCIES_LIST) passed!\n");
    stress_test(ADJACENCIES_MATRIX);
    printf("stress_test (ADJACENCIES_MATRIX) passed!\n");

    printf("\nAll test passed!!!!!!!\n\n");

    return 0;
}
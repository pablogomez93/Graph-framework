#include <iostream>
#include <cassert>
#include <utility>
#include <algorithm>
#include <set>
#include "graph.h"

typedef unsigned int uint;

using namespace std;

void test_addAndGetNodes(IMPL impl) {
    uint nodes = 100, extraNodes = 50;
    Graph g(nodes, false, impl);

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
    uint nodes = 100,
         edges_count = (nodes*(nodes-1)/2) * 0.5; //50% of edges

    Graph g(nodes, false, impl);

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
    uint nodes = 100,
         edges_count = (nodes*(nodes-1)/2) * 0.5; //50% of edges

    Graph g(nodes, false, impl);

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
    uint nodes = 100;
    Graph g(nodes, true, impl);

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

void test_DFS(IMPL impl) {
    uint nodes = 9, i;
    Graph g(nodes, true, impl);    
    
    //Test some edges
    g.applyEdge(0, 1);
    g.applyEdge(0, 2);
    g.applyEdge(2, 3);
    g.applyEdge(2, 4);
    g.applyEdge(3, 5);
    g.applyEdge(3, 6);
    g.applyEdge(3, 7);
    g.applyEdge(4, 7);
    g.applyEdge(4, 8);
    g.applyEdge(6, 8);

    i = 0;
    uint expected_dfs1[9] = {0, 2, 4, 8, 7, 3, 6, 5, 1};
    for (Graph::DFSIterator dfs1(0, &g); dfs1.thereIsMore(); dfs1.advance()) {
       assert(dfs1.next() == expected_dfs1[i++]);
    }

    //Test full graph
    g.fill();

    i = 0;
    uint expected_dfs2[9] = {0, 8, 7, 6, 5, 4, 3, 2, 1}; 
    for (Graph::DFSIterator dfs2(0, &g); dfs2.thereIsMore(); dfs2.advance()) {
       assert(dfs2.next() == expected_dfs2[i++]);
    }
}

void test_BFS(IMPL impl) {
    uint nodes = 9, i;
    Graph g(nodes, true, impl);    
    uint expected_bfs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    //Test some edges
    g.applyEdge(0, 1);
    g.applyEdge(0, 2);
    g.applyEdge(2, 3);
    g.applyEdge(2, 4);
    g.applyEdge(3, 5);
    g.applyEdge(3, 6);
    g.applyEdge(3, 7);
    g.applyEdge(4, 7);
    g.applyEdge(4, 8);
    g.applyEdge(6, 8);

    i = 0;
    for (Graph::BFSIterator bfs1(0, &g); bfs1.thereIsMore(); bfs1.advance()) {
       assert(bfs1.next() == expected_bfs[i++]);
    }

    //Test full graph
    g.fill();

    i = 0;
    for (Graph::BFSIterator bfs2(0, &g); bfs2.thereIsMore(); bfs2.advance()) {
       assert(bfs2.next() == expected_bfs[i++]);
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

    test_DFS(ADJACENCIES_MATRIX);
    test_DFS(ADJACENCIES_LIST);
    printf("Test DFS passed!\n");

    test_BFS(ADJACENCIES_MATRIX);
    test_BFS(ADJACENCIES_LIST);
    printf("Test BFS passed!\n");

    printf("\nAll test passed!!!!!!!\n\n");

    return 0;
}
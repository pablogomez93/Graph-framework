#ifndef __GRAPH__H__
#define __GRAPH__H__

#include <vector>
#include <list>
#include <string>

typedef unsigned int uint;

enum IMPL {ADJACENCIES_MATRIX, ADJACENCIES_LIST};

class Graph {

public:
    class AdjacentsIterator;
    
    struct Edge {
        uint from;
        uint to;
        float weight;
        bool painted;

        Edge(uint f, uint t, float w) {
            this->from = f;
            this->to = t;
            this->weight = w;
            this->painted = false;
        }

        Edge(const Edge& other) {
            this->from = other.from;
            this->to = other.to;
            this->weight = other.weight;
            this->painted = other.painted;
        }
    };

    struct Node {
        bool isolated;
        bool painted;
        int dIn;
        int dOut;

        Node() {
            isolated = true;
            dIn = 0;
            dOut = 0;
            painted = false;
        }
    };

    /*
     * Construct a simple graph, with n nodes.
     * Nodes are represented by numbers between 0 and n-1 inclusive.
     * Is necesary to define if the graph is oriented or not and if it is implemented 
       over adjacencies matrix (ADJACENCIES_MATRIX) or adjacencies lists (ADJACENCIES_LIST).
       If not nodes count is specified, the default value is 0.
       If not orientation is specified, the default value is false.
       If not implementation is specified, the default imple is ADJACENCIES_MATRIX
     * Each impl gets different execution times for each graph operation.
     */
    Graph(uint n = 0, bool isOrientedGraph = false, IMPL impl = ADJACENCIES_MATRIX);

    /*
     * Applies an edge with a weight (optional), between two diferent nodes v1 and v2 (nodes must be already applied to the graph).
     * This method is util when the graph has weight in each edge.
     */
    void applyEdge(uint v1, uint v2, float weight = 1);

    /*
     * In non-oriented graph, returns if nodes v1 and v2, are adjacents, that means if exist an edge between both.
     * If graph is oriented, returns if exist an edge from node v1 to node v2.
     */
    bool areAdjacent(uint v1, uint v2) const;

    /*
     * Return if the graph is oriented or not, true if it is.
     */
    bool isOrientedGraph() const;

    /*
     * Get the cuantity of nodes are now in the graph.
     */
    uint getNodesCount() const;

    /*
     * Get the cuantity of edges are now inserted in the graph.
     */
    uint getEdgesCount() const;

    /*
     * Return all edges of the graph, in a vector.
     */
    std::list<Graph::Edge>& getEdges();

    /*
     * Add a new vertex, with out adjacents.
     * Increases n by one.
     */
    void addVertex();

    /*
     * Return the weight of the edge between nodes v1 and v2. If the graph is non-oriented, the orden of the parameters
     * doesn't matter, but it's important in a oriented graph, v1 indicates the origin of the edge, and v2 indicates the end.
     */
    float getEdgeWeight(uint v1, uint v2) const;

    /*
     * Return an iterator of the adjacents nodes for a single node in the graph.
     */
    const AdjacentsIterator adjacentsOf(uint v) const;

    /*
     * Test if a node is isolated or not
     */
    bool isIsolatedNode(uint v) const;

    /*
     * Paint a node to remark it on the graphic output
     */
    void paintNode(uint v);

    /*
     * Unpaint a painted node
     */
    void unpaintNode(uint v);

    /*
     * Paint an edge to remark it on the graphic output
     */
    void paintEdge(Edge* v);

    /*
     * Unpaint a painted edge
     */
    void unpaintEdge(Edge* v);

    /*
     * Get the .DOT format representation of the graph.
     * Useful to pipe it to a .DOT graphical tool.
     */
    std::string getDOT(bool weighted) const;

    class AdjacentsIterator {

        public:
            AdjacentsIterator(const std::vector<float>& conections, uint n, IMPL type);
            AdjacentsIterator(const std::list<std::pair<uint,float> >& adjacents, uint n, IMPL type);

            /*
             * Get actual adjacent of the iterator. This not modify the iterator status.
             */
            std::pair<uint,float> next() const;

            /*
             * Indicates if exist an adjacent node for review
             */
            bool thereIsMore() const;

            /*
             * Moves the iterator to the next adjacent. This change the output of next() method in the next call to it.
             */
            void advance();

        private:
            uint _vSpace;
            IMPL _impl;
            std::vector<float> _values;
            std::list<std::pair<uint,float> > _adjacents;
            std::vector<float>::const_iterator _it;
            std::list<std::pair<uint,float> >::const_iterator _iter;
            int _current;
            bool firstCall;
    };

private:
    std::vector<std::vector<float> > matrix;
    std::vector<std::list<std::pair<uint,float> > > adjList;
    std::vector<Graph::Node> nodes;
    bool isOriented;
    IMPL type;
    std::list<Graph::Edge> edges;

};

#endif

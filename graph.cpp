#include "graph.h"
#include <sstream>
#include <fstream>
#include <utility>
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

const float DEFAULT_WEIGHT = numeric_limits<float>::max();

Graph::Graph(uint nodesSpaceSize, bool oriented, IMPL impl) {
    isOriented = oriented;
    type = impl;
    nodes = vector<Node>(nodesSpaceSize, Graph::Node());

    if(type == ADJACENCIES_MATRIX)
        matrix = vector<vector<float> >(nodesSpaceSize, vector<float>(nodesSpaceSize, DEFAULT_WEIGHT));
    else
        adjList = vector<list<pair<uint,float> > >(nodesSpaceSize, list<pair<uint,float> >());
}

bool Graph::isOrientedGraph() const {
    return isOriented;
}

uint Graph::getNodesCount() const {
    return (uint) nodes.size();
}

uint Graph::getEdgesCount() const {
    return edges.size();
}

list<Graph::Edge>& Graph::getEdges() {
    return edges;
}


bool Graph::isIsolatedNode(uint v) const {
    if(v >= nodes.size())
        throw invalid_argument("Invalid argument on isIsolatedNode member function, v isn't a node of the graph.");

    return nodes[v].isolated;
}

float Graph::getEdgeWeight(uint v1, uint v2) const {
    if(v1 >= nodes.size() || v2 >= nodes.size()) 
        throw invalid_argument("Invalid arguments on getEdgeWeight member function, v1 or v2 aren't nodes of the graph.");

    if(type == ADJACENCIES_MATRIX)
        return matrix[v1][v2];
    else
        for (const auto& edge : adjList[v1]) 
            if (edge.first == v2) 
                return edge.second;

    return -1;
}

bool Graph::areAdjacent(uint v1, uint v2) const {
    if(v1 >= nodes.size() || v2 >= nodes.size()) 
        throw invalid_argument("Invalid arguments on areAdjacent member function, v1 or v2 aren't nodes of the graph.");

    if(type == ADJACENCIES_MATRIX)
        return matrix[v1][v2] != DEFAULT_WEIGHT;
    else {
        for (const auto& edge : adjList[v1])
            if (edge.first == v2)
                return true;

        return false;
    }
}

void Graph::applyEdge(uint v1, uint v2, float weight) {
    if(v1 >= nodes.size() || v2 >= nodes.size()) 
        throw invalid_argument("Invalid arguments on applyEdge member function, v1 or v2 aren't nodes of the graph.");

    if(type == ADJACENCIES_MATRIX) {
        if(matrix[v1][v2] == DEFAULT_WEIGHT)
            edges.push_back(Edge(v1,v2,weight));

        matrix[v1][v2] = weight;

        if(nodes[v1]._starting_neighbor_it == -1 || ((uint) nodes[v1]._starting_neighbor_it) > v2)
            nodes[v1]._starting_neighbor_it = v2;

        if(!isOriented) {
            matrix[v2][v1] = weight;

            if(nodes[v2]._starting_neighbor_it == -1 || ((uint) nodes[v2]._starting_neighbor_it) > v1)
                nodes[v2]._starting_neighbor_it = v1;
        }

    } else {
        adjList[v1].push_back(make_pair(v2, weight));

        if(!isOriented)
            adjList[v2].push_back(make_pair(v1, weight));

        edges.push_back(Edge(v1,v2,weight));
    }


    //Update nodes status, input degree, output degree and isolated state
    nodes[v1].isolated = false;
    nodes[v2].isolated = false;

    nodes[v1].dOut++;
    nodes[v2].dIn++;
    
    if(!isOriented) {
        nodes[v2].dOut++;
        nodes[v1].dIn++;
    }
}

uint Graph::addVertex() {
    nodes.push_back(Graph::Node());

    if(type == ADJACENCIES_MATRIX) {
        for (uint i = 0; i < nodes.size() - 1; ++i)
            matrix[i].push_back(DEFAULT_WEIGHT);

        matrix.push_back(vector<float>(nodes.size(), DEFAULT_WEIGHT));

    } else {
        adjList.push_back({});
    }

    return (uint) nodes.size();
}

void Graph::fill() {
    for (uint node = 0; node < nodes.size() ; node++) {
        if(isIsolatedNode(node)){
            for (uint i = 0; i < nodes.size(); ++i) {
                if(node == i) continue;

                applyEdge(node, i);
                if(isOriented)
                    applyEdge(i, node);
            }

        } else {
            for (uint i = 0; i < nodes.size(); ++i) {
                if(node == i) continue;

                if(!areAdjacent(node, i))
                    applyEdge(node, i);

                if(isOriented && !areAdjacent(i, node))
                    applyEdge(i, node);
            }
        }
    }
}

void Graph::paintNode(uint v) {
    if(v >= nodes.size()) 
        throw invalid_argument("Invalid argument on paintNode member function, v is not a node of the graph.");

    nodes[v].painted = true;
}

bool Graph::paintedNode(uint v) {
    if(v >= nodes.size()) 
        throw invalid_argument("Invalid argument on paintedNode member function, v is not a node of the graph.");

    return nodes[v].painted;
}

void Graph::unpaintNode(uint v) {
    if(v >= nodes.size()) 
        throw invalid_argument("Invalid argument on unpaintNode member function, v is not a node of the graph.");
    
    nodes[v].painted = false;    
}

void Graph::paintEdge(Edge* e) {
    if(e == NULL) 
        throw invalid_argument("Invalid argument on paintEdge member function, NULL pointer passed by parameter.");
    
    e->painted = true;
}

bool Graph::paintedEdge(Edge* e) {
    if(e == NULL) 
        throw invalid_argument("Invalid argument on paintedEdge member function, NULL pointer passed by parameter.");
    
    return e->painted;
}

void Graph::unpaintEdge(Edge* e) {
    if(e == NULL) 
        throw invalid_argument("Invalid argument on unpaintEdge member function, NULL pointer passed by parameter.");
    
    e->painted = false;    
}

string Graph::getDOT(bool weighted) const {
    char tag[60];

    stringstream dot;
    string rel_type = isOriented ? "->" : "--";

    if(isOriented)  dot << "digraph";
    else            dot << "graph";
    
    dot << "{";

    dot << "node[shape=circle,width=0.5,height=0.5,fixedsize=true]";

    for (auto edge = edges.begin(); edge != edges.end(); ++edge) {
        auto from = edge->from;
        auto to = edge->to;
        auto weight = edge->weight;
        auto paintedEdge = edge->painted;

        if(weighted)
            sprintf(tag, (paintedEdge ? "[color=red,penwidth=1.0,label=\"%.2f\"]" : "[label=\"%.2f\"]"), weight);
        else
            sprintf(tag, (paintedEdge ? "[color=red,penwidth=1.0]" : ""));

        dot << to_string(from);
        dot << rel_type;
        dot << to_string(to);
        dot << tag;
        dot << ";";

        if(nodes[from].painted) {
            dot << to_string(from);
            dot << "[style=filled,fillcolor=red]";
        }

        if(nodes[to].painted) {
            dot << to_string(to);
            dot << "[style=filled,fillcolor=red]";
        }
    }

    for (uint i = 0; i < nodes.size(); i++) {
        auto node = nodes[i];
        if(node.isolated) {
            dot << to_string(i);
            dot << rel_type;
            dot << to_string(i);
            dot << "[style=invis];";

            if(node.painted) {
                dot << to_string(i);
                dot << "[style=filled, fillcolor=red]";
            }
        }
    }

    dot << "}";

    return dot.str();
}

void Graph::exportDOT(const char* file_name, bool force_override) const {
    string extension(".dot");
    string str_fname(file_name);
    
    // Formatting the final name of the file, with the extension appended 
    // (if it is not already included on the file_name)
    string fn(str_fname);
    if(str_fname.length() < 5 || str_fname.substr(str_fname.length()-4,4) != extension) {
        fn += extension;
    }

    //Checking if file exists and stop is force_override==false
    ifstream ifile(fn);    
    if(!force_override && ifile) {
        cout << "[Error] File " + fn + " already exists.\n"
             << "Use .exportDOT(..., force_override=true) to force writing."
             << endl;
        return;
    }
 
    //Proceeding to write into the file
    try {
        ofstream file;
        file.open(fn);
        file << getDOT();
        file.close();
    } catch (const std::exception &exc) {
        cerr << exc.what();
    }

}

const typename Graph::AdjacentsIterator Graph::adjacentsOf(uint v) const {
    if(v >= nodes.size()) 
        throw invalid_argument("Invalid argument on adjacentsOf member function, v is not a node of the graph.");

    if(type == ADJACENCIES_MATRIX)
        return AdjacentsIterator(nodes[v], matrix[v], nodes.size(), type);
    else
        return AdjacentsIterator(nodes[v], adjList[v], nodes.size(), type);
}

// const typename Graph::DFSIterator Graph::DFS(uint v) const {
//     if(v >= nodes.size()) 
//         throw invalid_argument("Invalid argument on DFS member function, v is not a node of the graph.");

//     return DFSIterator(v, this);
// }



/*
 * Adjacents iterator implementation.
 */

Graph::AdjacentsIterator::AdjacentsIterator(Graph::Node me, const vector<float>& relations, uint n, IMPL impl) {
    _values = relations;
    _it = _values.begin();
    _vSpace = n;
    _impl = impl;

    _current = me._starting_neighbor_it;
}

Graph::AdjacentsIterator::AdjacentsIterator(Graph::Node me, const list<pair<uint,float> >& adjacents, uint n, IMPL impl) {
    _adjacents = adjacents;
    _iter = _adjacents.begin();

    _vSpace = n;
    _impl = impl;
}

void Graph::AdjacentsIterator::advance() {
    if(_impl == ADJACENCIES_MATRIX) {

        while((++_current) < (long int)_vSpace)
            if(_values[_current] != DEFAULT_WEIGHT)
                break;

    } else {
        _iter++;
    }

    return;
}

pair<uint,float> Graph::AdjacentsIterator::next() const {
    if(_impl == ADJACENCIES_MATRIX)
        return make_pair(_current, _values[_current]);
    else
        return (*_iter);
}

bool Graph::AdjacentsIterator::thereIsMore() const {
    if(_impl == ADJACENCIES_MATRIX)
        return -1 < _current && _current < ((long int)_vSpace);
    else
        return _iter != _adjacents.end();
}



/*
 * DFS iterator implementation.
 */

Graph::DFSIterator::DFSIterator(uint source, Graph* g) {
    _g = g;
    _source = source;
    _visited_nodes = vector<bool>((*g).getNodesCount(), false);
    
    _q.push(source);
}

void Graph::DFSIterator::advance() {
    do {
        uint last = _q.top();
        _q.pop();

        if(!_visited_nodes[last]) {
            _visited_nodes[last] = true;

            for(auto it = (*_g).adjacentsOf(last); it.thereIsMore(); it.advance())
                if(!_visited_nodes[it.next().first]) 
                    _q.push(it.next().first);
             
        }
        
    } while(!_q.empty() && _visited_nodes[_q.top()]);
    
    return;
}

uint Graph::DFSIterator::next() const {
    return _q.top();
}

bool Graph::DFSIterator::thereIsMore() const {
    return !_q.empty();
}

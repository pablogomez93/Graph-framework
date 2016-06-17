#include "graph.h"

Graph::Graph(uint nodesSpaceSize, bool oriented, IMPL impl) {
    isOriented = oriented;
    type = impl;
    nodes = vector<Node>(nodesSpaceSize, Graph::Node());

    if(type == ADJACENCIES_MATRIX)
        matrix = vector<vector<float> > (nodesSpaceSize, vector<float>(nodesSpaceSize, DEFAULT_WEIGHT));
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
    return nodes[v].isolated;
}

float Graph::getEdgeWeight(uint v1, uint v2) const {
    if(type == ADJACENCIES_MATRIX)
        return matrix[v1][v2];
    else
        for (const auto& edge : adjList[v1]) 
            if (edge.first == v2) 
                return edge.second;

    return -1;
}

bool Graph::areAdjacent(uint v1, uint v2) const {
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
    if(type == ADJACENCIES_MATRIX) {
        if(matrix[v1][v2] == DEFAULT_WEIGHT)
            edges.push_back(Edge(v1,v2,weight));

        matrix[v1][v2] = weight;

        if(!isOriented)
            matrix[v2][v1] = weight;
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

void Graph::addVertex() {
    nodes.push_back(Graph::Node());

    if(type == ADJACENCIES_MATRIX) {
        for (uint i = 0; i < nodes.size() - 1; ++i)
            matrix[i].push_back(DEFAULT_WEIGHT);

        matrix.push_back(vector<float>(nodes.size(), DEFAULT_WEIGHT));

    } else {
        adjList.push_back({});
    }

}

void Graph::paintNode(uint v) {
    nodes[v].painted = true;
}

void Graph::unpaintNode(uint v) {
    nodes[v].painted = false;    
}

void Graph::paintEdge(Edge* e) {
    e->painted = true;
}

void Graph::unpaintEdge(Edge* e) {
    e->painted = false;    
}

string Graph::getDOT(bool weighted) const {
    char tag[60];

    stringstream dot;
    string rel_type = isOriented ? "->" : "--";

    if(isOriented)  dot << "digraph";
    else            dot << "graph";
    
    dot << "{";

    dot << "node [ shape=circle,width=0.5,height = 0.5,fixedsize=true]";

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
            dot << "[style=filled, fillcolor=red]";
        }

        if(nodes[to].painted) {
            dot << to_string(to);
            dot << "[style=filled, fillcolor=red]";
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

const typename Graph::AdjacentsIterator Graph::adjacentsOf(uint v) const {
    if(type == ADJACENCIES_MATRIX)
        return AdjacentsIterator(matrix[v], nodes.size(), type);
    else
        return AdjacentsIterator(adjList[v], nodes.size(), type);
}



/*
 * Adjacents iterator implementation.
 */

Graph::AdjacentsIterator::AdjacentsIterator(const vector<float>& relations, uint n, IMPL impl) {
    _values = relations;

    _it = _values.begin();
    _current = -1;

    for (uint current = 0; current < _vSpace; ++current) {
        if(_values[current] != DEFAULT_WEIGHT) {
            _current = current;
            break;
        }
    }

    _vSpace = n;
    _impl = impl;
}

Graph::AdjacentsIterator::AdjacentsIterator(const list<pair<uint,float> >& adjacents, uint n, IMPL impl) {
    _adjacents = adjacents;
    _iter = _adjacents.begin();

    _vSpace = n;
    _impl = impl;
}

void Graph::AdjacentsIterator::advance() {
    if(_impl == ADJACENCIES_MATRIX) {
        _current++;
        while(_current < (long int)_vSpace) {
            if(_values[_current] != DEFAULT_WEIGHT)
                break;

            _current++;
        }

    } else
        _iter++;

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

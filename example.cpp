#include <stdio.h>
#include <limits>
#include <iostream>
#include "graph.h"

using namespace std;

int main(){
	srand(time(NULL));

	//Set configuration of the graph
	uint n = 200;
	bool isOriented = true;

	//Create a graph with n nodes, oriented, based on ADJACENCIES_LIST.
	Graph g(n, isOriented, ADJACENCIES_LIST);

	//Fill with 1% of the edges, with random weight in each. 
	for (int i = 0; i < ((n*(n-1))/100); ++i) {
		uint from = rand()%n;
		uint to = rand()%n;

		if(!g.areAdjacent(from, to)) {
			g.applyEdge(from, to);

			g.paintNode(from);
			g.paintNode(to);
		} else {
			i--;
		}
	}

	//Print the graph in the standard .DOT format to represent graphs
	cout << g.getDOT(false);

	return 0;
}


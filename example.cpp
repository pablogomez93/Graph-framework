#include <iostream>
#include "graph.h"

using namespace std;

typedef unsigned int uint;

/*
 * An example of how to use the graph framework.
 *
 * This will build an oriented graph, based on adjacencies list.
 * Then will fill it with 1% of the edges, painting all the nodes what are not
 * isolated and then send it by standard output in the standard .DOT format.
 * Type 'make all & make run' in a terminal to run it.
 *
 * If you prefer, you can plot the graph, redirecting the output to a graphical
 * tool for drawing graphs, like graphviz to see the graph visually, for example:
 * 		make all && make run | dot -Tps -o graph.ps
 */
int main(){
	srand(time(NULL));

	//Configuration of the graph
	uint n = 50;
	bool isOriented = true;

	//Create a graph with n nodes, oriented, based on ADJACENCIES_LIST.
	Graph g(n, ADJACENCIES_LIST, isOriented);

	//Fill with 1% of the edges, with random weight in each. 
	for (uint i = 0; i < ((n*(n-1))/100); ++i) {
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
	cout << g.getDOT(false) << endl;

	return 0;
}


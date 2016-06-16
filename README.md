Graph Framework
===============

A framework to create and control graphs in C++, with the possibility of choosing one of the two most common graph implementations, which are based on *adjacencies list* or *adjacencies matrix*. Also oriented/non-oriented mode is eligible.

## Usage
A real example of usage can be found on my [Dijkstra's Algorithm implementation](https://github.com/pablogomez93/Dijkstra). Also, there is a example.cpp file in this repository with a little example of usage, to watch it compile and run just use make all && make run.<br/>

To create a new Graph just use one of the following constructors:
```c++
//Creates an empty graph, non-oriented and based on adjacencies matrix
Graph g();
```
```c++
//Identical previous but initialized with nodes_count nodes
Graph g(unsigned int nodes_count);
```
```c++
//Identical previous but implementation specified, it can be ADJACENCIES_MATRIX or ADJACENCIES_LIST
Graph g(unsigned int nodes_count, Graph::IMPL implementation);
```
```c++
//Identical previous but orientation prop specified
Graph g(unsigned int nodes_count, Graph::IMPL implementation, bool isOriented);
```
Then you are ready to use one of the available methods ([Full documentation here](#tests-and-stress-tests)), for example:
```c++
#include "graph.h"
//...
unsigned int nodes = 10;
for (unsigned int to = current+1; to < nodes; to++) {
    g.applyEdge(0, current);
    g.applyEdge(current, nodes-1);
    
    g.paintNode(rand()%nodes); //Paint random nodes
}
cout << g.getDOT(false);
```
Then, executing the previous code and dumping it, will give us a visual image of the graph:

    ./example | dot -Tps -o graph.ps
![](graph.png)
## Tests and Stress tests

There are also included a test suite which run some test to check the correcteness of the graph actions.<br/>
To run the included test you just must execute:

    make test

There is an stress_test too to put the graph under presion and check if it supports the charge.<br/>
If you wanna run it, you must be careful because it may put slow your computer for a while. To run it just do:

    make stress_test

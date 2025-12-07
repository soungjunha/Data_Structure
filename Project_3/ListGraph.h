#ifndef _LIST_H_
#define _LIST_H_

#include "Graph.h"

// Graph implementation using adjacency list representation
// Each vertex has a map of connected vertices and their edge weights
class ListGraph : public Graph {	
private:
	map<int, int>* m_list;     // Array of maps: each index represents a vertex, map contains (neighbor, weight) pairs
	vector<int>* kw_graph;     // Auxiliary data structure for additional graph operations
	
public:	
	// Constructor: creates a list graph with specified type and number of vertices
	ListGraph(bool type, int size);
	
	// Destructor: deallocates memory for adjacency list
	~ListGraph();
	
	// Gets adjacent edges treating graph as undirected
	// Includes both outgoing edges (from vertex) and incoming edges (to vertex)
	void getAdjacentEdges(int vertex, map<int, int>* m);	
	
	// Gets adjacent edges treating graph as directed
	// Only includes outgoing edges from the vertex
	void getAdjacentEdgesDirect(int vertex, map<int, int>* m);
	
	// Inserts a directed edge from 'from' to 'to' with specified weight
	void insertEdge(int from, int to, int weight);	
	
	// Prints the adjacency list representation to output file
	bool printGraph(ofstream *fout);
};

#endif
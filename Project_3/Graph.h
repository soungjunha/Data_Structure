#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <iostream>
#include <cstring>
#include <string.h>
#include <fstream>
#include <map>
#include <set>
#include <math.h>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>
#include <stack>

using namespace std;

// Base class for graph data structures
// Supports both adjacency list and adjacency matrix representations
class Graph {	
protected:
	bool m_type;  // Graph storage type: true for Matrix, false for List
	int m_size;   // Number of vertices in the graph

public:
	// Constructor: initializes graph with storage type and number of vertices
	Graph(bool type, int size);
	
	// Virtual destructor to ensure proper cleanup of derived classes
	virtual ~Graph();

	// Returns the graph storage type
	bool getType();	
	
	// Returns the number of vertices in the graph
	int getSize();

	// Gets adjacent edges treating graph as undirected (includes both outgoing and incoming edges)
	// vertex: source vertex to get edges from
	// m: output map containing neighboring vertices and their edge weights
	virtual void getAdjacentEdges(int vertex, map<int, int>* m) = 0;		
	
	// Gets adjacent edges treating graph as directed (only outgoing edges)
	// vertex: source vertex to get edges from
	// m: output map containing neighboring vertices and their edge weights
	virtual void getAdjacentEdgesDirect(int vertex, map<int, int>* m) = 0;	
	
	// Inserts a directed edge into the graph
	// from: source vertex
	// to: destination vertex
	// weight: edge weight
	virtual void insertEdge(int from, int to, int weight) = 0;				
	
	// Prints the graph structure to output file
	// fout: output file stream
	// Returns true if successful, false otherwise
	virtual	bool printGraph(ofstream *fout) = 0;
};

#endif
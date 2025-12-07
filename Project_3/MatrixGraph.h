#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <map>
#include "Graph.h"

// Graph implementation using adjacency matrix representation
// Uses a 2D array where matrix[i][j] represents the weight of edge from vertex i to vertex j
class MatrixGraph : public Graph {	
private:
	int** m_mat;  // 2D array representing adjacency matrix (0 means no edge)

public:
	// Constructor: creates a matrix graph with specified type and number of vertices
	MatrixGraph(bool type, int size);
	
	// Destructor: deallocates memory for adjacency matrix
	~MatrixGraph();
	
	// Gets adjacent edges treating graph as undirected
	// Includes both outgoing edges (row) and incoming edges (column)
	void getAdjacentEdges(int vertex, map<int, int>* m);	
	
	// Gets adjacent edges treating graph as directed
	// Only includes outgoing edges (row entries)
	void getAdjacentEdgesDirect(int vertex, map<int, int>* m);
	
	// Inserts a directed edge from 'from' to 'to' with specified weight
	void insertEdge(int from, int to, int weight);	
	
	// Prints the adjacency matrix representation to output file
	bool printGraph(ofstream *fout);
};

#endif
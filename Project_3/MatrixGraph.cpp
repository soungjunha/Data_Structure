#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Constructor
// Allocates memory for the adjacency matrix (2D array) and initializes it to 0
MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_mat = new int*[size];
	for (int i = 0; i < size; i++) {
		m_mat[i] = new int[size];
		memset(m_mat[i], 0, sizeof(int) * size);
	}
}

// Destructor
// Deallocates the memory used by the adjacency matrix
MatrixGraph::~MatrixGraph()
{
	for (int i = 0; i < m_size; i++) {
		delete[] m_mat[i];
	}
	delete[] m_mat;
}

// Get adjacent edges for Undirected Graph concept
// Includes both outgoing edges (row) and incoming edges (column)
void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	// Step 1: Check outgoing edges (row 'vertex')
	// matrix[vertex][i] != 0 means there's an edge from vertex to i
	for (int i = 0; i < m_size; i++) {
		if (m_mat[vertex][i] != 0) {
			m->insert(make_pair(i, m_mat[vertex][i]));
		}
	}

	// Step 2: Check incoming edges (column 'vertex')
	// matrix[i][vertex] != 0 means there's an edge from i to vertex
	for (int i = 0; i < m_size; i++) {
		if (m_mat[i][vertex] != 0) {
			m->insert(make_pair(i, m_mat[i][vertex]));
		}
	}
}

// Get adjacent edges for Directed Graph
// Only includes outgoing edges from the vertex (row entries only)
void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	for (int i = 0; i < m_size; i++) {
		if (m_mat[vertex][i] != 0) {
			m->insert(make_pair(i, m_mat[vertex][i]));
		}
	}
}

// Insert an edge into the graph
// Sets the matrix entry at [from][to] to the specified weight
void MatrixGraph::insertEdge(int from, int to, int weight)
{
	if (from >= 0 && from < m_size && to >= 0 && to < m_size) {
		m_mat[from][to] = weight;
	}
}

// Print the graph structure to the output file
// Outputs the adjacency matrix with row and column headers
bool MatrixGraph::printGraph(ofstream *fout)
{
	if (!fout || !fout->is_open()) return false;

	*fout << "========PRINT========" << endl;

	// Print column headers
	*fout << "\t";
	for (int i = 0; i < m_size; i++) {
		*fout << "[" << i << "]" << "\t";
	}
	*fout << endl;

	// Print each row with row header
	for (int i = 0; i < m_size; i++) {
		*fout << "[" << i << "]" << "\t";
		for (int j = 0; j < m_size; j++) {
			*fout << m_mat[i][j] << "\t";
		}
		*fout << endl;
	}

	*fout << "=======================" << endl << endl;
	return true;
}
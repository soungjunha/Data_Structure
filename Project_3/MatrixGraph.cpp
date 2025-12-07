#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Constructor
// Allocates memory for the adjacency matrix (2D array) and initializes it to 0
MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int*[size];
	for (int i = 0; i < size; i++) {
		m_Mat[i] = new int[size];
		memset(m_Mat[i], 0, sizeof(int) * size);
	}
}

// Destructor
// Deallocates the memory used by the adjacency matrix
MatrixGraph::~MatrixGraph()
{
	for (int i = 0; i < m_Size; i++) {
		delete[] m_Mat[i];
	}
	delete[] m_Mat;
}

// Get adjacent edges for Undirected Graph concept
void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	// 1. Check outgoing edges (row 'vertex')
	for (int i = 0; i < m_Size; i++) {
		if (m_Mat[vertex][i] != 0) {
			m->insert(make_pair(i, m_Mat[vertex][i]));
		}
	}

	// 2. Check incoming edges (column 'vertex')
	for (int i = 0; i < m_Size; i++) {
		if (m_Mat[i][vertex] != 0) {
			m->insert(make_pair(i, m_Mat[i][vertex]));
		}
	}
}

// Get adjacent edges for Directed Graph
void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	for (int i = 0; i < m_Size; i++) {
		if (m_Mat[vertex][i] != 0) {
			m->insert(make_pair(i, m_Mat[vertex][i]));
		}
	}
}

// Insert an edge into the graph
void MatrixGraph::insertEdge(int from, int to, int weight)
{
	if (from >= 0 && from < m_Size && to >= 0 && to < m_Size) {
		m_Mat[from][to] = weight;
	}
}

// Print the graph structure to the output file
bool MatrixGraph::printGraph(ofstream *fout)
{
	if (!fout || !fout->is_open()) return false;

	*fout << "========PRINT========" << endl;

	// Print Column Headers
	*fout << "\t";
	for (int i = 0; i < m_Size; i++) {
		*fout << "[" << i << "]" << "\t";
	}
	*fout << endl;

	// Print Rows
	for (int i = 0; i < m_Size; i++) {
		*fout << "[" << i << "]" << "\t";
		for (int j = 0; j < m_Size; j++) {
			*fout << m_Mat[i][j] << "\t";
		}
		*fout << endl;
	}

	*fout << "=======================" << endl << endl;
	return true;
}
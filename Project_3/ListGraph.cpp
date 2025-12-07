#include "ListGraph.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Constructor
// Allocates memory for the adjacency list (array of maps)
ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_list = new map<int, int>[size];
	kw_graph = new vector<int>[size];
}

// Destructor
// Deallocates the memory used by the adjacency list
ListGraph::~ListGraph()
{
	delete[] m_list;
	delete[] kw_graph;
}

// Get adjacent edges for Undirected Graph
// Adds both outgoing and incoming edges to the map 'm'
void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	// Step 1: Add outgoing edges (from vertex -> to neighbor)
	for (auto it = m_list[vertex].begin(); it != m_list[vertex].end(); it++) {
		m->insert(make_pair(it->first, it->second));
	}

	// Step 2: Add incoming edges (from neighbor -> to vertex)
	// Scan all other vertices to find edges pointing to current vertex
	for (int i = 0; i < m_size; i++) {
		if (i == vertex) continue;

		auto it = m_list[i].find(vertex);
		if (it != m_list[i].end()) {
			m->insert(make_pair(i, it->second));
		}
	}
}

// Get adjacent edges for Directed Graph
// Only adds outgoing edges from the vertex to the map 'm'
void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	for (auto it = m_list[vertex].begin(); it != m_list[vertex].end(); it++) {
		m->insert(make_pair(it->first, it->second));
	}
}

// Insert an edge into the graph
// Creates a directed edge from 'from' vertex to 'to' vertex with given weight
void ListGraph::insertEdge(int from, int to, int weight)
{
	m_list[from].insert(make_pair(to, weight));
}

// Print the graph structure to the output file
// Outputs adjacency list format: [vertex] -> (neighbor1, weight1) -> (neighbor2, weight2) ...
bool ListGraph::printGraph(ofstream *fout)
{
	if (!fout || !fout->is_open()) return false;

	*fout << "========PRINT========" << endl;

	// Print each vertex and its adjacent edges
	for (int i = 0; i < m_size; i++) {
		*fout << "[" << i << "]";
		
		bool first = true;
		for (auto it = m_list[i].begin(); it != m_list[i].end(); it++) {
			if (first) {
				*fout << " -> (" << it->first << "," << it->second << ")";
				first = false;
			} else {
				*fout << " -> (" << it->first << "," << it->second << ")";
			}
		}
		*fout << endl;
	}

	*fout << "=======================" << endl << endl;
	return true;
}
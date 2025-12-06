#include "ListGraph.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Constructor
// Allocates memory for the adjacency list (array of maps)
ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_List = new map<int, int>[size];
	kw_graph = new vector<int>[size]; // Allocate memory for kruskal generic vector if needed
}

// Destructor
// Deallocates the memory used by the adjacency list
ListGraph::~ListGraph()
{
	delete[] m_List;
	delete[] kw_graph;
}

// Get adjacent edges for Undirected Graph
// Adds both outgoing and incoming edges to the map 'm'
// If an edge u->v exists, v is added to u's adjacency list.
// If v->u exists, v is also added to u's adjacency list (as if undirected).
void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m)
{
	// 1. Add outgoing edges (from vertex -> to neighbor)
	for (auto it = m_List[vertex].begin(); it != m_List[vertex].end(); it++) {
		m->insert(make_pair(it->first, it->second));
	}

	// 2. Add incoming edges (from neighbor -> to vertex)
	// Iterate through all vertices to find edges pointing to 'vertex'
	for (int i = 0; i < m_Size; i++) {
		if (i == vertex) continue;

		// Check if vertex 'i' has an edge to 'vertex'
		auto it = m_List[i].find(vertex);
		if (it != m_List[i].end()) {
			// Insert edge (neighbor, weight) into m
			// Use insert to avoid overwriting if a directed edge already exists (keep the outgoing one)
			m->insert(make_pair(i, it->second));
		}
	}
}

// Get adjacent edges for Directed Graph
// Only adds outgoing edges from the vertex to the map 'm'
void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	// Iterate through the map of the specific vertex and insert into 'm'
	for (auto it = m_List[vertex].begin(); it != m_List[vertex].end(); it++) {
		m->insert(make_pair(it->first, it->second));
	}
}

// Insert an edge into the graph
// Adds a directed edge from 'from' to 'to' with 'weight'
void ListGraph::insertEdge(int from, int to, int weight)
{
	// Map automatically handles sorting by key (destination vertex)
	m_List[from].insert(make_pair(to, weight));
}

// Print the graph structure to the output file
// Format: [Source] -> (Dest,Weight) (Dest,Weight) ...
bool ListGraph::printGraph(ofstream *fout)
{
	if (!fout || !fout->is_open()) return false;

	*fout << "========PRINT========" << endl;

	for (int i = 0; i < m_Size; i++) {
		*fout << "[" << i << "] ->";
		
		// Iterate through neighbors
		for (auto it = m_List[i].begin(); it != m_List[i].end(); it++) {
			*fout << " (" << it->first << "," << it->second << ")";
		}
		*fout << endl;
	}

	*fout << "=====================" << endl << endl;
	return true;
}
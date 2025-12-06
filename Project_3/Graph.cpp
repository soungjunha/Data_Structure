#include "Graph.h"

// Constructor
// Initializes the graph type and size (number of vertices)
Graph::Graph(bool type, int size)
{
	this->m_Type = type;
	this->m_Size = size;
}

// Destructor
// Virtual destructor to ensure proper cleanup of derived classes
Graph::~Graph()
{
}

// Getter for graph type
// Returns true for MatrixGraph, false for ListGraph (depending on implementation in Manager)
bool Graph::getType()
{
	return this->m_Type;
}

// Getter for graph size
// Returns the number of vertices in the graph
int Graph::getSize()
{
	return this->m_Size;
}
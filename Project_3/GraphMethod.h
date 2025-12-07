#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"

// Breadth-First Search traversal
// graph: graph to traverse
// option: 'O' for directed, 'X' for undirected
// vertex: starting vertex
// Returns true if successful, false if error
bool BFS(Graph* graph, char option, int vertex);

// Depth-First Search traversal
// graph: graph to traverse
// option: 'O' for directed, 'X' for undirected
// vertex: starting vertex
// Returns true if successful, false if error
bool DFS(Graph* graph, char option, int vertex);

// Calculate closeness centrality for all vertices
// graph: graph to analyze (treated as undirected)
// Returns true if successful, false if error (negative cycle)
bool Centrality(Graph* graph);

// Kruskal's algorithm for Minimum Spanning Tree
// graph: graph to process (treated as undirected)
// Returns true if MST found, false if graph is disconnected or error
bool Kruskal(Graph* graph);

// Dijkstra's algorithm for single-source shortest paths
// graph: graph to process
// option: 'O' for directed, 'X' for undirected
// vertex: source vertex
// Returns true if successful, false if negative weights exist or error
bool Dijkstra(Graph* graph, char option, int vertex);

// Bellman-Ford algorithm for single-source shortest path
// graph: graph to process
// option: 'O' for directed, 'X' for undirected
// s_vertex: source vertex
// e_vertex: destination vertex
// Returns true if successful, false if negative cycle detected or error
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex);

// Floyd-Warshall algorithm for all-pairs shortest paths
// graph: graph to process
// option: 'O' for directed, 'X' for undirected
// Returns true if successful, false if negative cycle detected or error
bool FLOYD(Graph* graph, char option);

#endif
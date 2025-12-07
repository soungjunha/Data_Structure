#ifndef _GRAPHMETHOD_H_
#define _GRAPHMETHOD_H_

#include "ListGraph.h"
#include "MatrixGraph.h"

bool BFS(Graph* graph, char option, int vertex);     
bool DFS(Graph* graph, char option,  int vertex);     
bool Centrality(Graph* graph);  
bool Kruskal(Graph* graph);
bool Dijkstra(Graph* graph, char option, int vertex);
bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex);
bool FLOYD(Graph* graph, char option);

#endif
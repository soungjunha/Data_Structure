#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "GraphMethod.h"

// Manager class that coordinates all graph operations
// Reads commands from file and executes appropriate graph algorithms
class Manager {	
private:
	Graph* graph;     // Pointer to the current graph (ListGraph or MatrixGraph)
	ofstream fout;    // Output file stream for logging results
	int load;         // Flag indicating if a graph has been loaded (0 = not loaded, 1 = loaded)

public:
	// Constructor: initializes manager and clears log file
	Manager();	
	
	// Destructor: cleans up graph and closes file streams
	~Manager();	

	// Main execution loop: reads and processes commands from command file
	// command_txt: path to command file
	void run(const char* command_txt);
	
	// Loads graph data from file
	// filename: path to graph data file (graph_L.txt or graph_M.txt)
	// Returns true if successful, false if file doesn't exist or format error
	bool LOAD(const char* filename);	
	
	// Prints the current graph structure to log file
	// Returns true if successful, false if no graph loaded
	bool PRINT();	
	
	// Executes Breadth-First Search
	// option: 'O' for directed, 'X' for undirected
	// vertex: starting vertex
	// Returns true if successful, false if error
	bool mBFS(char option, int vertex);	
	
	// Executes Depth-First Search
	// option: 'O' for directed, 'X' for undirected
	// vertex: starting vertex
	// Returns true if successful, false if error
	bool mDFS(char option, int vertex);	
	
	// Executes Dijkstra's shortest path algorithm
	// option: 'O' for directed, 'X' for undirected
	// vertex: source vertex
	// Returns true if successful, false if negative weights or error
	bool mDIJKSTRA(char option, int vertex);	
	
	// Executes Kruskal's MST algorithm
	// Returns true if successful, false if graph disconnected or error
	bool mKRUSKAL();	
	
	// Executes Bellman-Ford shortest path algorithm
	// option: 'O' for directed, 'X' for undirected
	// s_vertex: source vertex
	// e_vertex: destination vertex
	// Returns true if successful, false if negative cycle or error
	bool mBELLMANFORD(char option, int s_vertex, int e_vertex);	
	
	// Executes Floyd-Warshall all-pairs shortest path algorithm
	// option: 'O' for directed, 'X' for undirected
	// Returns true if successful, false if negative cycle or error
	bool mFLOYD(char option); 
	
	// Calculates closeness centrality for all vertices
	// Returns true if successful, false if negative cycle or error
	bool mCentrality();
	
	// Prints error code to log file
	// n: error code number
	void printErrorCode(int n); 
};

#endif
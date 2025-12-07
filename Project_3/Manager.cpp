#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Constructor
// Initializes the manager with no graph loaded and clears the log file
Manager::Manager()	
{
	graph = nullptr;	
	// Clear log.txt at start (truncate mode)
	fout.open("log.txt", ios::trunc);
	fout.close();
	load = 0;  // No graph loaded initially
}

// Destructor
// Cleans up allocated graph memory and closes any open file streams
Manager::~Manager()
{
	if(load)
		delete graph;	
	if(fout.is_open())
		fout.close();
}

// Main execution loop
// Reads commands from command file and executes them sequentially
void Manager::run(const char* command_txt){
	ifstream fin;
	fin.open(command_txt, ios_base::in);
		
	if(!fin) {
		fout<<"command file open error"<<endl;
		return;
	}
	
	string line;
	while(getline(fin, line)){
		// Skip empty lines
		if(line.empty()) continue;
		
		stringstream ss(line);
		string cmd;
		ss >> cmd;

		// LOAD command: load graph from file
		if(cmd == "LOAD"){
			string filename, extra;
			// Check for correct number of arguments (exactly 1)
			if(ss >> filename && !(ss >> extra)){
				if(!LOAD(filename.c_str())){
					printErrorCode(100);
				}
			} else {
				printErrorCode(100);
			}
		}
		// PRINT command: print current graph structure
		else if(cmd == "PRINT"){
			string extra;
			// Check for no additional arguments
			if(!(ss >> extra)){
				if(!PRINT()){
					printErrorCode(200);
				}
			} else {
				printErrorCode(200);
			}
		}
		// BFS command: breadth-first search
		else if(cmd == "BFS"){
			char option;
			int vertex;
			string extra;
			// Check for correct arguments: option and vertex
			if(ss >> option >> vertex && !(ss >> extra)){
				if(option != 'O' && option != 'X'){
					printErrorCode(300);
				} else if(!mBFS(option, vertex)){
					printErrorCode(300);
				}
			} else {
				printErrorCode(300);
			}
		}
		// DFS command: depth-first search
		else if(cmd == "DFS"){
			char option;
			int vertex;
			string extra;
			// Check for correct arguments: option and vertex
			if(ss >> option >> vertex && !(ss >> extra)){
				if(option != 'O' && option != 'X'){
					printErrorCode(400);
				} else if(!mDFS(option, vertex)){
					printErrorCode(400);
				}
			} else {
				printErrorCode(400);
			}
		}
		// KRUSKAL command: minimum spanning tree
		else if(cmd == "KRUSKAL"){
			string extra;
			// Check for no additional arguments
			if(!(ss >> extra)){
				if(!mKRUSKAL()){
					printErrorCode(500);
				}
			} else {
				printErrorCode(500);
			}
		}
		// DIJKSTRA command: shortest path (no negative weights)
		else if(cmd == "DIJKSTRA"){
			char option;
			int vertex;
			string extra;
			// Check for correct arguments: option and vertex
			if(ss >> option >> vertex && !(ss >> extra)){
				if(option != 'O' && option != 'X'){
					printErrorCode(600);
				} else if(!mDIJKSTRA(option, vertex)){
					printErrorCode(600);
				}
			} else {
				printErrorCode(600);
			}
		}
		// BELLMANFORD command: shortest path (handles negative weights)
		else if(cmd == "BELLMANFORD"){
			char option;
			int s_vertex, e_vertex;
			string extra;
			// Check for correct arguments: option, source, destination
			if(ss >> option >> s_vertex >> e_vertex && !(ss >> extra)){
				if(option != 'O' && option != 'X'){
					printErrorCode(700);
				} else if(!mBELLMANFORD(option, s_vertex, e_vertex)){
					printErrorCode(700);
				}
			} else {
				printErrorCode(700);
			}
		}
		// FLOYD command: all-pairs shortest paths
		else if(cmd == "FLOYD"){
			char option;
			string extra;
			// Check for correct arguments: option only
			if(ss >> option && !(ss >> extra)){
				if(option != 'O' && option != 'X'){
					printErrorCode(800);
				} else if(!mFLOYD(option)){
					printErrorCode(800);
				}
			} else {
				printErrorCode(800);
			}
		}
		// CENTRALITY command: closeness centrality calculation
		else if(cmd == "CENTRALITY"){
			string extra;
			// Check for no additional arguments
			if(!(ss >> extra)){
				if(!mCentrality()){
					printErrorCode(900);
				}
			} else {
				printErrorCode(900);
			}
		}
		// EXIT command: clean up and terminate
		else if(cmd == "EXIT"){
			fout.open("log.txt", ios::app);
			fout << "========EXIT========" << endl;
			fout << "Success" << endl;
			fout << "====================" << endl << endl;
			fout.close();
			break;
		}
	}

	fin.close();
	return;
}

// LOAD function
// Reads graph data from file and constructs appropriate graph object
bool Manager::LOAD(const char* filename)
{
	ifstream file(filename);
	if(!file.is_open()) return false;

	// Reset existing graph if any
	if(load == 1){
		delete graph;
		graph = nullptr;
		load = 0;
	}

	char type;
	int size;
	
	string line;
	// Read graph type (L or M)
	if(!getline(file, line)) return false;
	type = line[0];
	// Read number of vertices
	if(!getline(file, line)) return false;
	size = stoi(line);

	// Validate size (minimum 5 vertices required)
	if(size < 5) return false;

	// Instantiate appropriate graph type
	if(type == 'L'){
		graph = new ListGraph(false, size);
	}
	else if(type == 'M'){
		graph = new MatrixGraph(true, size);
	}
	else {
		return false;
	}

	if(type == 'L'){
		// List Graph Parsing
		// Format: Each vertex has two lines:
		// Line 1: Source vertex number (single integer)
		// Line 2: Destination weight pairs (dest1 weight1 dest2 weight2 ...)
		//         Can be empty or just whitespace if no edges
		for(int v = 0; v < size; v++){
			// Read vertex line (source vertex number)
			if(!getline(file, line)) break;
			int src = stoi(line);
			
			// Read edges line (destination and weight pairs)
			if(!getline(file, line)) break;
			
			// Parse edges from the line
			stringstream ss(line);
			int dst, weight;
			while(ss >> dst >> weight){
				graph->insertEdge(src, dst, weight);
			}
		}
	}
	else if(type == 'M'){
		// Matrix Graph Parsing
		// Format: Each line represents one row of the adjacency matrix
		// 0 means no edge, non-zero value is edge weight
		int weight;
		for(int i = 0; i < size; i++){
			if(!getline(file, line)) break;
			stringstream ss(line);
			for(int j = 0; j < size; j++){
				if(ss >> weight){
					// Only insert edge if weight is non-zero
					if(weight != 0){
						graph->insertEdge(i, j, weight);
					}
				}
			}
		}
	}

	file.close();
	load = 1;
	
	// Write success message to log file
	fout.open("log.txt", ios::app);
	fout << "========LOAD========" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl << endl;
	fout.close();
	return true;
}

// PRINT function
// Outputs the current graph structure to log file
bool Manager::PRINT()
{
	if(graph == nullptr || load == 0) return false;
	fout.open("log.txt", ios::app);
	bool result = graph->printGraph(&fout);
	fout.close();
	return result;
}

// mBFS function
// Wrapper for BFS algorithm
bool Manager::mBFS(char option, int vertex)
{
	if(graph == nullptr || load == 0) return false;
	return BFS(graph, option, vertex);
}

// mDFS function
// Wrapper for DFS algorithm
bool Manager::mDFS(char option, int vertex)
{
	if(graph == nullptr || load == 0) return false;
	return DFS(graph, option, vertex);
}

// mDIJKSTRA function
// Wrapper for Dijkstra's algorithm
bool Manager::mDIJKSTRA(char option, int vertex)
{
	if(graph == nullptr || load == 0) return false;
	return Dijkstra(graph, option, vertex);
}

// mKRUSKAL function
// Wrapper for Kruskal's MST algorithm
bool Manager::mKRUSKAL()
{
	if(graph == nullptr || load == 0) return false;
	return Kruskal(graph);
}

// mBELLMANFORD function
// Wrapper for Bellman-Ford algorithm
bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if(graph == nullptr || load == 0) return false;
	return Bellmanford(graph, option, s_vertex, e_vertex);
}

// mFLOYD function
// Wrapper for Floyd-Warshall algorithm
bool Manager::mFLOYD(char option)
{
	if(graph == nullptr || load == 0) return false;
	return FLOYD(graph, option);
}

// mCentrality function
// Wrapper for Centrality calculation
bool Manager::mCentrality() {
	if(graph == nullptr || load == 0) return false;
	return Centrality(graph);
}

// printErrorCode function
// Outputs error code to log file in specified format
void Manager::printErrorCode(int n)
{
	fout.open("log.txt", ios::app);
	fout<<"========ERROR======="<<endl;
	fout<<n<<endl;
	fout<<"===================="<<endl << endl;
	fout.close();
}
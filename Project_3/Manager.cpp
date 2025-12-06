#include "Manager.h"
#include "GraphMethod.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

Manager::Manager()	
{
	graph = nullptr;	
	fout.open("log.txt", ios::app);
	load = 0;	//Anything is not loaded
}

Manager::~Manager()
{
	if(load)	//if graph is loaded, delete graph
		delete graph;	
	if(fout.is_open())	//if fout is opened, close file
		fout.close();	//close log.txt File
}

void Manager::run(const char* command_txt){
	ifstream fin;	//Command File Input File Stream
	fin.open(command_txt, ios_base::in);//File open with read mode
		
	if(!fin) { //If command File cannot be read, Print error
		fout<<"command file open error"<<endl;
		return;	//Return
	}
	string line;
	while(getline(fin, line)){
		stringstream ss(line);
		string cmd;
		ss >> cmd;

		if(cmd == "LOAD"){
			string filename, extra;
			if(ss >> filename && !(ss >> extra)){ // Check for exact argument count
				if(!LOAD(filename.c_str())){
					printErrorCode(100);
				}
			} else {
				printErrorCode(100);
			}
		}
		else if(cmd == "PRINT"){
			string extra;
			if(!(ss >> extra)){
				if(!PRINT()){
					printErrorCode(200);
				}
			} else {
				printErrorCode(200);
			}
		}
		else if(cmd == "BFS"){
			char option;
			int vertex;
			string extra;
			if(ss >> option >> vertex && !(ss >> extra)){
				if(!mBFS(option, vertex)){
					printErrorCode(300);
				}
			} else {
				printErrorCode(300);
			}
		}
		else if(cmd == "DFS"){
			char option;
			int vertex;
			string extra;
			if(ss >> option >> vertex && !(ss >> extra)){
				if(!mDFS(option, vertex)){
					printErrorCode(400);
				}
			} else {
				printErrorCode(400);
			}
		}
		else if(cmd == "KRUSKAL"){
			string extra;
			if(!(ss >> extra)){
				if(!mKRUSKAL()){
					printErrorCode(500);
				}
			} else {
				printErrorCode(500);
			}
		}
		else if(cmd == "DIJKSTRA"){
			char option;
			int vertex;
			string extra;
			if(ss >> option >> vertex && !(ss >> extra)){
				if(!mDIJKSTRA(option, vertex)){
					printErrorCode(600);
				}
			} else {
				printErrorCode(600);
			}
		}
		else if(cmd == "BELLMANFORD"){
			char option;
			int s_vertex, e_vertex;
			string extra;
			if(ss >> option >> s_vertex >> e_vertex && !(ss >> extra)){
				if(!mBELLMANFORD(option, s_vertex, e_vertex)){
					printErrorCode(700);
				}
			} else {
				printErrorCode(700);
			}
		}
		else if(cmd == "FLOYD"){
			char option;
			string extra;
			if(ss >> option && !(ss >> extra)){
				if(!mFLOYD(option)){
					printErrorCode(800);
				}
			} else {
				printErrorCode(800);
			}
		}
		else if(cmd == "CENTRALITY"){
			string extra;
			if(!(ss >> extra)){
				if(!mCentrality()){
					printErrorCode(900);
				}
			} else {
				printErrorCode(900);
			}
		}
		else if(cmd == "EXIT"){
			fout << "========EXIT========" << endl;
			fout << "Success" << endl;
			fout << "====================" << endl << endl;
			break;
		}
	}

	fin.close();
	return;
}

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
	// Read Header (Type and Size)
	// Example:
	// L
	// 8
	string line;
	if(!getline(file, line)) return false;
	type = line[0];
	if(!getline(file, line)) return false;
	size = stoi(line);

	// Instantiate appropriate graph
	if(type == 'L'){
		graph = new ListGraph(false, size); // Assuming undirected as base, algorithms handle direction
	}
	else if(type == 'M'){
		graph = new MatrixGraph(false, size);
	}
	else {
		return false; // Invalid type
	}

	if(type == 'L'){
		// List Graph Parsing
		// Format: 
		// Source Vertex (Line)
		// Dest Weight Dest Weight ... (Line)
		while(getline(file, line)){
			if(line.empty()) continue; 
			int src = stoi(line); // Source Vertex

			if(!getline(file, line)) break; // Error or End of file unexpectedly
			stringstream ss(line);
			int dst, weight;
			while(ss >> dst >> weight){
				graph->insertEdge(src, dst, weight);
			}
		}
	}
	else if(type == 'M'){
		// Matrix Graph Parsing
		// Format: Matrix of weights
		int weight;
		for(int i = 0; i < size; i++){
			getline(file, line);
			stringstream ss(line);
			for(int j = 0; j < size; j++){
				if(ss >> weight){
					if(weight != 0){
						graph->insertEdge(i, j, weight);
					}
				}
			}
		}
	}

	load = 1;
	fout << "========LOAD========" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl << endl;
	return true;
}

bool Manager::PRINT()
{
	if(graph == nullptr) return false;
	if(graph->printGraph(&fout)) return true;
	return false;
}

bool Manager::mBFS(char option, int vertex)
{
	if(graph == nullptr) return false;
	return BFS(graph, option, vertex);
}

bool Manager::mDFS(char option, int vertex)
{
	if(graph == nullptr) return false;
	return DFS(graph, option, vertex);
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if(graph == nullptr) return false;
	return Dijkstra(graph, option, vertex);
}

bool Manager::mKRUSKAL()
{
	if(graph == nullptr) return false;
	return Kruskal(graph);
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if(graph == nullptr) return false;
	return Bellmanford(graph, option, s_vertex, e_vertex);
}

bool Manager::mFLOYD(char option)
{
	if(graph == nullptr) return false;
	return FLOYD(graph, option);
}

bool Manager::mCentrality() {
	if(graph == nullptr) return false;
	return Centrality(graph);
}

void Manager::printErrorCode(int n)
{
	fout<<"========ERROR======="<<endl;
	fout<<n<<endl;
	fout<<"===================="<<endl << endl;
}
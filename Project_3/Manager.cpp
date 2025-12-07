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
	// Clear log.txt at start
	fout.open("log.txt", ios::trunc);
	fout.close();
	load = 0;
}

Manager::~Manager()
{
	if(load)
		delete graph;	
	if(fout.is_open())
		fout.close();
}

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

		if(cmd == "LOAD"){
			string filename, extra;
			if(ss >> filename && !(ss >> extra)){
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
				if(option != 'O' && option != 'X'){
					printErrorCode(300);
				} else if(!mBFS(option, vertex)){
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
				if(option != 'O' && option != 'X'){
					printErrorCode(400);
				} else if(!mDFS(option, vertex)){
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
				if(option != 'O' && option != 'X'){
					printErrorCode(600);
				} else if(!mDIJKSTRA(option, vertex)){
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
				if(option != 'O' && option != 'X'){
					printErrorCode(700);
				} else if(!mBELLMANFORD(option, s_vertex, e_vertex)){
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
				if(option != 'O' && option != 'X'){
					printErrorCode(800);
				} else if(!mFLOYD(option)){
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
	if(!getline(file, line)) return false;
	type = line[0];
	if(!getline(file, line)) return false;
	size = stoi(line);

	// Validate size
	if(size < 5) return false;

	// Instantiate appropriate graph
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
			// Read vertex line
			if(!getline(file, line)) break;
			int src = stoi(line);
			
			// Read edges line
			if(!getline(file, line)) break;
			
			// Parse edges
			stringstream ss(line);
			int dst, weight;
			while(ss >> dst >> weight){
				graph->insertEdge(src, dst, weight);
			}
		}
	}
	else if(type == 'M'){
		// Matrix Graph Parsing
		int weight;
		for(int i = 0; i < size; i++){
			if(!getline(file, line)) break;
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

	file.close();
	load = 1;
	
	// Open fout in append mode for writing
	fout.open("log.txt", ios::app);
	fout << "========LOAD========" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl << endl;
	fout.close();
	return true;
}

bool Manager::PRINT()
{
	if(graph == nullptr || load == 0) return false;
	fout.open("log.txt", ios::app);
	bool result = graph->printGraph(&fout);
	fout.close();
	return result;
}

bool Manager::mBFS(char option, int vertex)
{
	if(graph == nullptr || load == 0) return false;
	return BFS(graph, option, vertex);
}

bool Manager::mDFS(char option, int vertex)
{
	if(graph == nullptr || load == 0) return false;
	return DFS(graph, option, vertex);
}

bool Manager::mDIJKSTRA(char option, int vertex)
{
	if(graph == nullptr || load == 0) return false;
	return Dijkstra(graph, option, vertex);
}

bool Manager::mKRUSKAL()
{
	if(graph == nullptr || load == 0) return false;
	return Kruskal(graph);
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex)
{
	if(graph == nullptr || load == 0) return false;
	return Bellmanford(graph, option, s_vertex, e_vertex);
}

bool Manager::mFLOYD(char option)
{
	if(graph == nullptr || load == 0) return false;
	return FLOYD(graph, option);
}

bool Manager::mCentrality() {
	if(graph == nullptr || load == 0) return false;
	return Centrality(graph);
}

void Manager::printErrorCode(int n)
{
	fout.open("log.txt", ios::app);
	fout<<"========ERROR======="<<endl;
	fout<<n<<endl;
	fout<<"===================="<<endl << endl;
	fout.close();
}
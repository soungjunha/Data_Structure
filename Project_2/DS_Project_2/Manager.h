#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
#include <fstream>
#include <string>

using namespace std;

// Manager class that controls the overall operation of the program
// Manages interactions between BpTree and SelectionTree
// Reads commands from command file and logs results to log.txt
class Manager {
private:
	char* cmd;                    // Command string
	BpTree* bptree;               // Pointer to B+ Tree
	SelectionTree* stree;         // Pointer to Selection Tree

public:
	// Constructor: initializes B+ Tree and Selection Tree
	// Opens log file (truncated if already exists)
	Manager(int bp_order) {
		flog.open("log.txt", ios::trunc); 
		bptree = new BpTree(&flog, bp_order);
		stree = new SelectionTree(&flog);
	}

	// Destructor: deallocates trees and closes files
	~Manager() {
		delete bptree;
		delete stree;
		if (fin.is_open()) fin.close();
		if (flog.is_open()) flog.close();
	}

	ifstream fin;   // Input file stream for reading commands
	ofstream flog;  // Output file stream for logging results

	// Main function to read and execute commands from command file
	void run(const char* command);

	// Load employee data from employee.txt into B+ Tree
	void LOAD();

	// Add a single employee directly to the B+ Tree
	void ADD_BP(string name, int dept_no, int id, int income);

	// Search for an employee by name in the B+ Tree
	void SEARCH_BP_NAME(string name);

	// Search for employees within a name range in the B+ Tree
	void SEARCH_BP_RANGE(string start, string end);

	// Print all employees in the B+ Tree in name-ascending order
	void PRINT_BP();

	// Copy employees from B+ Tree to Selection Tree by department code
	void ADD_ST_DEPTNO(int dept_no);

	// Copy a single employee from B+ Tree to Selection Tree by name
	void ADD_ST_NAME(string name);

	// Print all employees in a specific department heap from Selection Tree
	void PRINT_ST(int dept_no);
	
	// Delete the employee with the highest salary from Selection Tree
	void DELETE();

	// Print an error code to the log file
	void printErrorCode(int n);

	// Print a success message with command header to the log file
	void printSuccessCode(string success);
};
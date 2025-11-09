#pragma once
#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"
#include "SelectionTree.h"
#include <fstream>

using namespace std;

// B+ Tree class for managing employee data sorted by name
// Employee data is stored in data nodes (leaves) and sorted by name in ascending order
// Index nodes contain keys and pointers to navigate the tree
class BpTree {
private:
	BpTreeNode* root;      // Root node of the B+ tree
	int	order;             // Order of the B+ tree (m children)
	ofstream* fout;        // Output file stream for printing results

public:
	// Constructor: initializes B+ tree with given order (default 3)
	BpTree(ofstream *fout, int order = 3) {
		this->root = NULL;
		this->order = order;
		this->fout = fout;
	}
	
	// Destructor
	~BpTree() {
		// TODO: Implement recursive deletion of all nodes
	}

	// Insert employee data into the B+ tree
	bool Insert(EmployeeData* new_data);

	// Check if a data node has exceeded its capacity
	bool excessDataNode(BpTreeNode* p_data_node);

	// Check if an index node has exceeded its capacity
	bool excessIndexNode(BpTreeNode* p_index_node);

	// Split a data node that has overflowed
	void splitDataNode(BpTreeNode* p_data_node);

	// Split an index node that has overflowed
	void splitIndexNode(BpTreeNode* p_index_node);

	// Get the root node of the B+ tree
	BpTreeNode* getRoot() { return root; }

	// Search for the data node containing or should contain the given name
	BpTreeNode* searchDataNode(string name);

	// Search and print all employees in the name range [start, end]
	void searchRange(string start, string end);
};

#endif
#pragma once
#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"
#include <map>

// Index node (internal node) class for B+ tree
// Internal nodes store keys and pointers to child nodes for navigation
// Index nodes do not store actual employee data
class BpTreeIndexNode : public BpTreeNode {
private:
	map <string, BpTreeNode*> mapIndex;  // Map storing keys and pointers to child nodes

public:
	// Constructor
	BpTreeIndexNode() {}

	// Destructor
	~BpTreeIndexNode() {
		// TODO: Delete all child nodes
	}

	// Insert a key and child node pointer into the index map
	void insertIndexMap(string name, BpTreeNode* pN) {
		mapIndex.insert(map<string, BpTreeNode*>::value_type(name, pN));
	}

	// Delete a key from the index map
	void deleteMap(string name) {
		mapIndex.erase(name);
	}

	// Get pointer to the index map
	map <string, BpTreeNode*>* getIndexMap() { return &mapIndex; }
};

#endif
#pragma once
#include "EmployeeData.h"

using namespace std;

// Base class for B+ tree nodes
// Provides common functionality for both BpTreeIndexNode (internal nodes) and BpTreeDataNode (leaf nodes)
// Uses virtual functions to allow polymorphic behavior
class BpTreeNode {
private:
	BpTreeNode* p_parent;          // Pointer to parent node
	BpTreeNode* p_most_left_child; // Pointer to the leftmost child node

public:
	// Constructor
	BpTreeNode() {
		p_parent = NULL;
		p_most_left_child = NULL;
	}

	// Destructor
	~BpTreeNode() {
		// Memory deallocation should be handled by derived classes
	}

	// Set the leftmost child node pointer
	void setMostLeftChild(BpTreeNode* p_n) { p_most_left_child = p_n; }

	// Set the parent node pointer
	void setParent(BpTreeNode* p_n) { p_parent = p_n; }

	// Get the parent node pointer
	BpTreeNode* getParent() { return p_parent; }

	// Get the leftmost child node pointer
	BpTreeNode* getMostLeftChild() { return p_most_left_child; }

	// Virtual functions for data node operations (only implemented by BpTreeDataNode)
	// Set next node pointer (for data nodes only)
	virtual void setNext(BpTreeNode* p_n) {}

	// Set previous node pointer (for data nodes only)
	virtual void setPrev(BpTreeNode* p_n) {}

	// Get next node pointer (for data nodes only)
	virtual BpTreeNode* getNext() { return NULL; }

	// Get previous node pointer (for data nodes only)
	virtual BpTreeNode* getPrev() { return NULL; }

	// Insert data into data node's map
	virtual void insertDataMap(string n, EmployeeData* p_n) {}

	// Insert key and pointer into index node's map
	virtual void insertIndexMap(string n, BpTreeNode* p_n) {}

	// Delete an entry from the map
	virtual void deleteMap(string n) {}

	// Get index map (for index nodes)
	virtual map<string, BpTreeNode*>* getIndexMap() { return NULL; }

	// Get data map (for data nodes)
	virtual map<string, EmployeeData*>* getDataMap() { return NULL; }
};
#pragma once
#include "EmployeeData.h"

// Base class for B+ tree nodes
// Provides common functionality for both BpTreeIndexNode (internal nodes) and BpTreeDataNode (leaf nodes)
// Uses virtual functions to allow polymorphic behavior
class BpTreeNode {
private:
	BpTreeNode* pParent;         // Pointer to parent node
	BpTreeNode* pMostLeftChild;  // Pointer to the leftmost child node

public:
	// Constructor
	BpTreeNode() {
		pParent = NULL;
		pMostLeftChild = NULL;
	}

	// Destructor
	~BpTreeNode() {
		// Memory deallocation should be handled by derived classes
	}

	// Set the leftmost child node pointer
	void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; }

	// Set the parent node pointer
	void setParent(BpTreeNode* pN) { pParent = pN; }

	// Get the parent node pointer
	BpTreeNode* getParent() { return pParent; }

	// Get the leftmost child node pointer
	BpTreeNode* getMostLeftChild() { return pMostLeftChild; }

	// Virtual functions for data node operations (only implemented by BpTreeDataNode)
	// Set next node pointer (for data nodes only)
	virtual void setNext(BpTreeNode* pN) {}

	// Set previous node pointer (for data nodes only)
	virtual void setPrev(BpTreeNode* pN) {}

	// Get next node pointer (for data nodes only)
	virtual BpTreeNode* getNext() { return NULL; }

	// Get previous node pointer (for data nodes only)
	virtual BpTreeNode* getPrev() { return NULL; }

	// Insert data into data node's map
	virtual void insertDataMap(string n, EmployeeData* pN) {}

	// Insert key and pointer into index node's map
	virtual void insertIndexMap(string n, BpTreeNode* pN) {}

	// Delete an entry from the map
	virtual void deleteMap(string n) {}

	// Get index map (for index nodes)
	virtual map<string, BpTreeNode*>* getIndexMap() { return NULL; }

	// Get data map (for data nodes)
	virtual map<string, EmployeeData*>* getDataMap() { return NULL; }
};
#pragma once
#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"

// Data node (leaf node) class for B+ tree
// Leaf nodes store actual employee data in a map sorted by name
// Data nodes are linked together in a doubly-linked list for efficient range queries
class BpTreeDataNode : public BpTreeNode {
private:
	map <string, EmployeeData*> mapData;  // Map storing employee data (key: name)
	BpTreeNode* pNext;                     // Pointer to next data node (right sibling)
	BpTreeNode* pPrev;                     // Pointer to previous data node (left sibling)

public:
	// Constructor
	BpTreeDataNode() {
		pNext = NULL;
		pPrev = NULL;
	}

	// Destructor
	~BpTreeDataNode() {
		// TODO: Delete all EmployeeData objects in mapData
	}

	// Set the next data node pointer
	void setNext(BpTreeNode* pN) { pNext = pN; }

	// Set the previous data node pointer
	void setPrev(BpTreeNode* pN) { pPrev = pN; }

	// Get the next data node pointer
	BpTreeNode* getNext() { return pNext; }

	// Get the previous data node pointer
	BpTreeNode* getPrev() { return pPrev; }

	// Insert employee data into the map
	void insertDataMap(string name, EmployeeData* pN) {
		mapData.insert(map<string, EmployeeData*>::value_type(name, pN));
	}

	// Delete employee data from the map by name
	void deleteMap(string name) {
		mapData.erase(name);
	}

	// Get pointer to the data map
	map<string, EmployeeData*>* getDataMap() { return &mapData; }
};

#endif
#pragma once
#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"

using namespace std;

// Data node (leaf node) class for B+ tree
// Leaf nodes store actual employee data in a map sorted by name
// Data nodes are linked together in a doubly-linked list for efficient range queries
class BpTreeDataNode : public BpTreeNode {
private:
	map <string, EmployeeData*> map_data;  // Map storing employee data (key: name)
	BpTreeNode* p_next;                    // Pointer to next data node (right sibling)
	BpTreeNode* p_prev;                    // Pointer to previous data node (left sibling)

public:
	// Constructor
	BpTreeDataNode() {
		p_next = NULL;
		p_prev = NULL;
	}

	// Destructor
	~BpTreeDataNode() {
		// TODO: Delete all EmployeeData objects in map_data
	}

	// Set the next data node pointer
	void setNext(BpTreeNode* p_n) { p_next = p_n; }

	// Set the previous data node pointer
	void setPrev(BpTreeNode* p_n) { p_prev = p_n; }

	// Get the next data node pointer
	BpTreeNode* getNext() { return p_next; }

	// Get the previous data node pointer
	BpTreeNode* getPrev() { return p_prev; }

	// Insert employee data into the map
	void insertDataMap(string name, EmployeeData* p_n) {
		map_data.insert(map<string, EmployeeData*>::value_type(name, p_n));
	}

	// Delete employee data from the map by name
	void deleteMap(string name) {
		map_data.erase(name);
	}

	// Get pointer to the data map
	map<string, EmployeeData*>* getDataMap() { return &map_data; }
};

#endif
#pragma once
#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"
#include "SelectionTree.h"
#include <fstream> // Need this for ofstream

class BpTree {
private:
	BpTreeNode* root;
	int	order;		// m children
	std::ofstream* fout; // For printing (e.g., in searchRange)

public:
	BpTree(std::ofstream *fout, int order = 3) {
		// Initialize member variables
		this->root = nullptr;
		this->order = order;
		this->fout = fout;
	}
	
	~BpTree() {

	}

	bool		Insert(EmployeeData* newData);
	bool		excessDataNode(BpTreeNode* pDataNode);
	bool		excessIndexNode(BpTreeNode* pIndexNode);
	void		splitDataNode(BpTreeNode* pDataNode);
	void		splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return root; }
	BpTreeNode* searchDataNode(string name);
	void		searchRange(string start, string end);
};

#endif

#include "BpTree.h"
#include <iostream>

// Insert a new employee data into the B+ tree
// If the name already exists, only the salary is updated
bool BpTree::Insert(EmployeeData* newData) {
    // Get employee name using getter function
    string name = newData->getName();

    // Case 1: The tree is completely empty - create root as data node
    if (root == NULL) {
        root = new BpTreeDataNode();
        root->insertDataMap(name, newData);
        return true;
    }

    // Find the correct leaf node to insert into
    BpTreeNode* pLeaf = searchDataNode(name);
    map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();

    // Case 2: Key already exists - update only the salary
    auto it = dataMap->find(name);
    if (it != dataMap->end()) {
        // Update salary using setter/getter functions
        it->second->setIncome(newData->getIncome());
        delete newData; // Delete the new object to prevent memory leak
        return true;
    }

    // Case 3: Key does not exist - insert the new data
    pLeaf->insertDataMap(name, newData);

    // Case 4: Check if the leaf node overflows and needs splitting
    if (excessDataNode(pLeaf)) {
        splitDataNode(pLeaf);
    }
    return true;
}

// Check if a data node has exceeded capacity
// A data node overflows if it has more than (order - 1) entries
bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
    return pDataNode->getDataMap()->size() > order - 1;
}

// Check if an index node has exceeded capacity
// An index node overflows if it has more than (order - 1) keys
bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
    return pIndexNode->getIndexMap()->size() > order - 1;
}

// Split a data node that has overflowed
// Splits a data node into two nodes and copies the split key up to the parent index node
// Maintains the doubly-linked list of leaf nodes
void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pDataNode);
    map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();

    // Find the split point (middle element)
    int splitIndex = dataMap->size() / 2;
    auto it = dataMap->begin();
    std::advance(it, splitIndex);

    // This key will be copied up to the parent
    string splitKey = it->first;

    // Create a new sibling data node
    BpTreeDataNode* pNewNode = new BpTreeDataNode();

    // Move the second half of data from the old node to the new node
    while (it != dataMap->end()) {
        pNewNode->insertDataMap(it->first, it->second);
        it = dataMap->erase(it); // Erase from old node and advance iterator
    }

    // Update the doubly-linked list pointers for leaf nodes
    BpTreeNode* pNext = pLeaf->getNext();
    pLeaf->setNext(pNewNode);
    pNewNode->setPrev(pLeaf);
    pNewNode->setNext(pNext);
    if (pNext) {
        pNext->setPrev(pNewNode);
    }

    // Handle the parent node
    BpTreeNode* pParent = pLeaf->getParent();
    if (pParent == NULL) {
        // This was the root - create a new root (IndexNode)
        root = new BpTreeIndexNode();
        root->setMostLeftChild(pLeaf);
        root->insertIndexMap(splitKey, pNewNode);
        pLeaf->setParent(root);
        pNewNode->setParent(root);
    } else {
        // Insert the new key and pointer to the new node into the parent
        pParent->insertIndexMap(splitKey, pNewNode);
        pNewNode->setParent(pParent);
        
        // Check if the parent node now overflows
        if (excessIndexNode(pParent)) {
            splitIndexNode(pParent);
        }
    }
}

// Split an index node that has overflowed
// Splits an index node into two nodes and pushes the split key up to the parent index node
void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    BpTreeIndexNode* pIndex = static_cast<BpTreeIndexNode*>(pIndexNode);
    map<string, BpTreeNode*>* indexMap = pIndex->getIndexMap();

    // Find the split point (middle key)
    int splitIndex = indexMap->size() / 2;
    auto it = indexMap->begin();
    std::advance(it, splitIndex);

    // This key will be pushed up to the parent
    string splitKey = it->first;
    // This child becomes the mostLeftChild of the new node
    BpTreeNode* pSplitChild = it->second;

    // Create a new sibling index node
    BpTreeIndexNode* pNewNode = new BpTreeIndexNode();
    
    // The child associated with the split key becomes the leftmost child of the new node
    pNewNode->setMostLeftChild(pSplitChild);
    pSplitChild->setParent(pNewNode);

    // Move keys/children after the split key to the new node
    it = indexMap->erase(it); // Erase split key (moving to parent) and get next
    while (it != indexMap->end()) {
        pNewNode->insertIndexMap(it->first, it->second);
        it->second->setParent(pNewNode); // Update parent of the moved child
        it = indexMap->erase(it);
    }

    // Handle the parent node
    BpTreeNode* pParent = pIndex->getParent();
    if (pParent == NULL) {
        // This was the root - create a new root
        root = new BpTreeIndexNode();
        root->setMostLeftChild(pIndex);
        root->insertIndexMap(splitKey, pNewNode);
        pIndex->setParent(root);
        pNewNode->setParent(root);
    } else {
        // Insert the split key and pointer to the new node into the parent
        pParent->insertIndexMap(splitKey, pNewNode);
        pNewNode->setParent(pParent);

        // Check if the parent node now overflows
        if (excessIndexNode(pParent)) {
            splitIndexNode(pParent);
        }
    }
}

// Search for the data node (leaf) that contains or should contain the given name
// Navigates through index nodes to find the appropriate leaf node
BpTreeNode* BpTree::searchDataNode(string name) {
    BpTreeNode* pCur = root;
    if (pCur == NULL) {
        return NULL; // Tree is empty
    }

    // Navigate down the tree while pCur is an index node
    // Check by testing if getDataMap() returns nullptr
    while (pCur->getDataMap() == NULL) {
        map<string, BpTreeNode*>* indexMap = pCur->getIndexMap();

        // upper_bound finds the first key strictly greater than 'name'
        auto it = indexMap->upper_bound(name);

        if (it == indexMap->begin()) {
            // 'name' is smaller than or equal to all keys in this index node
            // Follow the leftmost child pointer
            pCur = pCur->getMostLeftChild();
        } else {
            // 'name' is greater than the key pointed to by (it-1)
            // Follow the child pointer associated with that previous key
            it--;
            pCur = it->second;
        }
    }
    
    // Now pCur is a BpTreeDataNode (leaf)
    return pCur;
}

// Search and print all employees in the name range [start, end]
// Traverses the doubly-linked list of leaf nodes to find and print all employees
void BpTree::searchRange(string start, string end) {
    // Find the leaf node where the 'start' key would be
    BpTreeNode* pNode = searchDataNode(start);
    if (pNode == NULL) {
        return; // Tree is empty
    }

    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pNode);
    bool finished = false;

    // Iterate through the leaf nodes using the pNext pointer
    while (pLeaf != NULL && !finished) {
        map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();
        
        // Find the first element >= start
        auto it = dataMap->lower_bound(start);

        for (; it != dataMap->end(); ++it) {
            // Stop if we've exceeded the end range
            if (it->first > end) {
                finished = true; 
                break;
            }
            
            EmployeeData* data = it->second;
            
            // Print in format: name/dept_no/ID/income
            *fout << data->getName() << "/" << data->getDeptNo() << "/" 
                  << data->getID() << "/" << data->getIncome() << std::endl;
        }
        
        // Move to the next leaf node
        pLeaf = static_cast<BpTreeDataNode*>(pLeaf->getNext());
        start = ""; // Start from the beginning of the next node
    }
}
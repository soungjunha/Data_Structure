#include "BpTree.h"
#include <iostream>

using namespace std;

// Insert a new employee data into the B+ tree
// If the name already exists, only the salary is updated
bool BpTree::Insert(EmployeeData* new_data) {
    // Get employee name using getter function
    string name = new_data->getName();

    // Case 1: The tree is completely empty - create root as data node
    if (root == NULL) {
        root = new BpTreeDataNode();
        root->insertDataMap(name, new_data);
        return true;
    }

    // Find the correct leaf node to insert into
    BpTreeNode* p_leaf = searchDataNode(name);
    map<string, EmployeeData*>* data_map = p_leaf->getDataMap();

    // Case 2: Key already exists - update only the salary
    map<string, EmployeeData*>::iterator it = data_map->find(name);
    if (it != data_map->end()) {
        // Update salary using setter/getter functions
        it->second->setIncome(new_data->getIncome());
        delete new_data; // Delete the new object to prevent memory leak
        return true;
    }

    // Case 3: Key does not exist - insert the new data
    p_leaf->insertDataMap(name, new_data);

    // Case 4: Check if the leaf node overflows and needs splitting
    if (excessDataNode(p_leaf)) {
        splitDataNode(p_leaf);
    }
    return true;
}

// Check if a data node has exceeded capacity
// A data node overflows if it has more than (order - 1) entries
bool BpTree::excessDataNode(BpTreeNode* p_data_node) {
    return p_data_node->getDataMap()->size() > order - 1;
}

// Check if an index node has exceeded capacity
// An index node overflows if it has more than (order - 1) keys
bool BpTree::excessIndexNode(BpTreeNode* p_index_node) {
    return p_index_node->getIndexMap()->size() > order - 1;
}

// Split a data node that has overflowed
// Splits a data node into two nodes and copies the split key up to the parent index node
// Maintains the doubly-linked list of leaf nodes
void BpTree::splitDataNode(BpTreeNode* p_data_node) {
    BpTreeDataNode* p_leaf = static_cast<BpTreeDataNode*>(p_data_node);
    map<string, EmployeeData*>* data_map = p_leaf->getDataMap();

    // Find the split point (middle element)
    int split_index = data_map->size() / 2;
    map<string, EmployeeData*>::iterator it = data_map->begin();
    advance(it, split_index);

    // This key will be copied up to the parent
    string split_key = it->first;

    // Create a new sibling data node
    BpTreeDataNode* p_new_node = new BpTreeDataNode();

    // Move the second half of data from the old node to the new node
    while (it != data_map->end()) {
        p_new_node->insertDataMap(it->first, it->second);
        it = data_map->erase(it); // Erase from old node and advance iterator
    }

    // Update the doubly-linked list pointers for leaf nodes
    BpTreeNode* p_next = p_leaf->getNext();
    p_leaf->setNext(p_new_node);
    p_new_node->setPrev(p_leaf);
    p_new_node->setNext(p_next);
    if (p_next) {
        p_next->setPrev(p_new_node);
    }

    // Handle the parent node
    BpTreeNode* p_parent = p_leaf->getParent();
    if (p_parent == NULL) {
        // This was the root - create a new root (IndexNode)
        root = new BpTreeIndexNode();
        root->setMostLeftChild(p_leaf);
        root->insertIndexMap(split_key, p_new_node);
        p_leaf->setParent(root);
        p_new_node->setParent(root);
    } else {
        // Insert the new key and pointer to the new node into the parent
        p_parent->insertIndexMap(split_key, p_new_node);
        p_new_node->setParent(p_parent);
        
        // Check if the parent node now overflows
        if (excessIndexNode(p_parent)) {
            splitIndexNode(p_parent);
        }
    }
}

// Split an index node that has overflowed
// Splits an index node into two nodes and pushes the split key up to the parent index node
void BpTree::splitIndexNode(BpTreeNode* p_index_node) {
    BpTreeIndexNode* p_index = static_cast<BpTreeIndexNode*>(p_index_node);
    map<string, BpTreeNode*>* index_map = p_index->getIndexMap();

    // Find the split point (middle key)
    int split_index = index_map->size() / 2;
    map<string, BpTreeNode*>::iterator it = index_map->begin();
    advance(it, split_index);

    // This key will be pushed up to the parent
    string split_key = it->first;
    // This child becomes the mostLeftChild of the new node
    BpTreeNode* p_split_child = it->second;

    // Create a new sibling index node
    BpTreeIndexNode* p_new_node = new BpTreeIndexNode();
    
    // The child associated with the split key becomes the leftmost child of the new node
    p_new_node->setMostLeftChild(p_split_child);
    p_split_child->setParent(p_new_node);

    // Move keys/children after the split key to the new node
    it = index_map->erase(it); // Erase split key (moving to parent) and get next
    while (it != index_map->end()) {
        p_new_node->insertIndexMap(it->first, it->second);
        it->second->setParent(p_new_node); // Update parent of the moved child
        it = index_map->erase(it);
    }

    // Handle the parent node
    BpTreeNode* p_parent = p_index->getParent();
    if (p_parent == NULL) {
        // This was the root - create a new root
        root = new BpTreeIndexNode();
        root->setMostLeftChild(p_index);
        root->insertIndexMap(split_key, p_new_node);
        p_index->setParent(root);
        p_new_node->setParent(root);
    } else {
        // Insert the split key and pointer to the new node into the parent
        p_parent->insertIndexMap(split_key, p_new_node);
        p_new_node->setParent(p_parent);

        // Check if the parent node now overflows
        if (excessIndexNode(p_parent)) {
            splitIndexNode(p_parent);
        }
    }
}

// Search for the data node (leaf) that contains or should contain the given name
// Navigates through index nodes to find the appropriate leaf node
BpTreeNode* BpTree::searchDataNode(string name) {
    BpTreeNode* p_cur = root;
    if (p_cur == NULL) {
        return NULL; // Tree is empty
    }

    // Navigate down the tree while p_cur is an index node
    // Check by testing if getDataMap() returns nullptr
    while (p_cur->getDataMap() == NULL) {
        map<string, BpTreeNode*>* index_map = p_cur->getIndexMap();

        // upper_bound finds the first key strictly greater than 'name'
        map<string, BpTreeNode*>::iterator it = index_map->upper_bound(name);

        if (it == index_map->begin()) {
            // 'name' is smaller than or equal to all keys in this index node
            // Follow the leftmost child pointer
            p_cur = p_cur->getMostLeftChild();
        } else {
            // 'name' is greater than the key pointed to by (it-1)
            // Follow the child pointer associated with that previous key
            it--;
            p_cur = it->second;
        }
    }
    
    // Now p_cur is a BpTreeDataNode (leaf)
    return p_cur;
}

// Search and print all employees in the name range [start, end]
// Traverses the doubly-linked list of leaf nodes to find and print all employees
void BpTree::searchRange(string start, string end) {
    // Find the leaf node where the 'start' key would be
    BpTreeNode* p_node = searchDataNode(start);
    if (p_node == NULL) {
        return; // Tree is empty
    }

    BpTreeDataNode* p_leaf = static_cast<BpTreeDataNode*>(p_node);
    bool finished = false;

    // Iterate through the leaf nodes using the pNext pointer
    while (p_leaf != NULL && !finished) {
        map<string, EmployeeData*>* data_map = p_leaf->getDataMap();
        
        // Find the first element >= start
        map<string, EmployeeData*>::iterator it = data_map->lower_bound(start);

        for (; it != data_map->end(); ++it) {
            // Stop if we've exceeded the end range
            if (it->first > end) {
                finished = true; 
                break;
            }
            
            EmployeeData* data = it->second;
            
            // Print in format: name/dept_no/ID/income
            *fout << data->getName() << "/" << data->getDeptNo() << "/" 
                  << data->getID() << "/" << data->getIncome() << endl;
        }
        
        // Move to the next leaf node
        p_leaf = static_cast<BpTreeDataNode*>(p_leaf->getNext());
        start = ""; // Start from the beginning of the next node
    }
}
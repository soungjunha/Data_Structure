#pragma once
#include "EmployeeData.h"
#include "EmployeeHeap.h"

using namespace std;

// Node class for Selection Tree
// Internal nodes store the winner (employee with highest income) from their children
// Leaf nodes (runs) contain EmployeeHeap objects that store actual employee data
class SelectionTreeNode {
private:
    EmployeeData* p_data;           // Winner data (employee with highest income)
    SelectionTreeNode* p_left;      // Pointer to left child node
    SelectionTreeNode* p_right;     // Pointer to right child node
    SelectionTreeNode* p_parent;    // Pointer to parent node
    EmployeeHeap* h_root;           // Heap root (only for leaf nodes/runs)

public:
    // Constructor
    SelectionTreeNode() {
        this->p_data = NULL;
        this->p_left = NULL;
        this->p_right = NULL;
        this->p_parent = NULL;
        this->h_root = NULL;
    }

    // Destructor
    ~SelectionTreeNode() {
        if (h_root) delete h_root;
    }

    // Initialize the heap for this node (for leaf nodes only)
    void HeapInit() { this->h_root = new EmployeeHeap; }

    // Set the employee data (winner) for this node
    void setEmployeeData(EmployeeData* data) { this->p_data = data; }

    // Set the left child node pointer
    void setLeftChild(SelectionTreeNode* p_l) { this->p_left = p_l; }

    // Set the right child node pointer
    void setRightChild(SelectionTreeNode* p_r) { this->p_right = p_r; }

    // Set the parent node pointer
    void setParent(SelectionTreeNode* p_p) { this->p_parent = p_p; }

    // Set the heap root pointer
    void setHeap(EmployeeHeap* p_hr) { this->h_root = p_hr; }

    // Get the employee data (winner) stored in this node
    EmployeeData* getEmployeeData() { return p_data; }

    // Get the left child node pointer
    SelectionTreeNode* getLeftChild() { return p_left; }

    // Get the right child node pointer
    SelectionTreeNode* getRightChild() { return p_right; }

    // Get the parent node pointer
    SelectionTreeNode* getParent() { return p_parent; }

    // Get the heap pointer (for leaf nodes only)
    EmployeeHeap* getHeap() { return h_root; }
};
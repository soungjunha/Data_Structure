#pragma once
#include "EmployeeData.h"
#include "EmployeeHeap.h"

// Node class for Selection Tree
// Internal nodes store the winner (employee with highest income) from their children
// Leaf nodes (runs) contain EmployeeHeap objects that store actual employee data
class SelectionTreeNode {
private:
    EmployeeData* pData;            // Winner data (employee with highest income)
    SelectionTreeNode* pLeft;       // Pointer to left child node
    SelectionTreeNode* pRight;      // Pointer to right child node
    SelectionTreeNode* pParent;     // Pointer to parent node
    EmployeeHeap* hRoot;            // Heap root (only for leaf nodes/runs)

public:
    // Constructor
    SelectionTreeNode() {
        this->pData = NULL;
        this->pLeft = NULL;
        this->pRight = NULL;
        this->pParent = NULL;
        this->hRoot = NULL;
    }

    // Destructor
    ~SelectionTreeNode() {
        if (hRoot) delete hRoot;
    }

    // Initialize the heap for this node (for leaf nodes only)
    void HeapInit() { this->hRoot = new EmployeeHeap; }

    // Set the employee data (winner) for this node
    void setEmployeeData(EmployeeData* data) { this->pData = data; }

    // Set the left child node pointer
    void setLeftChild(SelectionTreeNode* pL) { this->pLeft = pL; }

    // Set the right child node pointer
    void setRightChild(SelectionTreeNode* pR) { this->pRight = pR; }

    // Set the parent node pointer
    void setParent(SelectionTreeNode* pP) { this->pParent = pP; }

    // Set the heap root pointer
    void setHeap(EmployeeHeap* pHR) { this->hRoot = pHR; }

    // Get the employee data (winner) stored in this node
    EmployeeData* getEmployeeData() { return pData; }

    // Get the left child node pointer
    SelectionTreeNode* getLeftChild() { return pLeft; }

    // Get the right child node pointer
    SelectionTreeNode* getRightChild() { return pRight; }

    // Get the parent node pointer
    SelectionTreeNode* getParent() { return pParent; }

    // Get the heap pointer (for leaf nodes only)
    EmployeeHeap* getHeap() { return hRoot; }
};
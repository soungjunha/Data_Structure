#pragma once
#include "SelectionTreeNode.h"
#include <fstream>

// Selection Tree class for managing employee salary rankings
// Implements a Max Winner Tree structure based on employee income
// Has 8 runs (leaf nodes) corresponding to 8 department codes (100~800)
// Tree is implemented using pointers (not arrays)
class SelectionTree {
private:
    SelectionTreeNode* root;           // Root node of the Selection Tree
    ofstream* fout;                    // Output file stream for logging
    SelectionTreeNode* run[8];         // 8 leaf nodes: [0]=100, [1]=200, ..., [7]=800

    // Convert department code to run index
    int getRunIndex(int dept_no) {return (dept_no / 100) - 1;}

    // Update the Selection Tree from a leaf node up to the root
    // When the heap is reorganized, the Selection Tree is also reorganized
    void updateTree(SelectionTreeNode* node);

public:
    // Constructor: creates 8 leaf nodes with heaps and builds tree structure
    // Number of runs equals the number of department codes
    SelectionTree(std::ofstream* fout) {
        this->fout = fout;
        this->root = NULL;

        // Create 8 leaf nodes (runs) and initialize a heap for each
        for (int i = 0; i < 8; i++) {
            run[i] = new SelectionTreeNode();
            run[i]->HeapInit(); // Each leaf node gets an EmployeeHeap
        }
        
        // Build the tree structure based on the leaf nodes
        setTree();
    }

    // Destructor
    ~SelectionTree() {
        // TODO: Recursively delete all nodes (run[8] and internal nodes)
        for (int i = 0; i < 8; i++) {
            delete run[i]; // Delete each leaf node (and its heap)
        }
        // ... (Internal nodes also need to be deleted)
    }

    // Set the root node of the Selection Tree
    void setRoot(SelectionTreeNode* pN) { this->root = pN; }

    // Get the root node of the Selection Tree
    SelectionTreeNode* getRoot() { return root; }

    // Build the Selection Tree structure from leaf nodes
    void setTree();

    // Insert employee data into the Selection Tree
    bool Insert(EmployeeData* newData);

    // Delete the employee with the highest salary from the Selection Tree
    bool Delete();

    // Print all employees from a specific department's heap
    bool printEmployeeData(int dept_no);
};
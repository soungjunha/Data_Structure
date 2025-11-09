#include "SelectionTree.h"
#include <vector>
#include <algorithm>
#include <string>

// Build the Selection Tree structure from leaf nodes (runs)
// Constructs the tree by creating parent nodes level by level
// Tree is implemented using pointers (not arrays)
void SelectionTree::setTree() {
    std::vector<SelectionTreeNode*> currentLevel;
    
    // Level 0: 8 leaf nodes (each node owns a heap)
    for (int i = 0; i < 8; i++) {
        currentLevel.push_back(run[i]);
    }

    // Build tree from Level 0 -> Level 1 -> ... -> Root
    while (currentLevel.size() > 1) {
        std::vector<SelectionTreeNode*> parentLevel;
        
        // Pair nodes to create parent nodes
        for (size_t i = 0; i < currentLevel.size(); i += 2) {
            SelectionTreeNode* parent = new SelectionTreeNode();
            SelectionTreeNode* leftChild = currentLevel[i];
            SelectionTreeNode* rightChild = currentLevel[i + 1];

            parent->setLeftChild(leftChild);
            parent->setRightChild(rightChild);
            leftChild->setParent(parent);
            rightChild->setParent(parent);

            parentLevel.push_back(parent);
        }
        currentLevel = parentLevel;
    }

    // The last remaining node is the root
    this->setRoot(currentLevel[0]);
}

// Insert employee data into the Selection Tree
// Data is inserted into the appropriate heap based on department code
// Heap is reorganized after insertion and changes propagate up to root
bool SelectionTree::Insert(EmployeeData* newData) {
    int dept = newData->getDeptNo();
    int index = getRunIndex(dept);

    if (index < 0 || index > 7) {
        return false; // Invalid department code (Error 500)
    }

    // Access the leaf node and heap for this department
    SelectionTreeNode* leaf = run[index];
    EmployeeHeap* heap = leaf->getHeap();

    // Insert data into the heap (Max Heap based on income)
    heap->Insert(newData);

    // Set the heap's new winner (Top) as the leaf node's data
    leaf->setEmployeeData(heap->Top());

    // Propagate changes up to the root (as required by specifications)
    updateTree(leaf);
    
    return true;
}

// Delete the employee with the highest salary from the Selection Tree
// Removes the employee stored in the root node (highest income)
// After deletion, heap is reorganized and changes propagate up to root
bool SelectionTree::Delete() {
    EmployeeData* winner = root->getEmployeeData();

    // Tree is empty (Error 700)
    if (winner == NULL) {
        return false;
    }

    // Find the department and access the corresponding heap
    int dept = winner->getDeptNo();
    int index = getRunIndex(dept);

    SelectionTreeNode* leaf = run[index];
    EmployeeHeap* heap = leaf->getHeap();

    // Delete the root (highest income employee) from the heap
    heap->Delete();

    // Set the heap's new winner (Top) as the leaf node's data
    leaf->setEmployeeData(heap->Top());

    // Propagate changes up to the root
    updateTree(leaf);

    return true;
}

// Print all employees from a specific department's heap
// Prints employees sorted by income in descending order
// Returns false if department code is invalid or heap is empty
bool SelectionTree::printEmployeeData(int dept_no) {
    int index = getRunIndex(dept_no);
    if (index < 0 || index > 7) {
        return false; // Invalid department (Error 600)
    }

    EmployeeHeap* heap = run[index]->getHeap();
    if (heap == NULL || heap->IsEmpty()) {
        return false; // Heap does not exist or is empty (Error 600)
    }

    // Print header
    *fout << "========PRINT_ST========\n";

    // Use getter functions from EmployeeHeap
    EmployeeData** heapArr = heap->getHeapArray(); 
    int count = heap->getDataNum();               

    // Copy all heap data to a temporary vector (heap index starts from 1)
    std::vector<EmployeeData*> sortedList;
    for (int i = 1; i <= count; i++) {
        sortedList.push_back(heapArr[i]);
    }

    // Sort the vector by income in descending order
    std::sort(sortedList.begin(), sortedList.end(), 
        [](EmployeeData* a, EmployeeData* b) {
            // Higher income first (descending order)
            return a->getIncome() > b->getIncome();
        }
    );

    // Print the sorted list in the required format
    for (EmployeeData* data : sortedList) {
        *fout << data->getName() << "/" << data->getDeptNo() << "/"
              << data->getID() << "/" << data->getIncome() << std::endl;
    }

    // Print footer
    *fout << "========================\n\n";

    return true;
}

// Update the Selection Tree from a leaf node up to the root
// Propagates changes by comparing incomes of left and right children
// Selects the winner (employee with higher income) for Max Winner Tree
void SelectionTree::updateTree(SelectionTreeNode* node) {
    SelectionTreeNode* current = node;

    // Repeat until reaching the root
    while (current != root && current->getParent() != NULL) {
        SelectionTreeNode* parent = current->getParent();
        SelectionTreeNode* left = parent->getLeftChild();
        SelectionTreeNode* right = parent->getRightChild();

        EmployeeData* leftWinner = left->getEmployeeData();
        EmployeeData* rightWinner = right->getEmployeeData();
        EmployeeData* newParentWinner = NULL;

        // Max Winner Tree: Select the child with higher income
        if (leftWinner == NULL && rightWinner == NULL) {
            newParentWinner = NULL;
        } else if (leftWinner == NULL) {
            newParentWinner = rightWinner;
        } else if (rightWinner == NULL) {
            newParentWinner = leftWinner;
        } else if (leftWinner->getIncome() > rightWinner->getIncome()) {
            newParentWinner = leftWinner;
        } else {
            newParentWinner = rightWinner;
        }

        // Check if the parent's winner has changed (if not, stop)
        if (parent->getEmployeeData() == newParentWinner) {
            break;
        }

        // Update the parent's winner and move to the next level
        parent->setEmployeeData(newParentWinner);
        current = parent;
    }
}
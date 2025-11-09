#pragma once

#include "EmployeeData.h"

// Max heap class for managing employees sorted by income
// Implemented using a pointer array (not linked list)
// Index 0 is unused, for node at index n, children are at 2n and 2n+1
class EmployeeHeap {
private:
    int datanum;                  // Current number of elements in the heap
    EmployeeData** heapArr;       // Pointer array for heap implementation
    int maxCapacity = 10;         // Maximum capacity of the heap array

public:
    // Constructor: initializes empty heap with capacity 10
    // Index 0 is set to nullptr and not used
    EmployeeHeap() {
        this->datanum = 0;
        this->heapArr = new EmployeeData*[this->maxCapacity];
        this->heapArr[0] = NULL; 
    }

    // Destructor
    ~EmployeeHeap() {
        delete[] heapArr;
    }
    
    // Insert employee data into the max heap
    void Insert(EmployeeData* data);

    // Get the employee with the highest income (root)
    EmployeeData* Top();

    // Delete the employee with the highest income from the heap
    void Delete();

    // Check if the heap is empty
    bool IsEmpty();

    // Perform up-heap operation to maintain max heap property
    void UpHeap(int index);

    // Perform down-heap operation to maintain max heap property
    void DownHeap(int index);

    // Resize the heap array when capacity is exceeded
    void ResizeArray();

    // Get the heap array pointer
    EmployeeData** getHeapArray() { return heapArr; }

    // Get the current number of elements in the heap
    int getDataNum() { return datanum; }
};
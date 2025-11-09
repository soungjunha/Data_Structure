#pragma once
#include "EmployeeHeap.h"
#include <algorithm>

// Insert employee data into the max heap
// New nodes are added from the left child first
// Heap is resized if necessary and maintained through UpHeap
void EmployeeHeap::Insert(EmployeeData* data) {
    // Check if resize is needed
    if (datanum + 1 >= maxCapacity) {
        ResizeArray();
    }

    // Insert at the next available leaf position
    datanum++;
    heapArr[datanum] = data;

    // Perform UpHeap to maintain max heap property
    UpHeap(datanum);
}

// Get the employee with the highest income (root of max heap)
EmployeeData* EmployeeHeap::Top() {
    if (IsEmpty()) {
        return NULL;
    }
    return heapArr[1]; // Root of the max heap
}

// Delete the employee with the highest income from the heap
// When the heap is reorganized, the Selection Tree is also reorganized accordingly
void EmployeeHeap::Delete() {
    if (IsEmpty()) {
        return;
    }

    // Replace root with the last element
    heapArr[1] = heapArr[datanum];
    datanum--;

    // Perform DownHeap to maintain max heap property
    if (!IsEmpty()) {
        DownHeap(1);
    }
}

// Check if the heap is empty
bool EmployeeHeap::IsEmpty() {
    return datanum == 0;
}

// Perform up-heap operation to maintain max heap property
// Compares the element with its parent and swaps if child's income is greater
void EmployeeHeap::UpHeap(int index) {
    if (index <= 1) {
        return; // At root
    }

    int parentIndex = index / 2;

    // Compare incomes (Max Heap) - parent must be >= child
    if (heapArr[index]->getIncome() > heapArr[parentIndex]->getIncome()) {
        std::swap(heapArr[index], heapArr[parentIndex]);
        UpHeap(parentIndex); // Recurse on parent
    }
}

// Perform down-heap operation to maintain max heap property
// Sorting is complete when all parent incomes are >= children incomes
void EmployeeHeap::DownHeap(int index) {
    int leftChild = 2 * index;
    int rightChild = 2 * index + 1;
    int largerChild = leftChild; // Assume left is larger

    // Case 1: No children (leaf node)
    if (leftChild > datanum) {
        return;
    }

    // Case 2: One child (only left)
    // No need to check, 'largerChild' is already 'leftChild'

    // Case 3: Two children - find the child with larger income
    if (rightChild <= datanum) {
        if (heapArr[rightChild]->getIncome() > heapArr[leftChild]->getIncome()) {
            largerChild = rightChild;
        }
    }

    // Compare current node with the larger child
    if (heapArr[index]->getIncome() < heapArr[largerChild]->getIncome()) {
        std::swap(heapArr[index], heapArr[largerChild]);
        DownHeap(largerChild); // Recurse on the child's position
    }
}

// Resize the heap array when capacity is exceeded
// Doubles the capacity through dynamic allocation
void EmployeeHeap::ResizeArray() {
    int newCapacity = maxCapacity * 2;
    EmployeeData** newArr = new EmployeeData*[newCapacity];

    // Copy existing data (including nullptr at index 0)
    for (int i = 0; i <= datanum; i++) {
        newArr[i] = heapArr[i];
    }

    delete[] heapArr;
    heapArr = newArr;
    maxCapacity = newCapacity;
}
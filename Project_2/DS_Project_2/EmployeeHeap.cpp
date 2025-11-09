#pragma once
#include "EmployeeHeap.h"
#include <algorithm>

using namespace std;

// Insert employee data into the max heap
// New nodes are added from the left child first
// Heap is resized if necessary and maintained through UpHeap
void EmployeeHeap::Insert(EmployeeData* data) {
    // Check if resize is needed
    if (data_num + 1 >= max_capacity) {
        ResizeArray();
    }

    // Insert at the next available leaf position
    data_num++;
    heap_arr[data_num] = data;

    // Perform UpHeap to maintain max heap property
    UpHeap(data_num);
}

// Get the employee with the highest income (root of max heap)
EmployeeData* EmployeeHeap::Top() {
    if (IsEmpty()) {
        return NULL;
    }
    return heap_arr[1]; // Root of the max heap
}

// Delete the employee with the highest income from the heap
// When the heap is reorganized, the Selection Tree is also reorganized accordingly
void EmployeeHeap::Delete() {
    if (IsEmpty()) {
        return;
    }

    // Replace root with the last element
    heap_arr[1] = heap_arr[data_num];
    data_num--;

    // Perform DownHeap to maintain max heap property
    if (!IsEmpty()) {
        DownHeap(1);
    }
}

// Check if the heap is empty
bool EmployeeHeap::IsEmpty() {
    return data_num == 0;
}

// Perform up-heap operation to maintain max heap property
// Compares the element with its parent and swaps if child's income is greater
void EmployeeHeap::UpHeap(int index) {
    if (index <= 1) {
        return; // At root
    }

    int parent_index = index / 2;

    // Compare incomes (Max Heap) - parent must be >= child
    if (heap_arr[index]->getIncome() > heap_arr[parent_index]->getIncome()) {
        swap(heap_arr[index], heap_arr[parent_index]);
        UpHeap(parent_index); // Recurse on parent
    }
}

// Perform down-heap operation to maintain max heap property
// Sorting is complete when all parent incomes are >= children incomes
void EmployeeHeap::DownHeap(int index) {
    int left_child = 2 * index;
    int right_child = 2 * index + 1;
    int larger_child = left_child; // Assume left is larger

    // Case 1: No children (leaf node)
    if (left_child > data_num) {
        return;
    }

    // Case 2: One child (only left)
    // No need to check, 'larger_child' is already 'left_child'

    // Case 3: Two children - find the child with larger income
    if (right_child <= data_num) {
        if (heap_arr[right_child]->getIncome() > heap_arr[left_child]->getIncome()) {
            larger_child = right_child;
        }
    }

    // Compare current node with the larger child
    if (heap_arr[index]->getIncome() < heap_arr[larger_child]->getIncome()) {
        swap(heap_arr[index], heap_arr[larger_child]);
        DownHeap(larger_child); // Recurse on the child's position
    }
}

// Resize the heap array when capacity is exceeded
// Doubles the capacity through dynamic allocation
void EmployeeHeap::ResizeArray() {
    int new_capacity = max_capacity * 2;
    EmployeeData** new_arr = new EmployeeData*[new_capacity];

    // Copy existing data (including nullptr at index 0)
    for (int i = 0; i <= data_num; i++) {
        new_arr[i] = heap_arr[i];
    }

    delete[] heap_arr;
    heap_arr = new_arr;
    max_capacity = new_capacity;
}
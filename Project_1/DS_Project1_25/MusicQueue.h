#pragma once
#include "MusicQueueNode.h"

// Implements a queue data structure for music tracks using a doubly linked list.
// It follows a First-In, First-Out (FIFO) principle.
// The queue has a maximum capacity of 100 elements.
class MusicQueue {
private:
	MusicQueueNode* head; // Pointer to the first element (front) of the queue.
	MusicQueueNode* rear; // Pointer to the last element (back) of the queue.
	int size;             // Current number of elements in the queue.
public:
	// Constructor: Initializes an empty queue.
	MusicQueue();
	// Destructor: Deletes all nodes to free memory.
	~MusicQueue();

	// Checks if the queue is empty. Returns true if size is 0.
	bool empty();
	// Checks if a specific song already exists in the queue.
	// Note: Returns true if the song does NOT exist, false if it does.
	bool exist(string artist,string title,string run_time);
	// Adds a new song to the rear of the queue.
	void push(string artist,string title,string run_time);
	// Removes the song from the front of the queue.
	void pop();
	// Returns a pointer to the song at the front of the queue without removing it.
	MusicQueueNode* front();
};
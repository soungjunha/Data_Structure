#pragma once
#include "MusicQueueNode.h"

class MusicQueue {
private:
	MusicQueueNode* head;
	MusicQueueNode* rear;
	int size = 0;
public:
	MusicQueue();
	~MusicQueue();

	bool empty();
	bool exist();
	void push();
	MusicQueueNode* pop();
	MusicQueueNode* front();
};
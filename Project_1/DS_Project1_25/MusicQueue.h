#pragma once
#include "MusicQueueNode.h"

class MusicQueue {
private:
	MusicQueueNode* head;
	MusicQueueNode* rear;
	int size;
public:
	MusicQueue();
	~MusicQueue();

	bool empty();
	bool exist(string artist,string title,string run_time);
	void push(string artist,string title,string run_time);
	void pop();
	MusicQueueNode* front();
};
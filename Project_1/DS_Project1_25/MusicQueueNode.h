#pragma once
//#include "MusicQueue.h"
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class MusicQueueNode {
private:
	string artist;
	string title;
	string run_time;

	MusicQueueNode* next;
	MusicQueueNode* prev;

public:
	MusicQueueNode(): artist(""),title(""),run_time(""),next(nullptr),prev(nullptr){}
	~MusicQueueNode() {
		delete next;
		delete prev;
	}

	void insert(string artist,string title,string run_time) {
		this->artist=artist;
		this->title=title;
		this->run_time=run_time;
	}

	bool exist() {
		return (artist!=""&&title!=""&&run_time!="")?true:false;
	}

	MusicQueueNode* get_next() const { return next; }
    MusicQueueNode* get_prev() const { return prev; }
    void set_next(MusicQueueNode* const node) { next = node; }
    void set_prev(MusicQueueNode* const node) { prev = node; }

    const string& get_artist() const { return artist; }
    const string& get_title() const { return title; }
    const string& get_run_time() const { return run_time; }

};
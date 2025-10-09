#pragma once

#include <string>

using namespace std;

class PlayListNode {
private:
	string artist;
	string title;
	int runtime_sec;

	PlayListNode* prev;
	PlayListNode* next;

public:
	PlayListNode(const string& artist,const string& title,const int& runtime):artist(artist),title(title),runtime_sec(runtime),prev(nullptr),next(nullptr){}
	~PlayListNode(){}
	
	void set(const string& artist,const string& title,const int& runtime) {
		this->artist=artist;
		this->title=title;
		this->runtime_sec=runtime;
	}
	
	string get_artist()const{return artist; }
	string get_title()const{return title; }
	int get_runtime()const{return runtime_sec; }
	PlayListNode* get_prev()const{return prev; }
	PlayListNode* get_next()const{return next; }

	void set_prev(PlayListNode* node) { prev = node; }
    void set_next(PlayListNode* node) { next = node; }


};
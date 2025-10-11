#pragma once

#include "PlayListNode.h"

class PlayList {
private:
	PlayListNode* head;
	PlayListNode* cursor;
	int count;
	int time;
	string data;

public:
	PlayList();
	~PlayList();
	void insert_node(const string& artist,const string& title,const int& runtime);
	void delete_node(bool only,const string& artist,const string& title);
	void delete_node(const string& data);
	bool empty();
	bool full();
	bool exist(const string& artist, const string& title);
	void print(ofstream& flog);
	string run_time();

	int get_count()const{return count; }

};
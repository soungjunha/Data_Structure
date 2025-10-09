#pragma once

#include "TitleBSTNode.h"
#include <utility>

using namespace std;

class TitleBST {
private:
	TitleBSTNode* root;
	TitleBSTNode* parent;
	string data;
	TitleBSTNode* target;

public:
	TitleBST();
	~TitleBST();

	void insert(const MusicQueueNode* node);
	vector<pair<string,string>> search(const string& title);
	void print(const string& title);
	void delete_node(const string& title);
	void clear(TitleBSTNode* node);
};
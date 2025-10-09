#pragma once

#include "TitleBSTNode.h"

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
	void print(ofstream& flog);
	void print(ofstream& flog,const TitleBSTNode* title);
	void delete_node(const string& title);
	void clear(TitleBSTNode* node);

	bool empty(){return root==nullptr; }
};
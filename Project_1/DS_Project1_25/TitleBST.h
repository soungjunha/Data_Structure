#pragma once

#include <vector>
#include <string>

using namespace std;

class TitleBSTNode;

class TitleBST {
private:
	TitleBSTNode* root;
	TitleBSTNode* parent;
	string data;
	TitleBSTNode* target;

public:
	TitleBST();
	~TitleBST();

	void insert();
	void search();
	void print();
	void delete_node();
};
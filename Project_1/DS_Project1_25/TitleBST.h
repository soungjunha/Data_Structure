#pragma once

#include "TitleBSTNode.h"

using namespace std;

class TitleBST {
private:
	TitleBSTNode* root;
	TitleBSTNode* parent;
	string data;
	TitleBSTNode* target;

	TitleBSTNode* delete_node_recursive(TitleBSTNode* node, const string& title);
	TitleBSTNode* find_min_node(TitleBSTNode* node);
	TitleBSTNode* find_node(const string& title);
	
	void find_first_song_by_artist(TitleBSTNode* node, const string& artist, bool& found, string& title_to_delete, TitleBSTNode*& node_to_modify);

	void clear(TitleBSTNode* node);

public:
	TitleBST();
	~TitleBST();

	void insert(const MusicQueueNode* node);
	vector<pair<string,string>> search(const string& title);
	void print(ofstream& flog);
	void print(ofstream& flog,const TitleBSTNode* title);
	void delete_node(const std::string& title);
	void delete_artist(const std::string& artist);
	
	void delete_song(const string& artist, const string& title);

	bool empty(){return root==nullptr; }
};
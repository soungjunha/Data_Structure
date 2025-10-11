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
	void collect_artist_songs(TitleBSTNode* node, const string& artist, vector<string>& titles_to_delete, vector<TitleBSTNode*>& nodes_to_modify);

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
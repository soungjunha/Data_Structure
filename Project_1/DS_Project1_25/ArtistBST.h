#pragma once

#include "ArtistBSTNode.h"

using namespace std;

class ArtistBST {
private:
	ArtistBSTNode* root;
	ArtistBSTNode* parent;
	string data;
	ArtistBSTNode* target;


public:
	ArtistBST();
	~ArtistBST();

	void insert(const MusicQueueNode* node);
	vector<pair<string,string>> search(const string& parameter);
	void print(ofstream& flog);
	void print(ofstream& flog,const ArtistBSTNode* node);
	void delete_node(const string& artist);
	void clear(ArtistBSTNode* node);

	bool empty(){return root==nullptr; }

};
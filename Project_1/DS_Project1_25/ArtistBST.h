#pragma once

#include "ArtistBSTNode.h"
#include <string>
#include <utility>

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
	void print(const string& artist);
	void delete_node(const string& artist);
	void clear(ArtistBSTNode* node);

};
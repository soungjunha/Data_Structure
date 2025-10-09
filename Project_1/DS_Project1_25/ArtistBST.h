#pragma once

#include "ArtistBSTNode.h"
#include <string>

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
	void search(const string& artist);
	void print(const string& artist);
	void delete_node(const string& artist);
	void clear(ArtistBSTNode* node);

};
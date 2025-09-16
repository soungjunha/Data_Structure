#pragma once

#include <string>

class ArtistBSTNode;

class ArtistBST {
private:
	ArtistBSTNode* root;
	ArtistBSTNode* parent;
	string data;
	ArtistBSTNode* target;


public:
	ArtistBST();
	~ArtistBST();

	void insert();
	void search();
	void print();
	void delete_node();

};
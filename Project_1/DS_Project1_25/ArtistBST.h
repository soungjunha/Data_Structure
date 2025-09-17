#pragma once

#include <string>

using namespace std;

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
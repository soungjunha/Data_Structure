#pragma once

#include "ArtistBSTNode.h"

using namespace std;

class ArtistBST {
private:
	ArtistBSTNode* root;
	ArtistBSTNode* parent;
	string data;
	ArtistBSTNode* target;

	ArtistBSTNode* delete_node_recursive(ArtistBSTNode* node, const string& artist);
	ArtistBSTNode* find_min_node(ArtistBSTNode* node);
	ArtistBSTNode* find_node(const std::string& artist);
	
	void find_first_artist_by_title(ArtistBSTNode* node, const string& title, bool& found, string& artist_to_delete, ArtistBSTNode*& node_to_modify);

	void clear(ArtistBSTNode* node);

public:
	ArtistBST();
	~ArtistBST();

	void insert(const MusicQueueNode* node);
	vector<pair<string,string>> search(const string& parameter);
	void print(ofstream& flog);
	void print(ofstream& flog,const ArtistBSTNode* node);
	void delete_node(const string& artist);
	void delete_title(const string& title);
	
	void delete_song(const string& artist, const string& title);

	bool empty(){return root==nullptr; }

};
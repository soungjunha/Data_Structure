#pragma once

#include "ArtistBSTNode.h"

using namespace std;

// Manages a Binary Search Tree of artists.
// This class provides functionality to insert, search, print, and delete
// artists or specific songs from the tree. The tree is ordered by artist name.
class ArtistBST {
private:
	ArtistBSTNode* root;      // Pointer to the root node of the BST.
	ArtistBSTNode* parent;    // (Unused) A pointer that could be used for parent tracking.
	string data;              // (Unused) A string that could be used for storing temporary data.
	ArtistBSTNode* target;    // A temporary pointer used for traversal and modification within methods.

	// Recursively deletes a node with a specific artist name.
	ArtistBSTNode* delete_node_recursive(ArtistBSTNode* node, const string& artist);

	// Finds the node with the minimum value (artist name) in a subtree.
	ArtistBSTNode* find_min_node(ArtistBSTNode* node);

	// Finds and returns a node with a specific artist name.
	ArtistBSTNode* find_node(const std::string& artist);
	
	// Helper function to find the first artist who has a song with the given title.
	// This function traverses the tree and uses reference parameters to return its findings.
	void find_first_artist_by_title(ArtistBSTNode* node, const string& title, bool& found, string& artist_to_delete, ArtistBSTNode*& node_to_modify);

	// Recursively clears all nodes in the tree to free memory.
	void clear(ArtistBSTNode* node);

public:
	// Default constructor. Initializes an empty BST.
	ArtistBST();
	// Destructor. Cleans up all dynamically allocated nodes.
	~ArtistBST();

	// Inserts a new song into the BST.
	// If the artist already exists, the song is added to that artist's song list.
	// Otherwise, a new node for the artist is created and inserted.
	void insert(const MusicQueueNode* node);

	// Searches for an artist and returns their list of songs.
	vector<pair<string,string>> search(const string& parameter);

	// Public method to print the contents of the tree to an output file stream.
	void print(ofstream& flog);

	// Private helper method to print the tree using in-order traversal.
	void print(ofstream& flog,const ArtistBSTNode* node);

	// Deletes an artist and all their songs from the tree.
	void delete_node(const string& artist);

	// Deletes all occurrences of a song by its title from any artist in the tree.
	void delete_title(const string& title);
	
	// Deletes a specific song by a specific artist.
	void delete_song(const string& artist, const string& title);

	// Checks if the BST is empty.
	bool empty(){return root==nullptr; }

};
#pragma once

#include "TitleBSTNode.h"

using namespace std;

// Manages a Binary Search Tree of songs, ordered by song title.
// This class provides functionality to insert, search, print, and delete
// songs or all songs by a specific artist from the tree.
class TitleBST {
private:
	TitleBSTNode* root;     // Pointer to the root node of the BST.
	TitleBSTNode* parent;   // (Unused) A pointer that could be used for parent tracking.
	string data;            // (Unused) A string that could be used for storing temporary data.
	TitleBSTNode* target;   // A temporary pointer used for traversal and modification within methods.

	// Recursively deletes a node with a specific song title.
	TitleBSTNode* delete_node_recursive(TitleBSTNode* node, const string& title);
	// Finds the node with the minimum value (song title) in a subtree.
	TitleBSTNode* find_min_node(TitleBSTNode* node);
	// Finds and returns a node with a specific song title.
	TitleBSTNode* find_node(const string& title);
	
	// Helper function to find the first song (by any title) released by a given artist.
	// This function traverses the tree and uses reference parameters to return its findings.
	void find_first_song_by_artist(TitleBSTNode* node, const string& artist, bool& found, string& title_to_delete, TitleBSTNode*& node_to_modify);

	// Recursively clears all nodes in the tree to free memory.
	void clear(TitleBSTNode* node);

public:
	// Default constructor. Initializes an empty BST.
	TitleBST();
	// Destructor. Cleans up all dynamically allocated nodes.
	~TitleBST();

	// Inserts a new song into the BST.
	// If the title already exists, the new artist is added to that title's node.
	// Otherwise, a new node for the title is created and inserted.
	void insert(const MusicQueueNode* node);

	// Searches for a song by title and returns a list of artists who released it.
	vector<pair<string,string>> search(const string& title);

	// Public method to print the contents of the tree to an output file stream.
	void print(ofstream& flog);

	// Private helper method to print the tree using in-order traversal.
	void print(ofstream& flog,const TitleBSTNode* title);

	// Deletes a title and all its associated artists from the tree.
	void delete_node(const std::string& title);

	// Deletes all songs by a specific artist from the entire tree.
	void delete_artist(const std::string& artist);
	
	// Deletes a specific song (a specific artist's version of a title).
	void delete_song(const string& artist, const string& title);

	// Checks if the BST is empty.
	bool empty(){return root==nullptr; }
};
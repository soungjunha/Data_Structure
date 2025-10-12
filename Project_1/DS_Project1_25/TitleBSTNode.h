#pragma once

#include "MusicQueueNode.h"

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <utility>

// Represents a single node in the TitleBST.
// Each node stores a song title and a collection of artists who have released a song
// with that title, along with their respective run times. It also contains pointers
// to left and right children in the BST.
class TitleBSTNode {
private:
	string title;                // The song's title.
	vector<string> artist;       // A vector containing the names of artists for this title.
	string run_time;             // (Unused) A string intended for run time.
	vector<int> rt;              // A vector storing the run time of each version in seconds.
	int count;                   // A counter for the number of artists added after the first.
	TitleBSTNode* left;          // Pointer to the left child node.
	TitleBSTNode* right;         // Pointer to the right child node.
public:
	// Constructor that initializes a node from a MusicQueueNode object.
	TitleBSTNode(const MusicQueueNode*& music): title(music->get_title()),count(0),left(nullptr),right(nullptr){
		// Add the first artist's name.
		this->artist.push_back(music->get_artist());
		string min,sec;
		// Find the position of ':' to parse the run time string.
		size_t pos=music->get_run_time().find(':');
		min = music->get_run_time().substr(0, pos);
		sec = music->get_run_time().substr(pos + 1);
		// Convert "MM:SS" format to total seconds and store it.
		rt.push_back(stoi(min)*60+stoi(sec));
	}
	// Destructor.
	~TitleBSTNode() {}

	// Adds a new artist and their song's run time to this title's node.
	// Throws an exception if the same artist is added again for this title.
	void set(const string& artist,const string& run_time) {
		if(search(artist)==-1){ // Check for duplicates.
			this->artist.push_back(artist);
			string min,sec;
			size_t pos=run_time.find(':');
			min = run_time.substr(0, pos);
			sec = run_time.substr(pos + 1);
			rt.push_back(stoi(min)*60+stoi(sec));
			count++;
		}
		else
		{
			// Throw an exception if the same artist is being added for this title.
			throw "same music";
		}
	}

	// Searches for an artist's name within this node.
	// Returns the index of the artist if found; otherwise, -1.
	int search(const string& artist) {
		auto it = find(this->artist.begin(), this->artist.end(), artist);

		if (it != this->artist.end()) {
			// Return the index of the found element.
			return distance(this->artist.begin(), it);
		} else {
			return -1; // Artist not found.
		}
	}

	// Removes an artist from this node's lists by name.
	void remove_artist(const std::string& artist_name) {
		int index = search(artist_name);
		if (index != -1) {
			// Erase the artist and corresponding run time from the vectors.
			this->artist.erase(this->artist.begin() + index);
			this->rt.erase(this->rt.begin() + index);
		}
	}

	// Copies artist and runtime data from another node.
	// This is used during the BST deletion process to replace a node's data.
	void copy_data_from(const TitleBSTNode* other) {
		this->title = other->title;
		this->artist = other->artist;
		this->rt = other->rt;
	}

	// --- Getters ---
	string get_title() const { return title; }
	vector<string> get_artist() const { return artist; }
	vector<int> get_rt() const { return rt; }
    TitleBSTNode* get_left() const { return left; }
    TitleBSTNode* get_right() const { return right; }
	
	// --- Setters ---
	void set_left(TitleBSTNode* node) { left = node; }
    void set_right(TitleBSTNode* node) { right = node; }

};
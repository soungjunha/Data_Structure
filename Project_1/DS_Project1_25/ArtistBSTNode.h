#pragma once

#include "MusicQueueNode.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <utility>


// Represents a single node in the ArtistBST.
// Each node stores an artist's name and a collection of their songs,
// including titles and run times. It also contains pointers to left and
// right children in the BST.
class ArtistBSTNode {
private:
	string artist;               // The artist's name.
	vector<string> title;        // A vector containing the titles of the artist's songs.
	string run_time;             // (Unused) A string intended for run time.
	vector<int> rt;              // A vector storing the run time of each song in seconds.
	int count;                   // A counter for the number of songs added after the first.
	ArtistBSTNode* left;         // Pointer to the left child node.
	ArtistBSTNode* right;        // Pointer to the right child node.

public:
	// Constructor that initializes a node from a MusicQueueNode object.
	ArtistBSTNode(const MusicQueueNode*& music): artist(music->get_artist()),count(0),left(nullptr),right(nullptr){
		// Add the first song's title.
		this->title.push_back(music->get_title());
		string min,sec;
		// Find the position of ':' to parse the run time string.
		size_t pos=music->get_run_time().find(':');
		min = music->get_run_time().substr(0, pos);
		sec = music->get_run_time().substr(pos + 1);
		// Convert "MM:SS" format to total seconds and store it.
		rt.push_back(stoi(min)*60+stoi(sec));
	}
	
	// Destructor.
	~ArtistBSTNode(){}

	// Adds a new song to this artist's list.
	// Throws an exception if the song title already exists for this artist.
	void set(const string& title,const string& run_time) {
		if(search(title)==-1){ // Check for duplicates.
			this->title.push_back(title);
			string min,sec;
			size_t pos=run_time.find(':');
			min = run_time.substr(0, pos);
			sec = run_time.substr(pos + 1);
			rt.push_back(stoi(min)*60+stoi(sec));
			count++;
		}
		else
		{
			// Throw an exception if the same music title is being added.
			throw "same music";
		}
	}

	// Searches for a song title within this node.
	// Returns the index of the title if found; otherwise, -1.
	int search(const string& title) {
		auto it = find(this->title.begin(), this->title.end(), title);

		if (it != this->title.end()) {
			// Return the index of the found element.
			return distance(this->title.begin(), it);
		} else {
			return -1; // Title not found.
		}
	}

	// Removes a song from this node's lists by its title.
	void remove_title(const std::string& title_name) {
		int index = search(title_name);
		if (index != -1) {
			// Erase the title and corresponding run time from the vectors.
			this->title.erase(this->title.begin() + index);
			this->rt.erase(this->rt.begin() + index);
		}
	}
	
	// Copies song data (artist, titles, and run times) from another node.
	// This is used during the BST deletion process to replace a node's data.
	void copy_data_from(const ArtistBSTNode* other) {
		this->artist = other->artist;
		this->title = other->title;
		this->rt = other->rt;
	}

	// --- Getters ---
	string get_artist() const { return artist; }
	vector<string> get_title() const { return title; }
	vector<int> get_rt() const { return rt; }
    ArtistBSTNode* get_left() const { return left; }
    ArtistBSTNode* get_right() const { return right; }

	// --- Setters ---
	void set_left(ArtistBSTNode* node) { left = node; }
    void set_right(ArtistBSTNode* node) { right = node; }
	
};
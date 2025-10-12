#pragma once

#include <string>
#include <fstream>

using namespace std;

// Represents a single node in the PlayList.
// This class acts as an element in a circular doubly linked list,
// storing song information and pointers to the next and previous nodes.
class PlayListNode {
private:
	string artist;      // The name of the artist.
	string title;       // The title of the song.
	int runtime_sec;    // The run time of the song in total seconds.

	PlayListNode* prev; // Pointer to the previous node in the list.
	PlayListNode* next; // Pointer to the next node in the list.

public:
	// Constructor: Initializes a node with the given song data.
	PlayListNode(const string& artist,const string& title,const int& runtime):artist(artist),title(title),runtime_sec(runtime),prev(nullptr),next(nullptr){}
	// Destructor.
	~PlayListNode(){}
	
	// Updates the data of an existing node.
	void set(const string& artist,const string& title,const int& runtime) {
		this->artist=artist;
		this->title=title;
		this->runtime_sec=runtime;
	}
	
	// --- Getters ---
	string get_artist()const{return artist; }
	string get_title()const{return title; }
	int get_runtime()const{return runtime_sec; }
	PlayListNode* get_prev()const{return prev; }
	PlayListNode* get_next()const{return next; }

	// --- Setters ---
	void set_prev(PlayListNode* node) { prev = node; }
    void set_next(PlayListNode* node) { next = node; }
};
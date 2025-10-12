#pragma once

#include <string>
#include <cstring>
#include <sstream>

using namespace std;

// Represents a single node in the MusicQueue.
// This class acts as an element in a doubly linked list,
// storing information about one song and pointers to the next and previous nodes.
class MusicQueueNode {
private:
	string artist;          // The name of the artist.
	string title;           // The title of the song.
	string run_time;        // The run time of the song in "MM:SS" format.

	MusicQueueNode* next;   // Pointer to the next node in the queue.
	MusicQueueNode* prev;   // Pointer to the previous node in the queue.

public:
	// Default constructor. Initializes an empty node.
	MusicQueueNode(): artist(""),title(""),run_time(""),next(nullptr),prev(nullptr){}
	
	// Parameterized constructor. Initializes a node with song data.
	// It throws an exception if any of the input strings are empty.
	MusicQueueNode(const string& artist,const string& title,const string& run_time): next(nullptr),prev(nullptr){
		if (exist(artist,title,run_time))
		{
			this->artist=artist;
			this->title=title;
			this->run_time=run_time;
		}
		else{
			throw "node is not exist";
		}
	}
	
	// Destructor.
	~MusicQueueNode() {
	}

	// Creates a new MusicQueueNode with the given data and links it
	// as the 'next' node to the current node.
	void insert(const string& artist,const string& title,const string& run_time) {
		try
		{
			next=new MusicQueueNode(artist,title,run_time);
		}
		catch(const char* errorMessage)
		{
			// Re-throw the exception from the MusicQueueNode constructor if creation fails.
			throw "node is not exist";
		}
		next->prev=this; // Set the back-pointer of the new node.
	}

	// A simple validation check to ensure that artist, title, and run_time are not empty strings.
	bool exist(const string& artist,const string& title,const string& run_time) {
		return (artist!=""&&title!=""&&run_time!="");
	}

	// --- Getters ---
	MusicQueueNode* get_next() const { return next; }
    MusicQueueNode* get_prev() const { return prev; }
    const string& get_artist() const { return artist; }
    const string& get_title() const { return title; }
    const string& get_run_time() const { return run_time; }

	// --- Setters ---
    void set_next(MusicQueueNode* const node) { next = node; }
    void set_prev(MusicQueueNode* const node) { prev = node; }
};
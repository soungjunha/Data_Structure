#pragma once

#include "PlayListNode.h"

// Manages a playlist of songs using a circular doubly linked list.
// The playlist has a maximum capacity of 10 songs.
// It provides functionalities to insert, delete, and print songs.
class PlayList {
private:
	PlayListNode* head;   // Pointer to the first node in the list.
	PlayListNode* cursor; // A movable pointer used for traversal and operations.
	int count;            // The current number of songs in the playlist.
	int time;             // The total runtime of all songs in the playlist, in seconds.
	string data;          // (Unused) A string that could be used for temporary data.

public:
	// Constructor: Initializes an empty playlist.
	PlayList();
	// Destructor: Deletes all nodes in the list to free memory.
	~PlayList();

	// Inserts a new song into the playlist.
	void insert_node(const string& artist,const string& title,const int& runtime);
	
	// Overloaded delete function.
	// Deletes a specific song from the playlist.
	// The 'only' flag determines behavior if the song is not found.
	void delete_node(bool only,const string& artist,const string& title);
	
	// Overloaded delete function.
	// Deletes all songs where the artist or title matches the given data string.
	void delete_node(const string& data);

	// Checks if the playlist is empty.
	bool empty();
	// Checks if the playlist is full (has reached its capacity of 10).
	bool full();
	// Checks if a specific song already exists in the playlist.
	// Note: Returns true if the song can be added (does not exist), false otherwise.
	bool exist(const string& artist, const string& title);
	
	// Prints the entire content of the playlist to the log file.
	void print(ofstream& flog);
	
	// Formats the total runtime into a "Xmin Ysec" string.
	string run_time();

	// Getter for the current song count.
	int get_count()const{return count; }
};
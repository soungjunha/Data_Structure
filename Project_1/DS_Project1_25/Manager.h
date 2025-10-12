#pragma once
#include "MusicQueue.h"
#include "TitleBST.h"
#include "ArtistBST.h"
#include "PlayList.h"
#include <fstream>

using namespace std;

// The Manager class is the main controller for the music management application.
// It handles reading commands from a file, executing them, and writing output to a log file.
// It orchestrates the interactions between the various data structures:
// MusicQueue, ArtistBST, TitleBST, and PlayList.
class Manager {
private:
	MusicQueue q;       // A queue to temporarily store music loaded from the file.
	ArtistBST ab;       // A Binary Search Tree to store music, sorted by artist name.
	TitleBST tb;        // A Binary Search Tree to store music, sorted by song title.
	PlayList pl;        // A data structure representing the user's playlist.
	ifstream fcmd;      // Input file stream for reading command files.
	ofstream flog;      // Output file stream for writing log messages.
public:
	// Constructor: Initializes the Manager and opens the log file.
	Manager();
	// Destructor: Cleans up resources, primarily by closing file streams.
	~Manager();

	// The main execution loop of the program. Reads and processes commands from the given file.
	void run(const char* command);

	// Loads music data from "Music_List.txt" into the MusicQueue.
	void LOAD();
	// Adds a single new song to the MusicQueue based on the provided parameters.
	void ADD(const string& parameter);
	// Pops all music from the MusicQueue and inserts them into both the ArtistBST and TitleBST.
	void QPOP();
	// Searches for music in the BSTs by artist, title, or a specific song.
	void SEARCH(const string& parameter);
	// Creates a playlist by adding search results to the PlayList data structure.
	void MAKEPL(const string& parameter);
	// Prints the contents of one of the data structures (ArtistBST, TitleBST, or PlayList) to the log.
	void PRINT(const string& parameter);
	// Deletes music from all relevant data structures based on the criteria (artist, title, song, etc.).
	void DELETE(const string& parameter);
	// Exits the program cleanly, closing all file streams.
	void EXIT();

};
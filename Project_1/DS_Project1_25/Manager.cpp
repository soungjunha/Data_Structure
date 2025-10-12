#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

// Constructor: Opens the log file for writing.
Manager::Manager() {
	flog.open("log.txt");
}

// Destructor: Currently empty, but would handle any necessary cleanup.
// File streams are automatically closed when their objects are destroyed.
Manager::~Manager() {
	
}

// Main execution function. It reads the command file line by line and
// calls the appropriate handler function for each command.
void Manager::run(const char* command) {
	fcmd.open(command);

	// Check if the command file was successfully opened.
	if (!fcmd.is_open())
	{
		cout<<"not found command file"<<endl;
	}
	else
	{
		string line;
		// Read the command file one line at a time.
		while (getline(fcmd, line))
		{
			string keyword,parameter;
			// Find the first space to separate the command keyword from its parameters.
			size_t pos=line.find(' ');

			if (pos != string::npos) {
				keyword = line.substr(0, pos);
				parameter = line.substr(pos + 1);
			} else {
				// Handle commands with no parameters.
				keyword = line;
				parameter = "";
			}
			
			// Call the corresponding function based on the command keyword.
			if (keyword=="LOAD")
			{
				LOAD();
			}
			else if (keyword=="ADD")
			{
				ADD(parameter);
			}
			else if (keyword=="QPOP")
			{
				QPOP();
			}
			else if (keyword=="SEARCH")
			{
				SEARCH(parameter);
			}
			else if (keyword=="MAKEPL")
			{
				MAKEPL(parameter);
			}
			else if (keyword=="PRINT")
			{
				PRINT(parameter);
			}
			else if (keyword=="DELETE")
			{
				DELETE(parameter);
			}
			else if (keyword=="EXIT")
			{
				EXIT();
				break; // Stop processing after EXIT command.
			}
			else
			{
				// Log an error for any unrecognized command.
				flog<<"========ERROR========"<<endl;
				flog<<"1000"<<endl;
				flog<<"======================"<<endl;
			}
			
		}
	}
}

// Loads music data from "Music_List.txt" into the music queue.
void Manager::LOAD() {
	ifstream music_list("Music_List.txt");
	// Error if the queue is not empty or the music file cannot be opened.
	if (!q.empty()||!music_list.is_open())
	{
		flog<<"========ERROR========"<<endl;
		flog<<"100"<<endl;
		flog<<"======================"<<endl;
	}
	else
	{
		string line;
		flog<<"========LOAD========"<<endl;
		// Read each line from the music file.
		while (getline(music_list, line))
		{
			string token;
			vector<string> tokens;
			stringstream ss(line);

			// Split the line by the '|' delimiter.
			while (getline(ss, token, '|')) {
        		tokens.push_back(token);
    		}
			// Push the parsed song data into the queue.
			q.push(tokens[0],tokens[1],tokens[2]);
			flog<<tokens[0]<<'/'<<tokens[1]<<'/'<<tokens[2]<<endl;
		}
		flog<<"======================"<<endl;
	}
	
}

// Adds a single song to the music queue.
void Manager::ADD(const string& parameter) {
	string token;
	vector<string> tokens;
	stringstream ss(parameter);

	// Parse the parameter string by the '|' delimiter.
	while (getline(ss, token, '|')) {
		tokens.push_back(token);
	}
	// A valid ADD command must have exactly 3 parts: artist, title, runtime.
	if(tokens.size()!=3){
		flog<<"========ERROR========"<<endl;
		flog<<"200"<<endl;
		flog<<"======================"<<endl;
		return;
	}
	try
	{
		// Push the new song data into the queue.
		q.push(tokens[0],tokens[1],tokens[2]);
		flog<<"========ADD========"<<endl;
		flog<<tokens[0]<<'/'<<tokens[1]<<'/'<<tokens[2]<<endl;
		flog<<"======================"<<endl;
	}
	catch(const char* errorMessage)
	{
		// Catch any errors during the push operation (e.g., validation).
		flog<<"========ERROR========"<<endl;
		flog<<"200"<<endl;
		flog<<"======================"<<endl;
	}
	
}

// Moves all songs from the queue into the Artist and Title BSTs.
void Manager::QPOP() {
	// Error if the queue is already empty.
	if (q.empty())
	{
		flog<<"========ERROR========"<<endl;
		flog<<"300"<<endl;
		flog<<"======================"<<endl;
		return;
	}
	else
	{
		// Loop until the queue is empty.
		while (!q.empty())
		{
			try
			{
				// Insert the song at the front of the queue into both BSTs.
				ab.insert(q.front());
				tb.insert(q.front());
			}
			catch(const char* errorMessage)
			{
				// Catch insertion errors (e.g., duplicate songs).
				flog<<"========ERROR========"<<endl;
				flog<<"300"<<endl;
				flog<<"======================"<<endl;
			}
			// Remove the song from the queue.
			q.pop();
		}
		flog<<"========QPOP========"<<endl;
		flog<<"Success"<<endl;
		flog<<"======================"<<endl;
	}
	
}

// Searches for songs based on different criteria.
void Manager::SEARCH(const string& parameter) {
	try {
		vector<pair<string,string>>res_vec;
        stringstream ss(parameter);
        string type;

        ss >> type; // Read the search type (ARTIST, TITLE, SONG).

        if (type == "ARTIST" || type == "TITLE") {
            string arg;
			
            ss >> ws; // Skip whitespace.
            getline(ss, arg); // Read the rest of the line as the argument.

            if (arg.empty()) {
                throw "400";
            }

			// Call the appropriate search function based on the type.
            if (type == "ARTIST") {
                res_vec=ab.search(arg);
            }
			else {
                res_vec=tb.search(arg);
            }

			// Log the search results.
			flog << "========SEARCH========" << endl;
			for (int i = 0; i < res_vec.size(); i++)
			{
				if(type == "ARTIST")
				{
					flog<<arg+'/'+res_vec[i].first+'/'+res_vec[i].second<<endl;
				}
				else
				{
					flog<<res_vec[i].first+'/'+arg+'/'+res_vec[i].second<<endl;
				}
			}
			flog << "======================" << endl;

        }
        else if (type == "SONG") {
            string title, artist;
            string remain;

            ss >> ws;
            getline(ss, remain); // Get the "artist|title" part.

            stringstream songStream(remain);
            
			// Parse the artist and title.
            if (getline(songStream, artist, '|') && getline(songStream, title)) {
                if (title.empty() || artist.empty()) {
                    throw "400";
                }
                res_vec=ab.search(artist); // Search for the artist first.
				// Then, find the specific song title in the results.
				auto song = find_if(res_vec.begin(), res_vec.end(), [&](const pair<string, string>& p) {return p.first == title;});
				
				if (song != res_vec.end()) {
					flog << "========SEARCH========" << endl;
					flog << artist << '/' << title << '/' << (*song).second << endl;
					flog << "======================" << endl;
				} else {
					throw "Song not found";
				}
            }
			else {
				// Handle malformed SONG search parameter.
				throw "400";
            }
        }
        else {
			// Handle invalid search type.
            throw "400";
        }
    }
    catch (const char* errorMessage) {
		// Catch any exceptions during search (e.g., not found).
        flog << "========ERROR========" << endl;
		flog << "400" << endl;
		flog << "======================" << endl;
	}
}

// Adds songs to the playlist.
void Manager::MAKEPL(const string& parameter) {
	try
	{
		vector<pair<string,string>>res_vec;
        stringstream ss(parameter);
        string type;

        ss >> type; // Read the type (ARTIST, TITLE, SONG).

        if (type == "ARTIST" || type == "TITLE") {
            string arg;
			
            ss >> ws;
            getline(ss, arg);

            if (arg.empty()) {
                throw "500";
            }

            if (type == "ARTIST") {
                res_vec=ab.search(arg);
				// Check if there's enough space in the playlist.
				if (res_vec.size()<(10-pl.get_count()))
				{
					// Add all songs by the artist to the playlist.
					for (const pair<string,string>& music:res_vec)
					{
						string min,sec;
						size_t pos=music.second.find(':');
						min = music.second.substr(0, pos);
						sec = music.second.substr(pos + 1);
						pl.insert_node(arg,music.first,stoi(min)*60+stoi(sec));
					}
				}
				else
				{
					throw "500"; // Not enough space.
				}
				
            }
			else { // TITLE
                res_vec=tb.search(arg);
				// Check for space.
				if (res_vec.size()<(10-pl.get_count()))
				{
					// Add all songs with that title to the playlist.
					for (const pair<string,string>& music:res_vec)
					{
						string min,sec;
						size_t pos=music.second.find(':');
						min = music.second.substr(0, pos);
						sec = music.second.substr(pos + 1);
						pl.insert_node(music.first,arg,stoi(min)*60+stoi(sec));
					}
				}
				else
				{
					throw "500";
				}
            }
        }
        else if (type == "SONG") {
            string title, artist;
            string remain;

            ss >> ws;
            getline(ss, remain);

            stringstream songStream(remain);
            
            if (getline(songStream, artist, '|') && getline(songStream, title)) {
                if (title.empty() || artist.empty()) {
                    throw "500";
                }
				// Find the specific song.
                res_vec=ab.search(artist);
				auto song = find_if(res_vec.begin(), res_vec.end(), [&](const pair<string, string>& p) {return p.first == title;});
				
				if (song != res_vec.end()) {
					// Add the found song to the playlist.
					string min,sec;
					size_t pos=(*song).second.find(':');
					min = (*song).second.substr(0, pos);
					sec = (*song).second.substr(pos + 1);
					pl.insert_node(artist,title,stoi(min)*60+stoi(sec));
				} 
				else {
					throw "Song not found";
				}
            }
			else {
				throw "500";
            }
        }
        else {
            throw "500"; // Invalid type.
        }

		// Print the updated playlist to the log.
		flog << "========MAKEPL========" << endl;
		pl.print(flog);
   		flog << "======================" << endl;
		
	}
	catch (const char* errorMessage) {
		// Catch any errors during the process.
        flog << "========ERROR========" << endl;
		flog << "500" << endl;
		flog << "======================" << endl;
	}
	
}

// Prints the content of a specified data structure.
void Manager::PRINT(const string& parameter) {
	try
	{
		if (parameter=="ARTIST")
		{
			ab.print(flog);
		}
		else if (parameter=="TITLE")
		{
			tb.print(flog);
		}
		else if (parameter=="LIST")
		{
			if (!pl.empty())
			{
				flog << "========Print========" << endl;
				pl.print(flog);
				flog << "======================" << endl;
			}
			else {
				// Error if trying to print an empty playlist.
				throw "600";
            }
			
		}
		else
		{
			// Error for invalid parameter.
			throw "600";
		}
		
		
	}
	catch (const char* errorMessage) {
        flog << "========ERROR========" << endl;
		flog << "600" << endl;
		flog << "======================" << endl;
	}
	
}

// Deletes songs based on various criteria from all relevant data structures.
void Manager::DELETE(const string& parameter) {
	try
	{
        stringstream ss(parameter);
        string type;

        ss >> type; // Get the deletion type.

        if (type == "ARTIST" || type == "TITLE") {
            string arg;
			
            ss >> ws;
            getline(ss, arg);

            if (arg.empty()) {
                throw "700";
            }

            if (type == "ARTIST") {
				// Delete the artist from all data structures.
				tb.delete_artist(arg);
				ab.delete_node(arg);
				pl.delete_node(arg);
            }
			else { // TITLE
                // Delete all occurrences of the title from all data structures.
                tb.delete_node(arg);
				ab.delete_title(arg);
				pl.delete_node(arg);
            }

        }
        else if (type == "SONG") {
            string title, artist;
            string remain;

            ss >> ws;
            getline(ss, remain);

            stringstream songStream(remain);
            
            if (getline(songStream, artist, '|') && getline(songStream, title)) {
                if (title.empty() || artist.empty()) {
                    throw "700";
                }
				// Delete a specific song from all data structures.
                tb.delete_song(artist,title);
				ab.delete_song(artist,title);
				pl.delete_node(false,artist,title);
            }
			else {
				throw "700";
            }
        }
		else if (type == "LIST") {
            string title, artist;
            string remain;

            ss >> ws;
            getline(ss, remain);

            stringstream songStream(remain);
            
            if (getline(songStream, artist, '|') && getline(songStream, title)) {
                if (title.empty() || artist.empty()) {
                    throw "700";
                }
				// Delete a specific song only from the playlist.
                pl.delete_node(true,artist,title);
            }
			else {
				throw "700";
            }
        }
        else {
            throw "700"; // Invalid type.
        }

		flog << "========DELETE========" << endl;
		flog << "Success" << endl;
   		flog << "======================" << endl;
		
	}
	catch (const char* errorMessage) {
		// Catch any errors during deletion.
        flog << "========ERROR========" << endl;
		flog << "700" << endl;
		flog << "======================" << endl;
	}
}

// Handles the EXIT command, closing files and terminating the program.
void Manager::EXIT() {
	flog<<"========EXIT========"<<endl;
	flog<<"Success"<<endl;
	flog<<"======================"<<endl;
	fcmd.close();
	flog.close();
}
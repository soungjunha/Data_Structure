#pragma once

#include "MusicQueueNode.h"
#include <vector>
#include <algorithm>
#include <iterator>

class ArtistBSTNode {
private:
	string artist;
	vector<string> title;
	string run_time;
	vector<int> rt;
	int count; 
	ArtistBSTNode* left;
	ArtistBSTNode* right;

public:
	ArtistBSTNode(const MusicQueueNode*& music): artist(music->get_artist()),count(0),left(nullptr),right(nullptr){
		this->title.push_back(music->get_title());
		string min,sec;
		size_t pos=music->get_run_time().find(':');
		min = music->get_run_time().substr(0, pos);
		sec = music->get_run_time().substr(pos + 1);
		rt.push_back(stoi(min)*60+stoi(sec));
	}
	~ArtistBSTNode(){}

	void set(const string& title,const string& run_time) {
		if(search(title)==-1){
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
			throw "same music";
		}
	}

	int search(const string& title) {

		auto it = find(this->title.begin(), this->title.end(), title);

		if (it != this->title.end()) {
			return distance(this->title.begin(), it);
		} else {
			return -1;
		}
	}

	string get_artist() const { return artist; }
    ArtistBSTNode* get_left() const { return left; }
    ArtistBSTNode* get_right() const { return right; }

	void set_left(ArtistBSTNode* node) { left = node; }
    void set_right(ArtistBSTNode* node) { right = node; }
	
};
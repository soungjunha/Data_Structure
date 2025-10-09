#pragma once

#include "MusicQueueNode.h"

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <utility>

class TitleBSTNode {
private:
	string title;
	vector<string> artist;
	string run_time;
	vector<int> rt;
	int count;
	TitleBSTNode* left;
	TitleBSTNode* right;
public:
	TitleBSTNode(const MusicQueueNode*& music): title(music->get_title()),count(0),left(nullptr),right(nullptr){
		this->artist.push_back(music->get_artist());
		string min,sec;
		size_t pos=music->get_run_time().find(':');
		min = music->get_run_time().substr(0, pos);
		sec = music->get_run_time().substr(pos + 1);
		rt.push_back(stoi(min)*60+stoi(sec));
	}
	~TitleBSTNode() {}

	void set(const string& artist,const string& run_time) {
		if(search(artist)==-1){
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
			throw "same music";
		}
	}

	int search(const string& artist) {

		auto it = find(this->artist.begin(), this->artist.end(), artist);

		if (it != this->artist.end()) {
			return distance(this->artist.begin(), it);
		} else {
			return -1;
		}
	}

	string get_title() const { return title; }
	vector<string> get_artist() const { return artist; }
	vector<int> get_rt() const { return rt; }

    TitleBSTNode* get_left() const { return left; }
    TitleBSTNode* get_right() const { return right; }
	
	void set_left(TitleBSTNode* node) { left = node; }
    void set_right(TitleBSTNode* node) { right = node; }

};
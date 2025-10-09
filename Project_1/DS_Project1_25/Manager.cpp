#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

Manager::Manager() {
	flog.open("log.txt");
}

Manager::~Manager() {
	fcmd.close();
	flog.close();
}

void Manager::run(const char* command) {
	fcmd.open(command);

	if (!fcmd.is_open())
	{
		cout<<"not found command file"<<endl;
	}
	else
	{

		string line;
		while (getline(fcmd, line))
		{
			string keyword,parameter;
			size_t pos=line.find(' ');

			if (pos != string::npos) {
				keyword = line.substr(0, pos);
				parameter = line.substr(pos + 1);
			} else {
				keyword = line;
				parameter = "";
			}
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
				PRINT();
			}
			else if (keyword=="DELETE")
			{
				DELETE();
			}
			else if (keyword=="EXIT")
			{
				EXIT();
			}
			else
			{
				flog<<"========ERROR========"<<endl;
				flog<<"1000"<<endl;
				flog<<"======================"<<endl;
			}
			
		}
	}
}

void Manager::LOAD() {
	ifstream music_list("Music_List.txt");
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
		while (getline(music_list, line))
		{
			string token;
			vector<string> tokens;
			stringstream ss(line);

			while (getline(ss, token, '|')) {
        		tokens.push_back(token);
    		}
			q.push(tokens[0],tokens[1],tokens[2]);
			flog<<tokens[0]<<'/'<<tokens[1]<<'/'<<tokens[2]<<endl;
		}
		flog<<"======================"<<endl;
	}
	
}

void Manager::ADD(const string& parameter) {
	string token;
	vector<string> tokens;
	stringstream ss(parameter);

	while (getline(ss, token, '|')) {
		tokens.push_back(token);
	}
	if(tokens.size()!=3){
		flog<<"========ERROR========"<<endl;
		flog<<"200"<<endl;
		flog<<"======================"<<endl;
		return;
	}
	try
	{
		q.push(tokens[0],tokens[1],tokens[2]);
		flog<<"========ADD========"<<endl;
		flog<<tokens[0]<<'/'<<tokens[1]<<'/'<<tokens[2]<<endl;
		flog<<"======================"<<endl;
	}
	catch(const char* errorMessage)
	{
		flog<<"========ERROR========"<<endl;
		flog<<"200"<<endl;
		flog<<"======================"<<endl;
	}
	
}

void Manager::QPOP() {
	if (q.empty())
	{
		flog<<"========ERROR========"<<endl;
		flog<<"300"<<endl;
		flog<<"======================"<<endl;
		return;
	}
	else
	{
		while (!q.empty())
		{
			try
			{
				ab.insert(q.front());
				tb.insert(q.front());
			}
			catch(const char* errorMessage)
			{
				flog<<"========ERROR========"<<endl;
				flog<<"300"<<endl;
				flog<<"======================"<<endl;
			}
			q.pop();
		}
		flog<<"========QPOP========"<<endl;
		flog<<"Success"<<endl;
		flog<<"======================"<<endl;
	}
	
}

void Manager::SEARCH(const string& parameter) {
	try {
		vector<pair<string,string>>res_vec;
        stringstream ss(parameter);
        string command;

        ss >> command;

        if (command == "ARTIST" || command == "TITLE") {
            string arg;
			
            ss >> ws;
            getline(ss, arg);

            if (arg.empty()) {
                flog << "========ERROR========" << endl;
				flog << "400" << endl;
				flog << "======================" << endl;
                return;
            }

            if (command == "ARTIST") {
                res_vec=ab.search(arg);
            }
			else {
                res_vec=tb.search(arg);
            }

			flog << "========SEARCH========" << endl;
			for (int i = 0; i < res_vec.size(); i++)
			{
				if(command == "ARTIST")
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
        else if (command == "SONG") {
            string title, artist;
            string remain;

            ss >> ws;
            getline(ss, remain);

            stringstream songStream(remain);
            
            if (getline(songStream, artist, '|') && getline(songStream, title)) {
                if (title.empty() || artist.empty()) {
                    flog << "========ERROR========" << endl;
					flog << "400" << endl;
					flog << "======================" << endl;
                    return;
                }
                res_vec=ab.search(artist);
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
				flog << "========ERROR========" << endl;
				flog << "400" << endl;
				flog << "======================" << endl;
                return;
            }
        }
        else {
            flog << "========ERROR========" << endl;
			flog << "400" << endl;
			flog << "======================" << endl;
            return;
        }
    }
    catch (const char* errorMessage) {
        flog << "========ERROR========" << endl;
		flog << "400" << endl;
		flog << "======================" << endl;
	}
	

}

void Manager::MAKEPL(const string& parameter) {
	try
	{
		vector<pair<string,string>>res_vec;
        stringstream ss(parameter);
        string command;

        ss >> command;

        if (command == "ARTIST" || command == "TITLE") {
            string arg;
			
            ss >> ws;
            getline(ss, arg);

            if (arg.empty()) {
                throw "500";
            }

            if (command == "ARTIST") {
                res_vec=ab.search(arg);
				if (res_vec.size()<(10-pl.get_count()))
				{
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
					throw "500";
				}
				
            }
			else {
                res_vec=tb.search(arg);
				if (res_vec.size()<(10-pl.get_count()))
				{
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
        else if (command == "SONG") {
            string title, artist;
            string remain;

            ss >> ws;
            getline(ss, remain);

            stringstream songStream(remain);
            
            if (getline(songStream, artist, '|') && getline(songStream, title)) {
                if (title.empty() || artist.empty()) {
                    throw "500";
                }
                res_vec=ab.search(artist);
				auto song = find_if(res_vec.begin(), res_vec.end(), [&](const pair<string, string>& p) {return p.first == title;});
				
				if (song != res_vec.end()) {
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
            throw "500";
        }

		flog << "========MAKEPL========" << endl;
		pl.print(flog);
   		flog << "======================" << endl;
		
	}
	catch (const char* errorMessage) {
        flog << "========ERROR========" << endl;
		flog << "500" << endl;
		flog << errorMessage << endl;
		flog << "======================" << endl;
	}
	
}

void Manager::PRINT() {
	
}

void Manager::DELETE() {
	
}

void Manager::EXIT() {

}
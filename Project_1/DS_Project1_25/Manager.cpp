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
				SEARCH();
			}
			else if (keyword=="MAKEPL")
			{
				MAKEPL();
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
	
}

void Manager::MAKEPL() {
	
}

void Manager::PRINT() {
	
}

void Manager::DELETE() {
	
}

void Manager::EXIT() {

}
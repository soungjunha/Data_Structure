#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

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
				ADD();
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
	
}

void Manager::ADD() {
	
}

void Manager::QPOP() {
	
}

void Manager::SEARCH() {
	
}

void Manager::MAKEPL() {
	
}

void Manager::PRINT() {
	
}

void Manager::DELETE() {
	
}

void Manager::EXIT() {

}
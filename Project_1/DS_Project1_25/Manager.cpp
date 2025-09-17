#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

Manager::Manager() {

}

Manager::~Manager() {

}

void Manager::run(const char* command) {
	ifstream command_file(command);

	if (!command_file.is_open())
	{
		cout<<"not found file"<<endl;
	}

	string line;
	while (getline(command_file, line))
	{
		string part_1,part_2;
		size_t pos=line.find(' ');

		if (pos != string::npos) {
        	part_1 = line.substr(0, pos);
        	part_2 = line.substr(pos + 1);
    	} else {
        	part_1 = line;
        	part_2 = "";
    	}

		cout<<part_1<<endl;

	}

	command_file.close();

}

void Manager::LOAD() {
	
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
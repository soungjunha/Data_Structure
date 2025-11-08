#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
#include <fstream>
#include <string> // string을 사용하기 위해 추가

using namespace std; // string을 사용하기 위해 추가 (권장)

class Manager {
private:
	char* cmd;
	BpTree* bptree;
	SelectionTree* stree;

public:
	Manager(int bpOrder) {
		// (이전 코드에서 구현)
		flog.open("log.txt", std::ios::trunc); 
		bptree = new BpTree(&flog, bpOrder);
		stree = new SelectionTree(&flog);
	}


	~Manager() {
		// (이전 코드에서 구현)
		delete bptree;
		delete stree;
		if (fin.is_open()) fin.close();
		if (flog.is_open()) flog.close();
	}

	ifstream fin;
	ofstream flog;

	void run(const char* command);
	void LOAD();

	// [수정 1] 인자를 받도록 변경
	void ADD_BP(string name, int dept_no, int id, int income);

	void SEARCH_BP_NAME(string name);
	void SEARCH_BP_RANGE(string start, string end);
	void PRINT_BP();
	void ADD_ST_DEPTNO(int dept_no);
	void ADD_ST_NAME(string name);

	// [수정 2] 인자를 받도록 변경
	void PRINT_ST(int dept_no);
	
	void DELETE();

	void printErrorCode(int n);
	void printSuccessCode(string success);
};
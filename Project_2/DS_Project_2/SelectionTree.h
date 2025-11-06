#pragma once
#include "SelectionTreeNode.h"
#include <fstream> // Need this for ofstream

class SelectionTree {
private:
    SelectionTreeNode* root;
    ofstream* fout;
    SelectionTreeNode* run[8]; // [0] = 100, [1] = 200, ... [7] = 800

    int getRunIndex(int dept_no) {return (dept_no / 100) - 1;}
    void updateTree(SelectionTreeNode* node);

public:
    SelectionTree(std::ofstream* fout) {
        this->fout = fout;
        this->root = nullptr;

        // 1. 8개의 리프 노드(run)를 생성하고 각 노드에 Heap을 생성합니다.
        for (int i = 0; i < 8; i++) {
            run[i] = new SelectionTreeNode();
            run[i]->HeapInit(); // 각 리프 노드가 EmployeeHeap을 갖도록 함
        }
        
        // 2. 리프 노드를 기반으로 상위 트리 구조를 구축합니다.
        setTree();
    }

    ~SelectionTree() {
        // TODO: 모든 노드(run[8] 및 상위 노드)를 재귀적으로 삭제해야 합니다.
        // delete root; (재귀 삭제 구현 필요)
        for (int i = 0; i < 8; i++) {
            delete run[i]; // 각 리프 노드(와 그 안의 Heap) 삭제
        }
        // ... (내부 노드들도 삭제해야 함)
    }

    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    SelectionTreeNode* getRoot() { return root; }

    void setTree();

    bool Insert(EmployeeData* newData);
    bool Delete();
    bool printEmployeeData(int dept_no);

    // Private helper functions (SelectionTree.cpp에 구현)
private:
    
};
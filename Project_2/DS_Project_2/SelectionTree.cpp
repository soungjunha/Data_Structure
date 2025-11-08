#include "SelectionTree.h"
#include <vector>
#include <algorithm>
#include <string>

void SelectionTree::setTree() {
    std::vector<SelectionTreeNode*> currentLevel;
    // 1. 레벨 0: 8개의 리프 노드 (각 노드는 힙을 소유함)
    for (int i = 0; i < 8; i++) {
        currentLevel.push_back(run[i]);
    }

    // 2. 레벨 0 -> 레벨 1 -> ... -> 루트 까지 구축
    while (currentLevel.size() > 1) {
        std::vector<SelectionTreeNode*> parentLevel;
        
        // 3. 두 개씩 짝지어 부모 노드 생성
        for (size_t i = 0; i < currentLevel.size(); i += 2) {
            SelectionTreeNode* parent = new SelectionTreeNode();
            SelectionTreeNode* leftChild = currentLevel[i];
            SelectionTreeNode* rightChild = currentLevel[i + 1];

            parent->setLeftChild(leftChild);
            parent->setRightChild(rightChild);
            leftChild->setParent(parent);
            rightChild->setParent(parent);

            parentLevel.push_back(parent);
        }
        currentLevel = parentLevel;
    }

    // 5. 마지막 남은 노드가 루트
    this->setRoot(currentLevel[0]);
}

bool SelectionTree::Insert(EmployeeData* newData) {
    int dept = newData->getDeptNo();
    int index = getRunIndex(dept);

    if (index < 0 || index > 7) {
        return false; // (Error 500)
    }

    // 1. 해당 부서의 리프 노드 및 힙에 접근
    SelectionTreeNode* leaf = run[index];
    EmployeeHeap* heap = leaf->getHeap();

    // 2. 힙에 데이터 삽입 (연봉 기준 Max Heap)
    heap->Insert(newData);

    // 3. 힙의 새로운 승자(Top)를 리프 노드의 데이터로 설정
    leaf->setEmployeeData(heap->Top());

    // 4. 변경 사항을 루트까지 전파(업데이트) (명세서 요구사항) [cite: 68, 80]
    updateTree(leaf);
    
    return true;
}

bool SelectionTree::Delete() {
    EmployeeData* winner = root->getEmployeeData();

    // 1. 트리가 비어있는 경우 (Error 700)
    if (winner == nullptr) {
        return false;
    }

    // 2. 최고 연봉자의 부서(dept)를 찾아 해당 힙에 접근
    int dept = winner->getDeptNo();
    int index = getRunIndex(dept);

    SelectionTreeNode* leaf = run[index];
    EmployeeHeap* heap = leaf->getHeap();

    // 3. 해당 힙에서 루트(최고 연봉자)를 삭제 (연봉 기준)
    heap->Delete();

    // 4. 힙의 *새로운* 승자(Top)를 리프 노드의 데이터로 설정
    leaf->setEmployeeData(heap->Top());

    // 5. 변경 사항을 루트까지 전파(업데이트) [cite: 80, 107]
    updateTree(leaf);

    return true;
}

bool SelectionTree::printEmployeeData(int dept_no) {
    int index = getRunIndex(dept_no);
    if (index < 0 || index > 7) {
        return false; // 잘못된 부서 (Error 600)
    }

    EmployeeHeap* heap = run[index]->getHeap();
    if (heap == nullptr || heap->IsEmpty()) {
        return false; // 힙이 없거나 비어있음 (Error 600)
    }

    // EmployeeHeap.h에 추가한 getter 함수들 사용
    EmployeeData** heapArr = heap->getHeapArray(); 
    int count = heap->getDataNum();               

    // 1. 힙의 모든 데이터를 임시 vector로 복사 (힙 인덱스 1부터 시작)
    std::vector<EmployeeData*> sortedList;
    for (int i = 1; i <= count; i++) {
        sortedList.push_back(heapArr[i]);
    }

    // 2. vector를 "연봉(income) 기준" 내림차순으로 정렬
    std::sort(sortedList.begin(), sortedList.end(), 
        [](EmployeeData* a, EmployeeData* b) {
            // 연봉이 높은 순서 (내림차순)
            return a->getIncome() > b->getIncome();
        }
    );

    // 3. 정렬된 리스트를 포맷에 맞게 출력
    for (EmployeeData* data : sortedList) {
        *fout << data->getName() << "/" << data->getDeptNo() << "/"
              << data->getID() << "/" << data->getIncome() << std::endl;
    }

    return true;
}


void SelectionTree::updateTree(SelectionTreeNode* node) {
    SelectionTreeNode* current = node;

    // 루트에 도달할 때까지 반복
    while (current != root && current->getParent() != nullptr) {
        SelectionTreeNode* parent = current->getParent();
        SelectionTreeNode* left = parent->getLeftChild();
        SelectionTreeNode* right = parent->getRightChild();

        EmployeeData* leftWinner = left->getEmployeeData();
        EmployeeData* rightWinner = right->getEmployeeData();
        EmployeeData* newParentWinner = nullptr;

        // 1. Max Winner Tree: 두 자식 중 연봉(Income)이 더 높은 쪽을 선택
        if (leftWinner == nullptr && rightWinner == nullptr) {
            newParentWinner = nullptr;
        } else if (leftWinner == nullptr) {
            newParentWinner = rightWinner;
        } else if (rightWinner == nullptr) {
            newParentWinner = leftWinner;
        } else if (leftWinner->getIncome() > rightWinner->getIncome()) {
            newParentWinner = leftWinner;
        } else {
            newParentWinner = rightWinner;
        }

        // 2. 부모의 Winner가 바뀌었는지 확인 (안바뀌었으면 종료)
        if (parent->getEmployeeData() == newParentWinner) {
            break;
        }

        // 3. 부모의 Winner를 업데이트하고, 다음 레벨로 이동
        parent->setEmployeeData(newParentWinner);
        current = parent;
    }
}
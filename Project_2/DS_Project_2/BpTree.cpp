#include "BpTree.h"
#include <iostream> // For debugging (optional)

bool BpTree::Insert(EmployeeData* newData) {
    // 1. getter 함수 사용
    string name = newData->getName();

    // Case 1: The tree is completely empty.
    if (root == nullptr) {
        root = new BpTreeDataNode();
        root->insertDataMap(name, newData);
        return true;
    }

    // Find the correct leaf node to insert into
    BpTreeNode* pLeaf = searchDataNode(name);
    map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();

    // Case 2: Key already exists. Update the salary. [cite: 32]
    auto it = dataMap->find(name);
    if (it != dataMap->end()) {
        // 2. setter/getter 함수 사용
        it->second->setIncome(newData->getIncome()); // 연봉만 업데이트
        delete newData; // We don't need the new object, prevent memory leak
        return true;
    }

    // Case 3: Key does not exist. Insert the new data.
    pLeaf->insertDataMap(name, newData);

    // Case 4: Check if the leaf node is full and needs to be split.
    if (excessDataNode(pLeaf)) {
        splitDataNode(pLeaf);
    }
    return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {
    // A data node overflows if it has more than 'order - 1' data entries
    return pDataNode->getDataMap()->size() > order - 1;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {
    // An index node overflows if it has more than 'order - 1' keys
    // (which implies 'order' children)
    return pIndexNode->getIndexMap()->size() > order - 1;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pDataNode);
    map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();

    // Find the split point (middle element)
    int splitIndex = dataMap->size() / 2;
    auto it = dataMap->begin();
    std::advance(it, splitIndex);

    string splitKey = it->first; // This key will be *copied* up to the parent.

    // Create a new sibling data node
    BpTreeDataNode* pNewNode = new BpTreeDataNode();

    // Move the second half of data from the old node to the new node
    while (it != dataMap->end()) {
        pNewNode->insertDataMap(it->first, it->second);
        it = dataMap->erase(it); // Erase from old node and advance iterator
    }

    // Update the doubly-linked list pointers [cite: 36] (implied by pNext/pPrev)
    BpTreeNode* pNext = pLeaf->getNext();
    pLeaf->setNext(pNewNode);
    pNewNode->setPrev(pLeaf);
    pNewNode->setNext(pNext);
    if (pNext) {
        pNext->setPrev(pNewNode);
    }

    // Handle the parent node
    BpTreeNode* pParent = pLeaf->getParent();
    if (pParent == nullptr) {
        // This was the root. Create a new root (IndexNode).
        root = new BpTreeIndexNode();
        root->setMostLeftChild(pLeaf);
        root->insertIndexMap(splitKey, pNewNode);
        pLeaf->setParent(root);
        pNewNode->setParent(root);
    } else {
        // Insert the new key and pointer to the new node into the parent
        pParent->insertIndexMap(splitKey, pNewNode);
        pNewNode->setParent(pParent);
        
        // Check if the parent node now overflows
        if (excessIndexNode(pParent)) {
            splitIndexNode(pParent);
        }
    }
}

void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    BpTreeIndexNode* pIndex = static_cast<BpTreeIndexNode*>(pIndexNode);
    map<string, BpTreeNode*>* indexMap = pIndex->getIndexMap();

    // Find the split point (middle key)
    int splitIndex = indexMap->size() / 2;
    auto it = indexMap->begin();
    std::advance(it, splitIndex);

    string splitKey = it->first; // This key will be *pushed up* to the parent.
    BpTreeNode* pSplitChild = it->second; // This child becomes the mostLeftChild of the new node.

    // Create a new sibling index node
    BpTreeIndexNode* pNewNode = new BpTreeIndexNode();
    
    // The child associated with the split key becomes the leftmost child of the new node
    pNewNode->setMostLeftChild(pSplitChild);
    pSplitChild->setParent(pNewNode);

    // Move keys/children *after* the split key to the new node
    it = indexMap->erase(it); // Erase split key (it's moving to parent) and get next
    while (it != indexMap->end()) {
        pNewNode->insertIndexMap(it->first, it->second);
        it->second->setParent(pNewNode); // Update parent of the moved child
        it = indexMap->erase(it);
    }

    // Handle the parent node
    BpTreeNode* pParent = pIndex->getParent();
    if (pParent == nullptr) {
        // This was the root. Create a new root.
        root = new BpTreeIndexNode();
        root->setMostLeftChild(pIndex);
        root->insertIndexMap(splitKey, pNewNode);
        pIndex->setParent(root);
        pNewNode->setParent(root);
    } else {
        // Insert the split key and pointer to the new node into the parent
        pParent->insertIndexMap(splitKey, pNewNode);
        pNewNode->setParent(pParent);

        // Check if the parent node now overflows
        if (excessIndexNode(pParent)) {
            splitIndexNode(pParent);
        }
    }
}

BpTreeNode* BpTree::searchDataNode(string name) {
    BpTreeNode* pCur = root;
    if (pCur == nullptr) {
        return nullptr; // Tree is empty
    }

    // While pCur is an index node (i.e., not a data node)
    // We check this by seeing if getDataMap() returns nullptr (as defined in BpTreeNode.h)
    while (pCur->getDataMap() == nullptr) {
        map<string, BpTreeNode*>* indexMap = pCur->getIndexMap();

        // upper_bound finds the first key *strictly greater* than 'name'
        auto it = indexMap->upper_bound(name);

        if (it == indexMap->begin()) {
            // 'name' is smaller than or equal to all keys in this index node.
            // Follow the leftmost child pointer.
            pCur = pCur->getMostLeftChild();
        } else {
            // 'name' is greater than the key pointed to by (it-1).
            // Follow the child pointer associated with that previous key.
            it--;
            pCur = it->second;
        }
    }
    
    // Now pCur is a BpTreeDataNode (leaf)
    return pCur;
}

void BpTree::searchRange(string start, string end) {
    // Find the leaf node where the 'start' key would be
    BpTreeNode* pNode = searchDataNode(start);
    if (pNode == nullptr) {
        return; // Tree is empty
    }

    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pNode);
    bool finished = false;

    // Iterate through the leaf nodes using the pNext pointer
    while (pLeaf != nullptr && !finished) {
        map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();
        
        auto it = dataMap->lower_bound(start);

        for (; it != dataMap->end(); ++it) {
            if (it->first > end) {
                finished = true; 
                break;
            }
            
            EmployeeData* data = it->second;
            
            // 3. getter 함수를 사용하여 출력 포맷에 맞게 출력
            // 출력 포맷: name/dept_no/ID/income [cite: 103, 105]
            *fout << data->getName() << "/" << data->getDeptNo() << "/" 
                  << data->getID() << "/" << data->getIncome() << std::endl;
        }
        
        pLeaf = static_cast<BpTreeDataNode*>(pLeaf->getNext());
        start = ""; // 다음 노드부터는 처음부터 검사
    }
}
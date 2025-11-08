#include "Manager.h"
#include <string>
#include <fstream>
#include <sstream> // For parsing lines
#include <vector>  // For ADD_ST_DEPTNO

/**
 * @brief Reads commands from the file and executes them.
 * @param command Filename of the command file (e.g., "command.txt")
 */
void Manager::run(const char* command) {
    fin.open(command);
    if (!fin.is_open()) {
        flog << "========ERROR========\n";
        flog << "Command file not found.\n";
        flog << "=====================\n\n";
        return;
    }

    std::string line;
    // Read command file line by line
    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        std::string cmdName;
        ss >> cmdName; // Get the command name

        if (cmdName == "LOAD") {
            // Check for extra arguments
            std::string extra;
            if (ss >> extra) printErrorCode(100);
            else LOAD();
        } 
        else if (cmdName == "ADD_BP") {
            std::string name, dept_s, id_s, income_s, extra;
            // Check for too few arguments (4 args needed)
            if (!(ss >> name >> dept_s >> id_s >> income_s)) {
                printErrorCode(200);
            } 
            // Check for too many arguments
            else if (ss >> extra) { 
                printErrorCode(200);
            } 
            else {
                // Call helper with parsed arguments
                ADD_BP(name, std::stoi(dept_s), std::stoi(id_s), std::stoi(income_s));
            }
        } 
        else if (cmdName == "SEARCH_BP") {
            std::string arg1, arg2, extra;
            if (!(ss >> arg1)) {
                printErrorCode(300); // Too few args
            } 
            else if (ss >> arg2) { // 2 arguments (range search)
                if (ss >> extra) printErrorCode(300); // Too many args
                else SEARCH_BP_RANGE(arg1, arg2);
            } 
            else { // 1 argument (name search)
                SEARCH_BP_NAME(arg1);
            }
        } 
        else if (cmdName == "PRINT_BP") {
            std::string extra;
            if (ss >> extra) printErrorCode(400);
            else PRINT_BP();
        } 
        else if (cmdName == "ADD_ST") {
            std::string type, value, extra;
            if (!(ss >> type >> value)) {
                printErrorCode(500); // Too few args
            } 
            else if (ss >> extra) {
                printErrorCode(500); // Too many args
            }
            else if (type == "name") {
                ADD_ST_NAME(value);
            } 
            else if (type == "dept_no") {
                ADD_ST_DEPTNO(std::stoi(value));
            } 
            else {
                printErrorCode(500); // Unknown argument type
            }
        } 
        else if (cmdName == "PRINT_ST") {
            std::string dept_s, extra;
            if (!(ss >> dept_s)) {
                printErrorCode(600); // Too few args
            } 
            else if (ss >> extra) {
                printErrorCode(600); // Too many args
            } 
            else {
                PRINT_ST(std::stoi(dept_s));
            }
        } 
        else if (cmdName == "DELETE") {
            std::string extra;
            if (ss >> extra) printErrorCode(700);
            else DELETE();
        } 
        else if (cmdName == "EXIT") {
            flog << "========EXIT========\n";
            flog << "Success\n";
            flog << "====================\n\n";
            break; // Exit loop
        } 
        else {
            printErrorCode(800); // Invalid command
        }
    }

    fin.close();
}

/**
 * @brief Loads employee.txt into BpTree.
 */
void Manager::LOAD() {
    if (bptree->getRoot() != nullptr) {
        printErrorCode(100); // Data already exists
        return;
    }

    std::ifstream empFile("employee.txt");
    if (!empFile.is_open()) {
        printErrorCode(100); // File does not exist
        return;
    }

    std::string line;
    while (std::getline(empFile, line)) {
        std::stringstream ss(line);
        std::string name, dept_s, id_s, income_s;

        // Parse data separated by tabs
        std::getline(ss, name, '\t');
        std::getline(ss, dept_s, '\t');
        std::getline(ss, id_s, '\t');
        std::getline(ss, income_s, '\t');

        EmployeeData* newData = new EmployeeData();
        newData->setData(name, std::stoi(dept_s), std::stoi(id_s), std::stoi(income_s));

        bptree->Insert(newData); // Insert into B+ Tree
    }

    empFile.close();
    flog << "========LOAD========\n";
    flog << "Success\n";
    flog << "====================\n\n";
}

/**
 * @brief Adds a single employee to BpTree.
 * (This function signature is modified to be logical)
 */
void Manager::ADD_BP(string name, int dept_no, int id, int income) {
    EmployeeData* newData = new EmployeeData();
    newData->setData(name, dept_no, id, income);

    bptree->Insert(newData); // Insert or update

    flog << "========ADD_BP========\n";
    flog << name << "/" << dept_no << "/" << id << "/" << income << "\n";
    flog << "====================\n\n";
}

/**
 * @brief Searches BpTree for a single name.
 */
void Manager::SEARCH_BP_NAME(string name) {
    flog << "========SEARCH_BP========\n";
    
    // searchDataNode finds the leaf, then we search the map
    BpTreeNode* leaf = bptree->searchDataNode(name);
    if(leaf == nullptr) {
        printErrorCode(300); // Tree is empty
        flog << "=========================\n\n";
        return;
    }
    
    auto* dataMap = leaf->getDataMap();
    auto it = dataMap->find(name);
    
    if (it == dataMap->end()) {
        printErrorCode(300); // Name not found
    } else {
        EmployeeData* data = it->second;
        flog << data->getName() << "/" << data->getDeptNo() << "/"
             << data->getID() << "/" << data->getIncome() << "\n";
    }
    
    flog << "=========================\n\n";
}

/**
 * @brief Searches BpTree for a range of names.
 */
void Manager::SEARCH_BP_RANGE(string start, string end) {
    flog << "========SEARCH_BP========\n";
    if(bptree->getRoot() == nullptr) {
        printErrorCode(300); // Tree is empty
        flog << "=========================\n\n";
        return;
    }

    // searchRange will print directly to flog
    bptree->searchRange(start, end);
    
    flog << "=========================\n\n";
}

/**
 * @brief Prints all data in BpTree in name-ascending order.
 */
void Manager::PRINT_BP() {
    flog << "========PRINT_BP========\n";
    if(bptree->getRoot() == nullptr) {
        printErrorCode(400); // Tree is empty
        flog << "========================\n\n";
        return;
    }

    // Find the left-most leaf node to start
    BpTreeNode* pCur = bptree->getRoot();
    while (pCur->getDataMap() == nullptr) {
        pCur = pCur->getMostLeftChild();
    }
    
    // Iterate through all leaf nodes using the linked list
    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pCur);
    while (pLeaf != nullptr) {
        // Iterate through the map (which is sorted by name)
        for (auto const& [name, data] : *(pLeaf->getDataMap())) {
            flog << data->getName() << "/" << data->getDeptNo() << "/"<< data->getIncome() << "\n";
        }
        pLeaf = static_cast<BpTreeDataNode*>(pLeaf->getNext());
    }
    
    flog << "========================\n\n";
}

/**
 * @brief Adds employees from BpTree to SelectionTree by department.
 */
void Manager::ADD_ST_DEPTNO(int dept_no) {
    if(bptree->getRoot() == nullptr) {
        printErrorCode(500); // B+ Tree is empty
        return;
    }

    // Find the left-most leaf node
    BpTreeNode* pCur = bptree->getRoot();
    while (pCur->getDataMap() == nullptr) {
        pCur = pCur->getMostLeftChild();
    }
    
    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pCur);
    bool found = false;

    // Iterate through all data nodes (leaves)
    while (pLeaf != nullptr) {
        
        // [수정] C++11 호환되는 map 순회 코드로 변경
        map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();
        for (auto it = dataMap->begin(); it != dataMap->end(); ++it) {
            EmployeeData* data = it->second;

            if (data->getDeptNo() == dept_no) {
                // IMPORTANT: Create a COPY for the Selection Tree
                EmployeeData* newData = new EmployeeData();
                newData->setData(data->getName(), data->getDeptNo(), data->getID(), data->getIncome());
                stree->Insert(newData); // Insert copy into heap
                found = true;
            }
        }
        pLeaf = static_cast<BpTreeDataNode*>(pLeaf->getNext());
    }

    if (!found) {
        printErrorCode(500); // No data found
    } else {
        flog << "========ADD_ST========\n";
        flog << "Success\n";
        flog << "====================\n\n";
    }
}

/**
 * @brief Adds a single employee from BpTree to SelectionTree by name.
 */
void Manager::ADD_ST_NAME(string name) {
    if(bptree->getRoot() == nullptr) {
        printErrorCode(500); // B+ Tree is empty
        return;
    }

    // Find the leaf and the specific data
    BpTreeNode* leaf = bptree->searchDataNode(name);
    auto* dataMap = leaf->getDataMap();
    auto it = dataMap->find(name);
    
    if (it == dataMap->end()) {
        printErrorCode(500); // Name not found
    } else {
        EmployeeData* data = it->second;
        
        // IMPORTANT: Create a COPY for the Selection Tree
        EmployeeData* newData = new EmployeeData();
        newData->setData(data->getName(), data->getDeptNo(), data->getID(), data->getIncome());
        stree->Insert(newData); // Insert copy into heap

        flog << "========ADD_ST========\n";
        flog << "Success\n";
        flog << "====================\n\n";
    }
}

/**
 * @brief Prints data from a specific department heap in SelectionTree.
 * (This function signature is modified to be logical)
 */
void Manager::PRINT_ST(int dept_no) {
    flog << "========PRINT_ST========\n";
    
    // printEmployeeData returns false on error (wrong dept, empty heap)
    if (!stree->printEmployeeData(dept_no)) {
        printErrorCode(600);
    }
    
    flog << "========================\n\n";
}

/**
 * @brief Deletes the employee with the highest salary from SelectionTree.
 */
void Manager::DELETE() {
    if (!stree->Delete()) { // Delete returns false if tree is empty
        printErrorCode(700);
    } else {
        flog << "========DELETE========\n";
        flog << "Success\n";
        flog << "====================\n\n";
    }
}

/**
 * @brief Prints an error code to the log file.
 */
void Manager::printErrorCode(int n) {
	flog << "========ERROR========\n";
	flog << n << "\n";
	flog << "=====================\n\n";
}

// printSuccessCode is not used because the success messages
// are different for each command.
void Manager::printSuccessCode(string success) {
	// This function from the skeleton is not used.
    // Printing is done directly in each helper function.
}
#include "Manager.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Main function to read and execute commands from the command file
// Reads commands line by line and executes each command
// Commands must be in uppercase
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
            printSuccessCode("EXIT");
            break; // Exit loop and terminate program
        } 
        else {
            printErrorCode(800); // Invalid command
        }
    }

    fin.close();
}

// Load employee data from employee.txt into B+ Tree
// Data fields are separated by tabs '\t'
// If data already exists or file does not exist, prints error code 100
void Manager::LOAD() {
    if (bptree->getRoot() != NULL) {
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
    printSuccessCode("LOAD");
}

// Add a single employee directly to the B+ Tree
// If employee does not exist in B+ Tree, creates new node
// If employee already exists, only updates the salary
void Manager::ADD_BP(string name, int dept_no, int id, int income) {
    EmployeeData* newData = new EmployeeData();
    newData->setData(name, dept_no, id, income);

    bptree->Insert(newData); // Insert or update

    flog << "========ADD_BP========\n";
    flog << name << "/" << dept_no << "/" << id << "/" << income << "\n";
    flog << "====================\n\n";
}

// Search for an employee by name in the B+ Tree
// Prints the search result
// Prints error code 300 if not found or tree is empty
void Manager::SEARCH_BP_NAME(string name) {
    // searchDataNode finds the leaf, then we search the map
    BpTreeNode* leaf = bptree->searchDataNode(name);
    if(leaf == NULL) {
        printErrorCode(300); // Tree is empty
        return;
    }
    
    auto* dataMap = leaf->getDataMap();
    auto it = dataMap->find(name);
    
    if (it == dataMap->end()) {
        printErrorCode(300); // Name not found
    } else {
        flog << "========SEARCH_BP========\n";
        EmployeeData* data = it->second;
        flog << data->getName() << "/" << data->getDeptNo() << "/"
             << data->getID() << "/" << data->getIncome() << "\n";
        flog << "=========================\n\n";
    }
}

// Search for employees within a name range [start, end] in the B+ Tree
// For example, start="a" and end="c" prints all employees where a <= name <= c
// Prints error code 300 if tree is empty
void Manager::SEARCH_BP_RANGE(string start, string end) {
    if(bptree->getRoot() == NULL) {
        printErrorCode(300); // Tree is empty
        return;
    }

    flog << "========SEARCH_BP========\n";
    // searchRange will print directly to flog
    bptree->searchRange(start, end);
    flog << "=========================\n\n";
}

// Print all employees in the B+ Tree in name-ascending order
// Prints error code 400 if tree is empty
void Manager::PRINT_BP() {
    if(bptree->getRoot() == NULL) {
        printErrorCode(400); // Tree is empty
        return;
    }

    flog << "========PRINT_BP========\n";
    // Find the left-most leaf node to start
    BpTreeNode* pCur = bptree->getRoot();
    while (pCur->getDataMap() == NULL) {
        pCur = pCur->getMostLeftChild();
    }
    
    // Iterate through all leaf nodes using the linked list
    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pCur);
    while (pLeaf != NULL) {
        // Iterate through the map (which is sorted by name)
        map<string, EmployeeData*>* dataMap = pLeaf->getDataMap();
        for (map<string, EmployeeData*>::iterator it = dataMap->begin(); it != dataMap->end(); ++it) {
            EmployeeData* data = it->second;
            flog << data->getName() << "/" << data->getDeptNo() << "/"
                 << data->getID() << "/" << data->getIncome() << "\n";
        }
        pLeaf = static_cast<BpTreeDataNode*>(pLeaf->getNext());
    }
    
    flog << "========================\n\n";
}

// Copy employees from B+ Tree to Selection Tree by department code
// Searches B+ Tree from first to last in ascending order
// Data is inserted into the appropriate heap and Selection Tree is updated
// Prints error code 500 if data not found or B+ Tree is empty
void Manager::ADD_ST_DEPTNO(int dept_no) {
    if(bptree->getRoot() == NULL) {
        printErrorCode(500); // B+ Tree is empty
        return;
    }

    // Find the left-most leaf node
    BpTreeNode* pCur = bptree->getRoot();
    while (pCur->getDataMap() == NULL) {
        pCur = pCur->getMostLeftChild();
    }
    
    BpTreeDataNode* pLeaf = static_cast<BpTreeDataNode*>(pCur);
    bool found = false;

    // Iterate through all data nodes (leaves)
    while (pLeaf != NULL) {
        
        // C++11 compatible map iteration
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
        printSuccessCode("ADD_ST");
    }
}

// Copy a single employee from B+ Tree to Selection Tree by name
// Uses IndexNode navigation (like SEARCH_BP) to find the employee
// Prints error code 500 if not found or B+ Tree is empty
void Manager::ADD_ST_NAME(string name) {
    if(bptree->getRoot() == NULL) {
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

        printSuccessCode("ADD_ST");
    }
}

// Print all employees in a specific department heap from Selection Tree
// Prints employees sorted by income in descending order
// Prints error code 600 if tree is empty, heap does not exist, or invalid department
void Manager::PRINT_ST(int dept_no) {
    // printEmployeeData returns false on error (wrong dept, empty heap)
    // printEmployeeData handles both header/footer and data output
    if (!stree->printEmployeeData(dept_no)) {
        printErrorCode(600);
    }
}

// Delete the employee with the highest salary from Selection Tree
// Removes the employee in the root node (highest income)
// When removed, if heap's income relationships change, heap is reorganized
// Prints error code 700 if no data exists
void Manager::DELETE() {
    if (!stree->Delete()) { // Delete returns false if tree is empty
        printErrorCode(700);
    } else {
        printSuccessCode("DELETE");
    }
}

// Print an error code to the log file
// Error codes: 100-LOAD, 200-ADD_BP, 300-SEARCH_BP, 400-PRINT_BP
//              500-ADD_ST, 600-PRINT_ST, 700-DELETE, 800-Invalid command
void Manager::printErrorCode(int n) {
	flog << "========ERROR========\n";
	flog << n << "\n";
	flog << "=====================\n\n";
}

// Print a success message with command header to the log file
void Manager::printSuccessCode(string command) {
	flog << "========" << command << "========\n";
	flog << "Success\n";
	flog << "====================\n\n";
}
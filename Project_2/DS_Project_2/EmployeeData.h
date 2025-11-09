#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

// Class for storing employee information
// Stores name, department code, employee ID, and annual income
class EmployeeData {
private:
    string name;     // Employee name (lowercase, less than 10 characters)
    int dept_no;     // Department code (100~800)
    int id;          // Employee ID
    int income;      // Annual income (salary)

public:
    // Constructor
    EmployeeData() {};

    // Destructor
    ~EmployeeData() {};

    // Set all employee data fields at once
    void setData(string name, int dept_no, int id, int income) {
        this->name = name;
        this->dept_no = dept_no;
        this->id = id;
        this->income = income;
    }

    // Set employee name
    void setName(string name) { this->name = name; }

    // Set department code
    void setDeptNo(int dept_no) { this->dept_no = dept_no; }

    // Set employee ID
    void setID(int id) { this->id = id; }

    // Set annual income
    void setIncome(int income) { this->income = income; }

    // Get employee name
    string getName() { return name; }

    // Get department code
    int getDeptNo() { return dept_no; }

    // Get employee ID
    int getID() { return id; }

    // Get annual income
    int getIncome() { return income; }
};
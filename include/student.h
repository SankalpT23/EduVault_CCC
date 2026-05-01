#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include "utils.h"

using namespace std;

class Student {
private:
    int id;
    char name[50];
    char branch[10];
    float cgpa;
    float attendance;
    char badge[20];
    char ai_skill[50];

public:
    Student() : id(0), cgpa(0.0f), attendance(0.0f) {
        strcpy(name, "Unknown");
        strcpy(branch, "N/A");
        strcpy(badge, "None");
        strcpy(ai_skill, "Pending");
    }

    void calculateBadge() {
        if (cgpa >= 9.0f && attendance > 90.0f)
            strcpy(badge, "ELITE SCHOLAR");
        else if (cgpa >= 8.0f)
            strcpy(badge, "RISING STAR");
        else if (attendance < 75.0f)
            strcpy(badge, "RISK ALERT");
        else
            strcpy(badge, "REGULAR");
    }

    void generateAISkill() {
        string b(branch);
        string skill;
        if (b == "CSE")       skill = (cgpa > 8.5f) ? "AI/ML Architect" : "Web Developer";
        else if (b == "ECE")  skill = (cgpa > 8.5f) ? "Embedded Designer" : "IoT Specialist";
        else if (b == "ME")   skill = (cgpa > 8.5f) ? "Robotics Eng." : "CAD Designer";
        else if (b == "EEE")  skill = (cgpa > 8.5f) ? "Power Systems" : "Circuit Designer";
        else                  skill = "Data Analyst";
        strncpy(ai_skill, skill.c_str(), 49);
        ai_skill[49] = '\0';
    }

    int getMeritScore() const {
        return static_cast<int>(cgpa * 10 + attendance * 0.5f);
    }

    int getScholarshipCost() const {
        string b(branch);
        if (b == "CSE" || b == "ECE") return 50;
        if (b == "ME" || b == "EEE") return 40;
        return 30;
    }

    void input() {
        cout << "\n" << BOLD << ">>> STUDENT REGISTRATION" << RESET << endl;

        id = Utils::getValidInt("  ID (1-999999): ", 1, 999999);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "  Name: ";
        string nameStr;
        getline(cin, nameStr);
        if (nameStr.empty()) nameStr = "Unknown";
        strncpy(name, nameStr.c_str(), 49);
        name[49] = '\0';

        cout << "  Branch (CSE/ECE/ME/EEE): ";
        string branchStr;
        getline(cin, branchStr);
        if (branchStr.empty()) branchStr = "N/A";
        strncpy(branch, branchStr.c_str(), 9);
        branch[9] = '\0';

        cgpa = Utils::getValidFloat("  CGPA (0-10): ", 0.0f, 10.0f);
        attendance = Utils::getValidFloat("  Attendance % (0-100): ", 0.0f, 100.0f);

        calculateBadge();
        generateAISkill();

        cout << GREEN << "\n  Badge    : [" << badge << "]" << RESET << endl;
        cout << GREEN << "  AI Skill : [" << ai_skill << "]" << RESET << endl;
        cout << CYAN  << "  Merit    : " << getMeritScore()
             << "  |  Scholarship Cost: " << getScholarshipCost() << "K" << RESET << endl;
        Utils::delay(400);
    }

    void displayRow() const {
        string color = GREEN;
        if (attendance < 75.0f) color = RED;
        else if (cgpa >= 9.0f)  color = CYAN;

        cout << "  " << left
             << setw(7)  << id
             << setw(18) << name
             << setw(8)  << branch
             << setw(7)  << fixed << setprecision(1) << cgpa
             << color    << setw(10) << attendance << RESET
             << setw(16) << badge
             << setw(18) << ai_skill
             << setw(6)  << getMeritScore() << endl;
    }

    void displayDetailed() const {
        string attColor = (attendance >= 75.0f) ? GREEN : RED;
        cout << BOLD << "\n  +----- STUDENT PROFILE -----+" << RESET << endl;
        cout << "  | ID         : " << id << endl;
        cout << "  | Name       : " << name << endl;
        cout << "  | Branch     : " << branch << endl;
        cout << "  | CGPA       : " << fixed << setprecision(2) << cgpa << endl;
        cout << "  | Attendance : " << attColor << attendance << "%" << RESET << endl;
        cout << "  | Badge      : " << YELLOW << badge << RESET << endl;
        cout << "  | AI Skill   : " << CYAN << ai_skill << RESET << endl;
        cout << "  | Merit      : " << MAGENTA << getMeritScore() << RESET << endl;
        cout << "  | Schol Cost : " << getScholarshipCost() << "K" << endl;
        cout << BOLD << "  +----------------------------+" << RESET << endl;
    }

    int    getId()         const { return id; }
    float  getCgpa()       const { return cgpa; }
    float  getAttendance() const { return attendance; }
    string getName()       const { return name; }
    string getBadge()      const { return badge; }
    string getBranch()     const { return branch; }
};

#endif

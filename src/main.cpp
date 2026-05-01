#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include "../include/utils.h"
#include "../include/student.h"
#include "../include/database.h"
#include "../include/auth.h"
#include "../include/algorithms.h"

using namespace std;

void analyticsDashboard(Database& db) {
    Utils::printHeader("ANALYTICS DASHBOARD");

    vector<Student> students = db.loadAll();

    if (students.empty()) {
        Utils::printWarning("No records available for analysis.");
        Utils::pause();
        return;
    }

    int count = (int)students.size();
    float totalCgpa = 0, totalAtt = 0;
    int alerts = 0, elites = 0, risingStar = 0;

    for (const auto& s : students) {
        totalCgpa += s.getCgpa();
        totalAtt  += s.getAttendance();
        if (s.getAttendance() < 75.0f) alerts++;
        if (s.getBadge() == string("ELITE SCHOLAR")) elites++;
        if (s.getBadge() == string("RISING STAR"))   risingStar++;
    }

    cout << "\n" << BOLD << "  +---- INSTITUTION OVERVIEW ----+" << RESET << endl;
    cout << "  | Total Students    : " << count << endl;
    cout << "  | Average CGPA      : " << fixed << setprecision(2) << totalCgpa / count << endl;
    cout << "  | Average Attendance: " << fixed << setprecision(1) << totalAtt / count << "%" << endl;
    cout << "  |" << endl;
    cout << "  | " << CYAN   << "Elite Scholars  : " << elites     << RESET << endl;
    cout << "  | " << GREEN  << "Rising Stars    : " << risingStar << RESET << endl;
    cout << "  | " << RED    << "Attendance Alerts: " << alerts    << RESET << endl;
    cout << BOLD << "  +-------------------------------+" << RESET << endl;

    Utils::pause();
}

void viewAllStudents(Database& db) {
    Utils::printHeader("ALL STUDENTS");

    vector<Student> students = db.loadAll();
    if (students.empty()) {
        Utils::printWarning("Database is empty. Register students first.");
        Utils::pause();
        return;
    }

    cout << "\n" << BOLD << "  " << left
         << setw(7)  << "ID"
         << setw(18) << "NAME"
         << setw(8)  << "BRANCH"
         << setw(7)  << "CGPA"
         << setw(10) << "ATT(%)"
         << setw(16) << "BADGE"
         << setw(18) << "AI SKILL"
         << setw(6)  << "MERIT" << RESET << endl;

    cout << "  " << string(90, '-') << endl;

    for (const auto& s : students) {
        s.displayRow();
    }

    cout << "\n" << DIM << "  Total: " << students.size() << " student(s)" << RESET << endl;
    Utils::pause();
}

void registerStudent(Database& db) {
    Utils::printHeader("REGISTER NEW STUDENT");

    Student s;
    s.input();

    if (db.addStudent(s)) {
        Utils::loadingBar("  Saving to database");
        Utils::printSuccess("Student registered successfully!");
    }
    Utils::pause();
}

void deleteStudent(Database& db) {
    Utils::printHeader("DELETE STUDENT");

    int id = Utils::getValidInt("\n  Enter Student ID to delete: ", 1, 999999);

    cout << "\n  Are you sure? (1 = Yes, 2 = No): ";
    int confirm = Utils::getValidInt("", 1, 2);

    if (confirm == 1) {
        if (db.deleteStudent(id)) {
            Utils::loadingBar("  Deleting record");
            Utils::printSuccess("Student with ID " + to_string(id) + " deleted.");
        } else {
            Utils::printError("Student with ID " + to_string(id) + " not found.");
        }
    } else {
        Utils::printWarning("Deletion cancelled.");
    }
    Utils::pause();
}

int main() {
#ifdef _WIN32
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut != INVALID_HANDLE_VALUE && GetConsoleMode(hOut, &dwMode)) {
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif

    AuthSystem auth;
    if (!auth.authenticate()) {
        Utils::printError("Authentication failed. Exiting.");
        Utils::delay(1000);
        return 1;
    }

    Database db;

    int choice;
    while (true) {
        Utils::printHeader("MAIN MENU");

        cout << "\n";
        cout << "   " << CYAN << "1." << RESET << "  Register Student\n";
        cout << "   " << CYAN << "2." << RESET << "  View All Students\n";
        cout << "   " << CYAN << "3." << RESET << "  Search Student       " << DIM << "[Binary Search]" << RESET << "\n";
        cout << "   " << CYAN << "4." << RESET << "  Student Rankings     " << DIM << "[Merge Sort]" << RESET << "\n";
        cout << "   " << CYAN << "5." << RESET << "  Scholarship Optimizer" << DIM << " [0/1 Knapsack DP]" << RESET << "\n";
        cout << "   " << CYAN << "6." << RESET << "  Analytics Dashboard\n";
        cout << "   " << CYAN << "7." << RESET << "  Delete Student\n";
        cout << "   " << RED  << "8." << RESET << "  Exit\n";

        choice = Utils::getValidInt("\n  Choice: ", 1, 8);

        vector<Student> students;

        switch (choice) {
            case 1:
                registerStudent(db);
                break;
            case 2:
                viewAllStudents(db);
                break;
            case 3:
                students = db.loadAll();
                Algorithms::searchStudent(students);
                Utils::pause();
                break;
            case 4:
                students = db.loadAll();
                Algorithms::showRankings(students);
                Utils::pause();
                break;
            case 5:
                students = db.loadAll();
                Algorithms::scholarshipOptimizer(students);
                Utils::pause();
                break;
            case 6:
                analyticsDashboard(db);
                break;
            case 7:
                deleteStudent(db);
                break;
            case 8:
                Utils::loadingBar("  Shutting down EduVault");
                cout << GREEN << "\n  Goodbye!\n" << RESET << endl;
                return 0;
        }
    }
}

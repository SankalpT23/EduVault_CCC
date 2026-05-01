#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "student.h"
#include "utils.h"

using namespace std;

const string DB_FILE = "data/students.dat";

class Database {
public:
    Database() {
        #ifdef _WIN32
        system("if not exist data mkdir data >nul 2>&1");
        #else
        system("mkdir -p data 2>/dev/null");
        #endif
        ofstream file(DB_FILE, ios::binary | ios::app);
        file.close();
    }

    vector<Student> loadAll() const {
        vector<Student> students;
        ifstream file(DB_FILE, ios::binary);
        if (!file.is_open()) return students;

        Student s;
        while (file.read(reinterpret_cast<char*>(&s), sizeof(Student))) {
            students.push_back(s);
        }
        file.close();
        return students;
    }

    bool addStudent(Student& s) {
        vector<Student> all = loadAll();
        for (const auto& existing : all) {
            if (existing.getId() == s.getId()) {
                Utils::printError("Student with ID " + to_string(s.getId()) + " already exists!");
                return false;
            }
        }

        ofstream file(DB_FILE, ios::binary | ios::app);
        if (!file.is_open()) {
            Utils::printError("Failed to open database file!");
            return false;
        }
        file.write(reinterpret_cast<char*>(&s), sizeof(Student));
        file.close();
        return true;
    }

    bool deleteStudent(int id) {
        vector<Student> students = loadAll();
        bool found = false;

        ofstream file(DB_FILE, ios::binary | ios::trunc);
        if (!file.is_open()) {
            Utils::printError("Failed to open database file!");
            return false;
        }

        for (auto& s : students) {
            if (s.getId() == id) {
                found = true;
                continue;
            }
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
        }
        file.close();
        return found;
    }

    void saveAll(vector<Student>& students) {
        ofstream file(DB_FILE, ios::binary | ios::trunc);
        if (!file.is_open()) {
            Utils::printError("Failed to open database file!");
            return;
        }
        for (auto& s : students) {
            file.write(reinterpret_cast<char*>(&s), sizeof(Student));
        }
        file.close();
    }

    int count() const {
        return (int)loadAll().size();
    }
};

#endif

#ifndef AUTH_H
#define AUTH_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "utils.h"

using namespace std;

const string USERS_FILE = "data/users.dat";

struct User {
    char username[30];
    char password[30];
};

class AuthSystem {
private:
    vector<User> users;

    void loadUsers() {
        users.clear();
        ifstream file(USERS_FILE, ios::binary);
        if (!file.is_open()) return;

        User u;
        while (file.read(reinterpret_cast<char*>(&u), sizeof(User))) {
            users.push_back(u);
        }
        file.close();
    }

    bool saveUser(const User& u) {
        ofstream file(USERS_FILE, ios::binary | ios::app);
        if (!file.is_open()) {
            Utils::printError("Cannot open users file!");
            return false;
        }
        file.write(reinterpret_cast<const char*>(&u), sizeof(User));
        file.close();
        return true;
    }

    bool userExists(const string& uname) const {
        for (const auto& u : users) {
            if (string(u.username) == uname) return true;
        }
        return false;
    }

public:
    bool authenticate() {
        #ifdef _WIN32
        system("if not exist data mkdir data >nul 2>&1");
        #else
        system("mkdir -p data 2>/dev/null");
        #endif

        loadUsers();
        Utils::printHeader("AUTHENTICATION");

        if (users.empty()) {
            cout << YELLOW << "\n  First time? Create your account to get started.\n" << RESET;
            return registerUser();
        }

        cout << "\n  1. Login\n  2. Register New Account\n\n";
        int choice = Utils::getValidInt("  Choice: ", 1, 2);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) return login();
        return registerUser();
    }

    bool login() {
        cout << "\n" << BOLD << "  -- LOGIN --" << RESET << "\n\n";

        cout << "  Username: ";
        string uname;
        getline(cin, uname);

        cout << "  Password: ";
        string pass;
        getline(cin, pass);

        Utils::loadingBar("  Verifying");

        for (const auto& u : users) {
            if (string(u.username) == uname && string(u.password) == pass) {
                Utils::printSuccess("Welcome back, " + uname + "!");
                Utils::delay(600);
                return true;
            }
        }

        Utils::printError("Invalid credentials!");
        Utils::delay(800);
        return false;
    }

    bool registerUser() {
        cout << "\n" << BOLD << "  -- REGISTER --" << RESET << "\n\n";

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "  Choose Username (max 29 chars): ";
        string uname;
        getline(cin, uname);

        if (uname.empty() || uname.length() > 29) {
            Utils::printError("Username must be 1-29 characters!");
            Utils::delay(600);
            return false;
        }

        if (userExists(uname)) {
            Utils::printError("Username '" + uname + "' is already taken!");
            Utils::delay(600);
            return false;
        }

        cout << "  Choose Password (min 4, max 29 chars): ";
        string pass;
        getline(cin, pass);

        if (pass.length() < 4 || pass.length() > 29) {
            Utils::printError("Password must be 4-29 characters!");
            Utils::delay(600);
            return false;
        }

        User newUser;
        memset(&newUser, 0, sizeof(User));
        strncpy(newUser.username, uname.c_str(), 29);
        strncpy(newUser.password, pass.c_str(), 29);

        if (!saveUser(newUser)) return false;

        users.push_back(newUser);
        Utils::loadingBar("  Creating Account");
        Utils::printSuccess("Account created! Welcome, " + uname + "!");
        Utils::delay(600);
        return true;
    }
};

#endif

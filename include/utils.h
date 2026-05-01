#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <limits>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[97m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

using namespace std;

class Utils {
public:
    static void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    }

    static void delay(int ms) {
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
    }

    static void printHeader(const string& title) {
        clearScreen();
        cout << CYAN << BOLD;
        cout << "================================================================" << endl;
        cout << "   E D U V A U L T  |  " << title << endl;
        cout << "================================================================" << RESET << endl;
    }

    static void pause() {
        cout << "\n" << DIM << "Press Enter to continue..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    static void loadingBar(const string& task) {
        cout << YELLOW << task << " ";
        for (int i = 0; i < 20; i++) {
            cout << "#";
            cout.flush();
            delay(25);
        }
        cout << GREEN << " [DONE]" << RESET << endl;
        delay(200);
    }

    static void printSuccess(const string& msg) {
        cout << GREEN << BOLD << "  [OK] " << msg << RESET << endl;
    }

    static void printError(const string& msg) {
        cout << RED << BOLD << "  [ERROR] " << msg << RESET << endl;
    }

    static void printWarning(const string& msg) {
        cout << YELLOW << BOLD << "  [WARN] " << msg << RESET << endl;
    }

    static int getValidInt(const string& prompt, int minVal = 0, int maxVal = INT_MAX) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= minVal && value <= maxVal) {
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid input. Enter a number between " +
                       to_string(minVal) + " and " + to_string(maxVal) + ".");
        }
    }

    static float getValidFloat(const string& prompt, float minVal, float maxVal) {
        float value;
        while (true) {
            cout << prompt;
            if (cin >> value && value >= minVal && value <= maxVal) {
                return value;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printError("Invalid input. Enter a value between " +
                       to_string((int)minVal) + " and " + to_string((int)maxVal) + ".");
        }
    }
};

#endif

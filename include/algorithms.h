#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include "student.h"
#include "utils.h"

using namespace std;

enum SortCriteria { BY_CGPA, BY_ATTENDANCE, BY_NAME, BY_MERIT };

class Algorithms {
public:

    static bool compare(const Student& a, const Student& b,
                        SortCriteria criteria, bool descending) {
        switch (criteria) {
            case BY_CGPA:
                return descending ? (a.getCgpa() >= b.getCgpa())
                                  : (a.getCgpa() <= b.getCgpa());
            case BY_ATTENDANCE:
                return descending ? (a.getAttendance() >= b.getAttendance())
                                  : (a.getAttendance() <= b.getAttendance());
            case BY_NAME:
                return descending ? (a.getName() >= b.getName())
                                  : (a.getName() <= b.getName());
            case BY_MERIT:
                return descending ? (a.getMeritScore() >= b.getMeritScore())
                                  : (a.getMeritScore() <= b.getMeritScore());
            default:
                return true;
        }
    }

    static void merge(vector<Student>& arr, int left, int mid, int right,
                      SortCriteria criteria, bool descending) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<Student> L(arr.begin() + left, arr.begin() + left + n1);
        vector<Student> R(arr.begin() + mid + 1, arr.begin() + mid + 1 + n2);

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (compare(L[i], R[j], criteria, descending)) {
                arr[k++] = L[i++];
            } else {
                arr[k++] = R[j++];
            }
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    static void mergeSort(vector<Student>& arr, int left, int right,
                          SortCriteria criteria, bool descending = true) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid, criteria, descending);
            mergeSort(arr, mid + 1, right, criteria, descending);
            merge(arr, left, mid, right, criteria, descending);
        }
    }

    static void showRankings(vector<Student> students) {
        if (students.empty()) {
            Utils::printWarning("No students in database!");
            return;
        }

        Utils::printHeader("STUDENT RANKINGS  [Merge Sort]");

        cout << "\n  Sort by:\n";
        cout << "    1. CGPA          (Highest First)\n";
        cout << "    2. Attendance    (Highest First)\n";
        cout << "    3. Name          (A -> Z)\n";
        cout << "    4. Merit Score   (Highest First)\n\n";

        int choice = Utils::getValidInt("  Choice: ", 1, 4);

        SortCriteria criteria = BY_CGPA;
        string label;
        bool desc = true;

        switch (choice) {
            case 1: criteria = BY_CGPA;       label = "CGPA";       break;
            case 2: criteria = BY_ATTENDANCE; label = "Attendance"; break;
            case 3: criteria = BY_NAME;       label = "Name"; desc = false; break;
            case 4: criteria = BY_MERIT;      label = "Merit Score"; break;
        }

        cout << "\n";
        Utils::loadingBar("  Sorting with Merge Sort");

        mergeSort(students, 0, (int)students.size() - 1, criteria, desc);

        cout << "\n" << BOLD << CYAN
             << "  === LEADERBOARD  (Sorted by " << label << ") ===" << RESET << "\n\n";

        cout << BOLD << "  " << left
             << setw(6) << "RANK"
             << setw(7) << "ID"
             << setw(18) << "NAME"
             << setw(8)  << "BRANCH"
             << setw(7)  << "CGPA"
             << setw(10) << "ATT(%)"
             << setw(16) << "BADGE"
             << setw(18) << "AI SKILL"
             << setw(6)  << "MERIT" << RESET << endl;

        cout << "  " << string(96, '-') << endl;

        for (int i = 0; i < (int)students.size(); i++) {
            string rank;
            if (i == 0)      rank = " #1";
            else if (i == 1) rank = " #2";
            else if (i == 2) rank = " #3";
            else             rank = " #" + to_string(i + 1);

            cout << BOLD;
            if (i == 0)      cout << YELLOW;
            else if (i == 1) cout << WHITE;
            else if (i == 2) cout << MAGENTA;
            cout << "  " << left << setw(6) << rank << RESET;

            students[i].displayRow();
        }

        cout << "\n" << DIM << "  Algorithm: Merge Sort  |  Complexity: O(n log n)  |  n = "
             << students.size() << RESET << endl;
    }

    static void mergeSortById(vector<Student>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSortById(arr, left, mid);
            mergeSortById(arr, mid + 1, right);
            mergeById(arr, left, mid, right);
        }
    }

    static void mergeById(vector<Student>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<Student> L(arr.begin() + left, arr.begin() + left + n1);
        vector<Student> R(arr.begin() + mid + 1, arr.begin() + mid + 1 + n2);

        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (L[i].getId() <= R[j].getId()) arr[k++] = L[i++];
            else                               arr[k++] = R[j++];
        }
        while (i < n1) arr[k++] = L[i++];
        while (j < n2) arr[k++] = R[j++];
    }

    static int binarySearchById(const vector<Student>& students, int targetId) {
        int lo = 0, hi = (int)students.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (students[mid].getId() == targetId) return mid;
            if (students[mid].getId() < targetId) lo = mid + 1;
            else                                   hi = mid - 1;
        }
        return -1;
    }

    static void searchStudent(vector<Student> students) {
        if (students.empty()) {
            Utils::printWarning("No students in database!");
            return;
        }

        Utils::printHeader("BINARY SEARCH  [O(log n)]");

        int targetId = Utils::getValidInt("\n  Enter Student ID to search: ", 1, 999999);

        cout << "\n";
        Utils::loadingBar("  Sorting by ID (Merge Sort)");
        mergeSortById(students, 0, (int)students.size() - 1);

        Utils::loadingBar("  Binary Searching");

        int idx = binarySearchById(students, targetId);

        if (idx != -1) {
            Utils::printSuccess("Student FOUND!");
            students[idx].displayDetailed();
        } else {
            Utils::printError("Student with ID " + to_string(targetId) + " NOT found.");
        }

        int n = (int)students.size();
        int steps = 0;
        int lo = 0, hi = n - 1;
        while (lo <= hi) { steps++; int mid = lo + (hi - lo) / 2;
            if (students[mid].getId() == targetId) break;
            if (students[mid].getId() < targetId) lo = mid + 1; else hi = mid - 1;
        }
        cout << "\n" << DIM << "  Algorithm: Binary Search  |  Steps: " << steps
             << "  |  Max possible: log2(" << n << ") = ~"
             << (n > 0 ? (int)(log2(n) + 1) : 0)
             << "  |  Linear would take: " << n << RESET << endl;
    }

    static void scholarshipOptimizer(vector<Student> students) {
        if (students.empty()) {
            Utils::printWarning("No students in database!");
            return;
        }

        Utils::printHeader("SCHOLARSHIP OPTIMIZER  [0/1 Knapsack DP]");

        cout << "\n  Each student has:\n";
        cout << "    - Merit Score  (value)  = CGPA*10 + Attendance*0.5\n";
        cout << "    - Scholarship Cost (weight) based on branch\n";
        cout << "      CSE/ECE = 50K | ME/EEE = 40K | Others = 30K\n\n";

        cout << BOLD << "  " << left
             << setw(7) << "ID"
             << setw(18) << "NAME"
             << setw(8)  << "BRANCH"
             << setw(10) << "MERIT"
             << setw(12) << "COST (K)" << RESET << endl;
        cout << "  " << string(55, '-') << endl;

        for (const auto& s : students) {
            cout << "  " << left
                 << setw(7)  << s.getId()
                 << setw(18) << s.getName()
                 << setw(8)  << s.getBranch()
                 << setw(10) << s.getMeritScore()
                 << setw(12) << s.getScholarshipCost() << endl;
        }

        int totalCostAvail = 0;
        for (const auto& s : students)
            totalCostAvail += s.getScholarshipCost();

        cout << "\n  Total cost of all students: " << totalCostAvail << "K\n";
        int budget = Utils::getValidInt(
            "  Enter scholarship budget (in thousands, e.g. 150): ", 1, 999999);

        cout << "\n";
        Utils::loadingBar("  Running 0/1 Knapsack DP");

        int n = (int)students.size();
        int W = budget;

        vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

        for (int i = 1; i <= n; i++) {
            int cost  = students[i - 1].getScholarshipCost();
            int merit = students[i - 1].getMeritScore();
            for (int w = 0; w <= W; w++) {
                dp[i][w] = dp[i - 1][w];
                if (cost <= w) {
                    int pick = dp[i - 1][w - cost] + merit;
                    if (pick > dp[i][w]) dp[i][w] = pick;
                }
            }
        }

        vector<int> selected;
        int w = W;
        for (int i = n; i >= 1; i--) {
            if (dp[i][w] != dp[i - 1][w]) {
                selected.push_back(i - 1);
                w -= students[i - 1].getScholarshipCost();
            }
        }

        int maxMerit = dp[n][W];
        int usedBudget = budget - w;

        cout << "\n" << BOLD << CYAN
             << "  === OPTIMAL SCHOLARSHIP ALLOCATION ===" << RESET << "\n\n";

        if (selected.empty()) {
            Utils::printWarning("Budget too low to award any scholarship!");
        } else {
            cout << BOLD << "  " << left
                 << setw(7) << "ID"
                 << setw(18) << "NAME"
                 << setw(8)  << "BRANCH"
                 << setw(10) << "MERIT"
                 << setw(12) << "COST (K)" << RESET << endl;
            cout << "  " << string(55, '-') << endl;

            for (int idx : selected) {
                const Student& s = students[idx];
                cout << GREEN << "  " << left
                     << setw(7)  << s.getId()
                     << setw(18) << s.getName()
                     << setw(8)  << s.getBranch()
                     << setw(10) << s.getMeritScore()
                     << setw(12) << s.getScholarshipCost() << RESET << endl;
            }

            cout << "\n  " << string(55, '-') << endl;
            cout << BOLD << "  Students Selected : " << GREEN << selected.size()
                 << " / " << n << RESET << endl;
            cout << BOLD << "  Total Merit       : " << CYAN << maxMerit << RESET << endl;
            cout << BOLD << "  Budget Used       : " << YELLOW << usedBudget << "K / "
                 << budget << "K" << RESET << endl;
            cout << BOLD << "  Budget Remaining  : " << GREEN << (budget - usedBudget)
                 << "K" << RESET << endl;
        }

        cout << "\n" << DIM
             << "  Algorithm: 0/1 Knapsack DP  |  Time: O(n*W)  |  n="
             << n << ", W=" << W << "  |  Table size: "
             << (long long)(n + 1) * (W + 1) << " cells" << RESET << endl;
    }
};

#endif

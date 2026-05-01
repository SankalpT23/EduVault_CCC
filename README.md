# EduVault — Intelligent Student Record Management System

A **full-stack Student Management System** with a C++ console backend and HTML/CSS/JS web frontend. Integrates **three DSA algorithms** for intelligent data processing: **Merge Sort**, **Binary Search**, and **0/1 Knapsack Dynamic Programming**.

## Features

### Core Operations
- **Multi-User Authentication** — Register and login (no hardcoded passwords)
- **Student Registration** — Add students with auto-generated badges and AI career predictions
- **View / Delete Records** — Full CRUD with persistent binary file storage
- **Analytics Dashboard** — Institution-wide statistics at a glance

### DSA Algorithms

| # | Algorithm | Category | Purpose | Complexity |
|---|-----------|----------|---------|------------|
| 1 | **Merge Sort** | Divide & Conquer | Rank students by CGPA, Attendance, Name, or Merit Score | O(n log n) |
| 2 | **Binary Search** | Searching | Fast student lookup by ID after sorting | O(log n) |
| 3 | **0/1 Knapsack** | Dynamic Programming | Optimize scholarship allocation within a budget | O(n × W) |

### Smart Features
- **Badge System**: ELITE SCHOLAR, RISING STAR, RISK ALERT, REGULAR — auto-assigned based on CGPA & attendance
- **AI Career Prediction**: Suggests career paths based on branch and performance
- **Merit Score**: Composite score (CGPA × 10 + Attendance × 0.5) used in knapsack optimization

## Project Structure

```
EduVault/
├── src/
│   └── main.cpp            # C++ console application
├── include/
│   ├── utils.h             # UI helpers, colors, input validation
│   ├── student.h           # Student class with badge & skill logic
│   ├── database.h          # Binary file I/O operations
│   ├── auth.h              # Multi-user registration & login
│   └── algorithms.h        # Merge Sort, Binary Search, 0/1 Knapsack
├── frontend/
│   ├── index.html          # Web interface
│   ├── style.css           # Dark-themed premium styling
│   └── app.js              # DSA algorithms in JavaScript
├── data/                   # Runtime data (students.dat, users.dat)
├── docs/                   # Project documentation & PDFs
├── guide.md                # Git collaboration guide (5 members)
├── Makefile                # Build script
├── LICENSE
└── README.md
```

## How to Run

### C++ Console App
```bash
g++ -std=c++17 -Wall -O2 -I include src/main.cpp -o eduvault.exe
./eduvault.exe
```
Or use the Makefile: `make && make run`

### Web Frontend
Open `frontend/index.html` in any browser — no server required. Data is stored in localStorage.

## Tech Stack
- **Backend**: C++ (C++17) with OOP, File I/O, ANSI terminal colors
- **Frontend**: HTML5, CSS3, Vanilla JavaScript
- **DSA**: Merge Sort, Binary Search, 0/1 Knapsack Dynamic Programming
- **Cross-Platform**: Windows & Linux/macOS support

## Algorithm Details

### 1. Merge Sort (Student Rankings)
Sorts the student array using the divide-and-conquer merge sort algorithm. Users choose the sorting criteria (CGPA, Attendance, Name, Merit Score) and see a ranked leaderboard.

### 2. Binary Search (Fast Lookup)
First sorts all students by ID using merge sort, then performs a binary search to find a specific student in O(log n) time — compared to O(n) with linear search. The system displays the number of comparison steps taken.

### 3. 0/1 Knapsack DP (Scholarship Optimizer)
Given a scholarship budget (in thousands), the system uses dynamic programming to select the optimal combination of students that maximizes total merit score without exceeding the budget. Each student has a cost (based on branch) and a value (merit score).

## License

MIT License — See [LICENSE](LICENSE) for details.

# 📚 Student Management System
### A Complete Console-Based C++ Mini Project

---

## 🧾 Overview

The **Student Management System** is a fully functional, production-like console application built in **C++** using **Object-Oriented Programming**, **STL**, and **File Handling**. It is designed as a college-level mini project for practical exams and viva submissions.

---

## 🗂️ Project Structure

```
StudentManagementSystem/
├── src/
│   └── main.cpp          ← Complete source code (single file)
├── data/
│   └── students.dat      ← Auto-generated data file (persistent storage)
├── docs/
│   └── PROJECT_REPORT.md ← Detailed project report
└── README.md             ← This file
```

---

## ✅ Features

| # | Feature | Description |
|---|---------|-------------|
| 1 | **Add Student** | Add student with ID, Name, Age, Gender, Course, Marks, Contact, Attendance |
| 2 | **Display All** | Table-formatted display of all records |
| 3 | **Search** | Find a student by unique ID |
| 4 | **Update** | Edit any field of an existing record |
| 5 | **Delete** | Remove a student record with confirmation |
| 6 | **Sort** | Sort by Marks, Name, ID, or Attendance |
| 7 | **Top Students** | View top N students by marks |
| 8 | **Dashboard** | Statistics: average marks, grade distribution, attendance overview |
| 9 | **Admin Login** | Username/password authentication before access |
| 10 | **Persistent Storage** | All data saved to `students.dat` — survives restarts |
| 11 | **Grade Auto-Calc** | Grades A+/A/B+/B/C/D/F assigned automatically from marks |
| 12 | **Duplicate Prevention** | Blocks adding a student with an existing ID |
| 13 | **Error Handling** | Validates all inputs, prevents crashes |

---

## 🔐 Admin Credentials

```
Username : admin
Password : admin123
```

> You can change these by editing `ADMIN_USER` and `ADMIN_PASS` constants in `main.cpp`.

---

## 🛠️ How to Compile & Run

### Option 1: VS Code (with g++ installed)
```bash
cd StudentManagementSystem/src
g++ -o sms main.cpp -std=c++17
./sms           # Linux/macOS
sms.exe         # Windows
```

### Option 2: CodeBlocks
1. Open CodeBlocks → New Project → Console Application → C++
2. Replace the default `main.cpp` with the provided `main.cpp`
3. Press **Build & Run** (F9)

### Option 3: Online Compilers (no file saving)
- [OnlineGDB](https://www.onlinegdb.com/online_c++_compiler)
- [Replit](https://replit.com/)
> Note: File-saving features won't persist in online compilers.

### Verify g++ is installed
```bash
g++ --version
```
If not installed:
- **Linux**: `sudo apt install g++`
- **Windows**: Install [MinGW](https://www.mingw-w64.org/)
- **macOS**: `xcode-select --install`

---

## 💡 Grade System

| Marks Range | Grade |
|-------------|-------|
| 90 – 100    | A+    |
| 80 – 89     | A     |
| 70 – 79     | B+    |
| 60 – 69     | B     |
| 50 – 59     | C     |
| 40 – 49     | D     |
| Below 40    | F     |

---

## 📊 Attendance Status

| Attendance % | Status   |
|--------------|----------|
| ≥ 75%        | Regular  |
| 50% – 74%    | Low      |
| < 50%        | Critical |

---

## 🏗️ OOP Concepts Used

- **Classes** — `Student` class encapsulates all data and behavior
- **Encapsulation** — Private data, public getters/setters
- **Constructors** — Default and parameterized constructors
- **Methods** — `serialize()`, `deserialize()`, `getGrade()`, etc.
- **STL Vectors** — Dynamic storage for student records
- **STL Algorithms** — `std::sort`, lambdas for flexible sorting
- **File I/O (fstream)** — `ifstream`/`ofstream` for persistent storage

---

## 📁 Data File Format

Records are stored in `data/students.dat` as pipe-delimited text:

```
101|John Doe|20|M|Computer Science|85.50|9876543210|88.00
102|Jane Smith|19|F|Electronics|92.00|9123456789|95.00
```

---

## 🧪 Sample Run

```
==========================================
     STUDENT MANAGEMENT SYSTEM v1.0
==========================================

  Username : admin
  Password : admin123

  [OK] Login successful! Welcome, Admin.
  [###########################] Done!

  +------------------------------------------+
  |   1.  Add Student                        |
  |   2.  Display All Students               |
  |   3.  Search Student by ID               |
  |   4.  Update Student Record              |
  |   5.  Delete Student Record              |
  |   6.  Sort Students                      |
  |   7.  Top Students by Marks              |
  |   8.  Dashboard & Statistics             |
  |   0.  Exit                               |
  +------------------------------------------+

  Enter your choice: _
```

---

## 👨‍💻 Technologies Used

- **Language**: C++17
- **Paradigm**: Object-Oriented Programming (OOP)
- **Libraries**: `<iostream>`, `<fstream>`, `<vector>`, `<algorithm>`, `<string>`, `<sstream>`, `<iomanip>`
- **IDE**: VS Code / CodeBlocks / Any C++ IDE
- **Compiler**: g++ / MinGW / clang++

---

## 📌 Notes

- On first run, `students.dat` is created automatically in the `data/` folder
- If the `data/` folder doesn't exist, create it manually or adjust the `DATA_FILE` path in `main.cpp`
- Compatible with **Windows**, **Linux**, and **macOS**

---

*Developed as a C++ Mini Project | Student Management System v1.0*

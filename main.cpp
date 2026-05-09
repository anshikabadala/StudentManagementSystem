/*
 * ============================================================
 *        STUDENT MANAGEMENT SYSTEM
 *        Version: 1.0
 *        Language: C++
 *        Author: College Project
 *        Description: A complete console-based student
 *                     management system with file handling,
 *                     OOP concepts, and admin authentication.
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <climits>
#include <sstream>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP(ms) Sleep(ms)
    #define CLEAR "cls"
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
    #define CLEAR "clear"
#endif

using namespace std;

// ─────────────────────────────────────────
//  CONSTANTS & CONFIG
// ─────────────────────────────────────────
const string DATA_FILE      = "../data/students.dat";
const string ADMIN_FILE     = "../data/admin.dat";
const string ADMIN_USER     = "admin";
const string ADMIN_PASS     = "admin123";
const int    TABLE_WIDTH     = 120;

// ─────────────────────────────────────────
//  STUDENT CLASS
// ─────────────────────────────────────────
class Student {
private:
    int    studentId;
    string name;
    int    age;
    char   gender;       // 'M' / 'F' / 'O'
    string course;
    float  marks;        // out of 100
    string contact;
    float  attendance;   // percentage 0-100

public:
    // ── Constructors ──────────────────────
    Student() : studentId(0), age(0), gender('M'), marks(0.0f), attendance(0.0f) {}

    Student(int id, const string& nm, int ag, char gn,
            const string& cr, float mk, const string& ct, float att)
        : studentId(id), name(nm), age(ag), gender(gn),
          course(cr), marks(mk), contact(ct), attendance(att) {}

    // ── Getters ───────────────────────────
    int    getId()         const { return studentId; }
    string getName()       const { return name; }
    int    getAge()        const { return age; }
    char   getGender()     const { return gender; }
    string getCourse()     const { return course; }
    float  getMarks()      const { return marks; }
    string getContact()    const { return contact; }
    float  getAttendance() const { return attendance; }

    // ── Grade Calculator ──────────────────
    string getGrade() const {
        if (marks >= 90) return "A+";
        if (marks >= 80) return "A";
        if (marks >= 70) return "B+";
        if (marks >= 60) return "B";
        if (marks >= 50) return "C";
        if (marks >= 40) return "D";
        return "F";
    }

    // ── Attendance Status ─────────────────
    string getAttendanceStatus() const {
        if (attendance >= 75) return "Regular";
        if (attendance >= 50) return "Low";
        return "Critical";
    }

    // ── Setters ───────────────────────────
    void setName(const string& nm)      { name = nm; }
    void setAge(int ag)                 { age = ag; }
    void setGender(char gn)             { gender = gn; }
    void setCourse(const string& cr)    { course = cr; }
    void setMarks(float mk)             { marks = mk; }
    void setContact(const string& ct)   { contact = ct; }
    void setAttendance(float att)       { attendance = att; }

    // ── File Serialization ────────────────
    // Format: id|name|age|gender|course|marks|contact|attendance
    string serialize() const {
        ostringstream oss;
        oss << studentId << "|" << name << "|" << age << "|" << gender
            << "|" << course << "|" << fixed << setprecision(2) << marks
            << "|" << contact << "|" << fixed << setprecision(2) << attendance;
        return oss.str();
    }

    static Student deserialize(const string& line) {
        Student s;
        istringstream iss(line);
        string token;
        int field = 0;
        while (getline(iss, token, '|')) {
            switch (field++) {
                case 0: s.studentId  = stoi(token);           break;
                case 1: s.name       = token;                  break;
                case 2: s.age        = stoi(token);            break;
                case 3: s.gender     = token[0];               break;
                case 4: s.course     = token;                  break;
                case 5: s.marks      = stof(token);            break;
                case 6: s.contact    = token;                  break;
                case 7: s.attendance = stof(token);            break;
            }
        }
        return s;
    }
};

// ─────────────────────────────────────────
//  UTILITY / UI HELPERS
// ─────────────────────────────────────────
void clearScreen()  { system(CLEAR); }

void printLine(char ch = '=', int width = TABLE_WIDTH) {
    cout << string(width, ch) << "\n";
}

void printCentered(const string& text, int width = TABLE_WIDTH) {
    int padding = (width - (int)text.size()) / 2;
    if (padding < 0) padding = 0;
    cout << string(padding, ' ') << text << "\n";
}

void waitForEnter() {
    cout << "\n  Press ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Masked password input (cross-platform simple version)
string getPassword(const string& prompt) {
    cout << prompt;
    string pwd;
    getline(cin, pwd);
    return pwd;
}

// Loading animation
void loadingBar(const string& msg, int steps = 30) {
    cout << "\n  " << msg << "\n  [";
    for (int i = 0; i < steps; ++i) {
        cout << "#" << flush;
        SLEEP(30);
    }
    cout << "] Done!\n";
    SLEEP(300);
}

// ─────────────────────────────────────────
//  WELCOME SCREEN
// ─────────────────────────────────────────
void showWelcomeScreen() {
    clearScreen();
    printLine('*');
    printLine(' ');
    printCentered("*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *");
    printCentered("");
    printCentered("  ____  _  _  ____  ____  ____  _  _  ____     ");
    printCentered(" / ___)/ )( \\(    \\(  __)(  _ \\/ )( \\(_  _)    ");
    printCentered(" \\___ \\) \\/ ( ) D ( ) _)  )   /) \\/ (  )(      ");
    printCentered(" (____/\\____/(____/(____)(__\\_)\\____/ (__)      ");
    printCentered("  __  __  __   __ _   __    ___  ____  __  __  _  _  ____  __ _  ____");
    printCentered(" (  \\/  )(  ) (  ( \\ / _\\  / __)(  __)(  \\/  )/ )( \\(  __)( \\/ )(  __)");
    printCentered("  )    (  )(  /    //    \\( (_ \\ ) _)  )    ( ) __ ( ) _)  )  (  ) _) ");
    printCentered(" (__/\\__)(__)  \\_)__)\\_/\\_/ \\___/(____)(__/\\__)(_)(_)(____)(_/\\_)(____) ");
    printCentered("  ____  _  _  ____  ____  ____  __  _");
    printCentered(" / ___)( \\/ )/ ___)(_  _)(  __)(  )( \\  ");
    printCentered(" \\___ \\ )  / \\___ \\  )(   ) _)  )(  ) ) ");
    printCentered(" (____/(__/  (____/ (__) (____)(__)(_/ ");
    printCentered("");
    printCentered("*  *  *  *  *  *  *  *  *  *  *  *  *  *  *  *");
    printLine(' ');
    printLine('*');

    cout << "\n";
    printCentered("Welcome to the Student Management System v1.0");
    printCentered("A Complete College Mini Project in C++");
    cout << "\n";
    loadingBar("Initializing system");
}

// ─────────────────────────────────────────
//  FILE HANDLING
// ─────────────────────────────────────────
void ensureDataDir() {
    // Create data directory files if they don't exist
    ifstream test(DATA_FILE);
    if (!test.good()) {
        ofstream create(DATA_FILE);
        create.close();
    }
}

vector<Student> loadStudents() {
    vector<Student> students;
    ifstream file(DATA_FILE);
    if (!file.is_open()) return students;
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            students.push_back(Student::deserialize(line));
    }
    file.close();
    return students;
}

bool saveStudents(const vector<Student>& students) {
    ofstream file(DATA_FILE, ios::trunc);
    if (!file.is_open()) return false;
    for (const auto& s : students)
        file << s.serialize() << "\n";
    file.close();
    return true;
}

bool idExists(const vector<Student>& students, int id) {
    for (const auto& s : students)
        if (s.getId() == id) return true;
    return false;
}

// ─────────────────────────────────────────
//  INPUT HELPERS
// ─────────────────────────────────────────
int getIntInput(const string& prompt, int minVal = INT_MIN, int maxVal = INT_MAX) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid input. Please enter a number";
        if (minVal != INT_MIN && maxVal != INT_MAX)
            cout << " between " << minVal << " and " << maxVal;
        cout << ".\n";
    }
}

float getFloatInput(const string& prompt, float minVal = 0.0f, float maxVal = 100.0f) {
    float val;
    while (true) {
        cout << prompt;
        if (cin >> val && val >= minVal && val <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  [!] Invalid input. Enter a value between "
             << minVal << " and " << maxVal << ".\n";
    }
}

string getStringInput(const string& prompt) {
    string val;
    cout << prompt;
    getline(cin, val);
    while (val.empty()) {
        cout << "  [!] Field cannot be empty.\n" << prompt;
        getline(cin, val);
    }
    return val;
}

char getGenderInput() {
    while (true) {
        cout << "  Enter Gender (M/F/O): ";
        char g;
        cin >> g;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        g = toupper(g);
        if (g == 'M' || g == 'F' || g == 'O') return g;
        cout << "  [!] Invalid. Enter M (Male), F (Female), or O (Other).\n";
    }
}

// ─────────────────────────────────────────
//  DISPLAY TABLE
// ─────────────────────────────────────────
void printTableHeader() {
    printLine('-');
    cout << left
         << setw(6)  << " ID"
         << setw(22) << " Name"
         << setw(5)  << " Age"
         << setw(4)  << " Gen"
         << setw(18) << " Course"
         << setw(8)  << " Marks"
         << setw(7)  << " Grade"
         << setw(16) << " Contact"
         << setw(8)  << " Att.%"
         << setw(10) << " Status"
         << "\n";
    printLine('-');
}

void printStudentRow(const Student& s) {
    string genderStr = (s.getGender() == 'M') ? "M" :
                       (s.getGender() == 'F') ? "F" : "O";
    cout << left
         << setw(6)  << (" " + to_string(s.getId()))
         << setw(22) << (" " + s.getName())
         << setw(5)  << (" " + to_string(s.getAge()))
         << setw(4)  << (" " + genderStr)
         << setw(18) << (" " + s.getCourse())
         << setw(8)  << (" " + to_string((int)s.getMarks()))
         << setw(7)  << (" " + s.getGrade())
         << setw(16) << (" " + s.getContact())
         << setw(8)  << (" " + to_string((int)s.getAttendance()) + "%")
         << setw(10) << (" " + s.getAttendanceStatus())
         << "\n";
}

// ─────────────────────────────────────────
//  DASHBOARD / STATS
// ─────────────────────────────────────────
void showDashboard(const vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("DASHBOARD  —  STUDENT STATISTICS");
    printLine('=');

    if (students.empty()) {
        printCentered("No student records found.");
        waitForEnter();
        return;
    }

    int total = (int)students.size();
    int gradeAPlus = 0, gradeA = 0, gradeBPlus = 0, gradeB = 0,
        gradeC = 0, gradeD = 0, gradeF = 0;
    int attCritical = 0, attLow = 0, attRegular = 0;
    float totalMarks = 0, totalAtt = 0;

    for (const auto& s : students) {
        totalMarks += s.getMarks();
        totalAtt   += s.getAttendance();
        string g = s.getGrade();
        if      (g == "A+") gradeAPlus++;
        else if (g == "A")  gradeA++;
        else if (g == "B+") gradeBPlus++;
        else if (g == "B")  gradeB++;
        else if (g == "C")  gradeC++;
        else if (g == "D")  gradeD++;
        else                gradeF++;

        string as = s.getAttendanceStatus();
        if      (as == "Regular")  attRegular++;
        else if (as == "Low")      attLow++;
        else                       attCritical++;
    }

    float avgMarks = totalMarks / total;
    float avgAtt   = totalAtt   / total;

    cout << "\n";
    cout << "  Total Students       : " << total                                 << "\n";
    cout << "  Average Marks        : " << fixed << setprecision(2) << avgMarks  << " / 100\n";
    cout << "  Average Attendance   : " << fixed << setprecision(2) << avgAtt    << " %\n";
    cout << "\n";
    printLine('-');
    cout << "  GRADE DISTRIBUTION\n";
    printLine('-');
    cout << "  A+ (>=90)  : " << gradeAPlus  << " students\n";
    cout << "  A  (>=80)  : " << gradeA      << " students\n";
    cout << "  B+ (>=70)  : " << gradeBPlus  << " students\n";
    cout << "  B  (>=60)  : " << gradeB      << " students\n";
    cout << "  C  (>=50)  : " << gradeC      << " students\n";
    cout << "  D  (>=40)  : " << gradeD      << " students\n";
    cout << "  F  (<40)   : " << gradeF      << " students\n";
    cout << "\n";
    printLine('-');
    cout << "  ATTENDANCE STATUS\n";
    printLine('-');
    cout << "  Regular (>=75%) : " << attRegular  << " students\n";
    cout << "  Low     (>=50%) : " << attLow      << " students\n";
    cout << "  Critical(<50%)  : " << attCritical << " students\n";
    cout << "\n";
    printLine('=');
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: ADD STUDENT
// ─────────────────────────────────────────
void addStudent(vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("ADD NEW STUDENT");
    printLine('=');
    cout << "\n";

    int id = getIntInput("  Enter Student ID   : ", 1, 999999);
    if (idExists(students, id)) {
        cout << "\n  [!] Student ID " << id << " already exists. Duplicate IDs not allowed.\n";
        waitForEnter();
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string name    = getStringInput("  Enter Full Name    : ");
    int    age     = getIntInput("  Enter Age          : ", 5, 100);
    char   gender  = getGenderInput();
    string course  = getStringInput("  Enter Course       : ");
    float  marks   = getFloatInput("  Enter Marks (0-100): ", 0.0f, 100.0f);
    string contact = getStringInput("  Enter Contact No.  : ");
    float  att     = getFloatInput("  Attendance % (0-100): ", 0.0f, 100.0f);

    Student s(id, name, age, gender, course, marks, contact, att);
    students.push_back(s);

    if (saveStudents(students)) {
        cout << "\n  [OK] Student added successfully!\n";
        cout << "       Grade assigned : " << s.getGrade() << "\n";
    } else {
        cout << "\n  [!] Error saving to file.\n";
    }
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: DISPLAY ALL STUDENTS
// ─────────────────────────────────────────
void displayAllStudents(const vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("ALL STUDENT RECORDS");
    printLine('=');

    if (students.empty()) {
        cout << "\n  [!] No student records found.\n";
        waitForEnter();
        return;
    }

    cout << "  Total Records : " << students.size() << "\n\n";
    printTableHeader();
    for (const auto& s : students)
        printStudentRow(s);
    printLine('-');
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: SEARCH STUDENT
// ─────────────────────────────────────────
void searchStudent(const vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("SEARCH STUDENT");
    printLine('=');

    int id = getIntInput("\n  Enter Student ID to search: ", 1, 999999);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (const auto& s : students) {
        if (s.getId() == id) {
            cout << "\n  [OK] Student found!\n\n";
            printLine('-');
            cout << "  Student ID     : " << s.getId()             << "\n";
            cout << "  Full Name      : " << s.getName()           << "\n";
            cout << "  Age            : " << s.getAge()            << "\n";
            cout << "  Gender         : " << s.getGender()         << "\n";
            cout << "  Course         : " << s.getCourse()         << "\n";
            cout << "  Marks          : " << s.getMarks()          << " / 100\n";
            cout << "  Grade          : " << s.getGrade()          << "\n";
            cout << "  Contact        : " << s.getContact()        << "\n";
            cout << "  Attendance     : " << s.getAttendance()     << " %\n";
            cout << "  Att. Status    : " << s.getAttendanceStatus() << "\n";
            printLine('-');
            waitForEnter();
            return;
        }
    }
    cout << "\n  [!] Student with ID " << id << " not found.\n";
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: UPDATE STUDENT
// ─────────────────────────────────────────
void updateStudent(vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("UPDATE STUDENT RECORD");
    printLine('=');

    int id = getIntInput("\n  Enter Student ID to update: ", 1, 999999);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto& s : students) {
        if (s.getId() == id) {
            cout << "\n  Student found: " << s.getName()
                 << " | Leave blank & press ENTER to keep current value.\n\n";

            // Name
            cout << "  Name [" << s.getName() << "]: ";
            string nm; getline(cin, nm);
            if (!nm.empty()) s.setName(nm);

            // Age
            cout << "  Age  [" << s.getAge() << "]: ";
            string ageStr; getline(cin, ageStr);
            if (!ageStr.empty()) {
                try { int a = stoi(ageStr); if (a > 0 && a < 120) s.setAge(a); }
                catch (...) {}
            }

            // Gender
            cout << "  Gender (M/F/O) [" << s.getGender() << "]: ";
            string gStr; getline(cin, gStr);
            if (!gStr.empty()) {
                char g = toupper(gStr[0]);
                if (g == 'M' || g == 'F' || g == 'O') s.setGender(g);
            }

            // Course
            cout << "  Course [" << s.getCourse() << "]: ";
            string cr; getline(cin, cr);
            if (!cr.empty()) s.setCourse(cr);

            // Marks
            cout << "  Marks  [" << s.getMarks() << "]: ";
            string mkStr; getline(cin, mkStr);
            if (!mkStr.empty()) {
                try {
                    float mk = stof(mkStr);
                    if (mk >= 0 && mk <= 100) s.setMarks(mk);
                } catch (...) {}
            }

            // Contact
            cout << "  Contact [" << s.getContact() << "]: ";
            string ct; getline(cin, ct);
            if (!ct.empty()) s.setContact(ct);

            // Attendance
            cout << "  Attendance % [" << s.getAttendance() << "]: ";
            string attStr; getline(cin, attStr);
            if (!attStr.empty()) {
                try {
                    float at = stof(attStr);
                    if (at >= 0 && at <= 100) s.setAttendance(at);
                } catch (...) {}
            }

            if (saveStudents(students)) {
                cout << "\n  [OK] Record updated successfully!\n";
                cout << "       New Grade: " << s.getGrade() << "\n";
            } else {
                cout << "\n  [!] Error saving to file.\n";
            }
            waitForEnter();
            return;
        }
    }
    cout << "\n  [!] Student with ID " << id << " not found.\n";
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: DELETE STUDENT
// ─────────────────────────────────────────
void deleteStudent(vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("DELETE STUDENT RECORD");
    printLine('=');

    int id = getIntInput("\n  Enter Student ID to delete: ", 1, 999999);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->getId() == id) {
            cout << "\n  Found: " << it->getName() << " (ID: " << id << ")\n";
            cout << "  Are you sure you want to delete? (Y/N): ";
            char confirm;
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(confirm) == 'Y') {
                students.erase(it);
                if (saveStudents(students))
                    cout << "\n  [OK] Student deleted successfully.\n";
                else
                    cout << "\n  [!] Error saving to file.\n";
            } else {
                cout << "\n  [--] Delete cancelled.\n";
            }
            waitForEnter();
            return;
        }
    }
    cout << "\n  [!] Student with ID " << id << " not found.\n";
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: SORT STUDENTS
// ─────────────────────────────────────────
void sortStudents(vector<Student>& students) {
    clearScreen();
    printLine('=');
    printCentered("SORT STUDENTS");
    printLine('=');
    cout << "\n";
    cout << "  [1] Sort by Marks    (High to Low)\n";
    cout << "  [2] Sort by Marks    (Low to High)\n";
    cout << "  [3] Sort by Name     (A to Z)\n";
    cout << "  [4] Sort by Name     (Z to A)\n";
    cout << "  [5] Sort by ID       (Ascending)\n";
    cout << "  [6] Sort by Attendance (High to Low)\n";
    cout << "\n";

    int choice = getIntInput("  Choose sort option: ", 1, 6);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice) {
        case 1: sort(students.begin(), students.end(),
                    [](const Student& a, const Student& b){ return a.getMarks() > b.getMarks(); }); break;
        case 2: sort(students.begin(), students.end(),
                    [](const Student& a, const Student& b){ return a.getMarks() < b.getMarks(); }); break;
        case 3: sort(students.begin(), students.end(),
                    [](const Student& a, const Student& b){ return a.getName() < b.getName(); }); break;
        case 4: sort(students.begin(), students.end(),
                    [](const Student& a, const Student& b){ return a.getName() > b.getName(); }); break;
        case 5: sort(students.begin(), students.end(),
                    [](const Student& a, const Student& b){ return a.getId() < b.getId(); }); break;
        case 6: sort(students.begin(), students.end(),
                    [](const Student& a, const Student& b){ return a.getAttendance() > b.getAttendance(); }); break;
    }

    cout << "\n  [OK] Students sorted. Displaying sorted records:\n\n";
    if (students.empty()) { cout << "  No records found.\n"; }
    else {
        printTableHeader();
        for (const auto& s : students) printStudentRow(s);
        printLine('-');
    }
    if (saveStudents(students))
        cout << "\n  [OK] Sort order saved.\n";
    waitForEnter();
}

// ─────────────────────────────────────────
//  FEATURE: TOP N STUDENTS
// ─────────────────────────────────────────
void showTopStudents(vector<Student> students) {
    clearScreen();
    printLine('=');
    printCentered("TOP STUDENTS BY MARKS");
    printLine('=');

    if (students.empty()) {
        cout << "\n  [!] No records found.\n";
        waitForEnter();
        return;
    }

    int n = getIntInput("\n  How many top students to display? ", 1, (int)students.size());
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    sort(students.begin(), students.end(),
         [](const Student& a, const Student& b){ return a.getMarks() > b.getMarks(); });

    cout << "\n  Top " << n << " Students:\n\n";
    printTableHeader();
    for (int i = 0; i < n && i < (int)students.size(); ++i)
        printStudentRow(students[i]);
    printLine('-');
    waitForEnter();
}

// ─────────────────────────────────────────
//  ADMIN LOGIN
// ─────────────────────────────────────────
bool adminLogin() {
    clearScreen();
    printLine('=');
    printCentered("ADMIN LOGIN — STUDENT MANAGEMENT SYSTEM");
    printLine('=');
    cout << "\n";

    int attempts = 3;
    while (attempts-- > 0) {
        cout << "  Username : ";
        string user; getline(cin, user);
        cout << "  Password : ";
        string pass; getline(cin, pass);

        if (user == ADMIN_USER && pass == ADMIN_PASS) {
            cout << "\n  [OK] Login successful! Welcome, Admin.\n";
            loadingBar("Loading dashboard");
            return true;
        }
        cout << "\n  [!] Invalid credentials. Attempts remaining: " << attempts << "\n\n";
    }
    cout << "\n  [X] Too many failed attempts. Exiting.\n\n";
    return false;
}

// ─────────────────────────────────────────
//  MAIN MENU
// ─────────────────────────────────────────
void showMainMenu() {
    clearScreen();
    printLine('=');
    printCentered("STUDENT MANAGEMENT SYSTEM  v1.0");
    printCentered("[ Admin Panel ]");
    printLine('=');
    cout << "\n";
    cout << "  +------------------------------------------+\n";
    cout << "  |   1.  Add Student                        |\n";
    cout << "  |   2.  Display All Students               |\n";
    cout << "  |   3.  Search Student by ID               |\n";
    cout << "  |   4.  Update Student Record              |\n";
    cout << "  |   5.  Delete Student Record              |\n";
    cout << "  |   6.  Sort Students                      |\n";
    cout << "  |   7.  Top Students by Marks              |\n";
    cout << "  |   8.  Dashboard & Statistics             |\n";
    cout << "  |   0.  Exit                               |\n";
    cout << "  +------------------------------------------+\n";
    cout << "\n";
}

// ─────────────────────────────────────────
//  MAIN
// ─────────────────────────────────────────
int main() {
    ensureDataDir();
    showWelcomeScreen();

    if (!adminLogin()) return 0;

    vector<Student> students = loadStudents();

    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        choice = getIntInput("  Enter your choice: ", 0, 8);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: addStudent(students);            break;
            case 2: displayAllStudents(students);    break;
            case 3: searchStudent(students);         break;
            case 4: updateStudent(students);         break;
            case 5: deleteStudent(students);         break;
            case 6: sortStudents(students);          break;
            case 7: showTopStudents(students);       break;
            case 8: showDashboard(students);         break;
            case 0:
                clearScreen();
                printLine('=');
                printCentered("Thank you for using Student Management System!");
                printCentered("Goodbye, Admin.");
                printLine('=');
                cout << "\n";
                break;
            default:
                cout << "\n  [!] Invalid option. Please choose 0-8.\n";
                waitForEnter();
        }
    }
    return 0;
}

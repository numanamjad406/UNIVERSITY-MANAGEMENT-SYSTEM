# UNIVERSITY-MANAGEMENT-SYSTEM
A console-based C++ app to manage university courses, students &amp; enrollments. Supports add, update, delete, search &amp; reports. Data saved in .txt files using file I/O, dynamic arrays, structs &amp; custom string functions. No STL used. — 1st Sem Project
# 🎓 University Management System

### A File-Based C++ Console Application for Managing Courses, Students & Enrollments

> 📌 **Project 3 — 1st Semester (C++ Programming)**

---

## 📖 Overview

The **University Management System** is a console-based application written in **C++** that allows a university administrator to manage **Courses**, **Students**, and **Enrollments** through a simple menu-driven interface.

All data is stored persistently in `.txt` files using **pipe (`|`) separated values**, so information is **saved automatically when you exit** and **loaded automatically when you start** the program again.

This project demonstrates core C++ concepts including:
- 🧩 **Structs** for organizing related data
- 📦 **Dynamic memory allocation** (`new` / `delete[]`)
- 📁 **File handling** (`ifstream` / `ofstream`)
- 🔁 **Custom string functions** (no heavy reliance on `<string>`)
- 🧭 **Menu-driven navigation** using nested loops & switch statements

---

## ✨ Features

### 🟦 1. Course Management
| Option | Description |
|--------|-------------|
| ➕ Add New Course | Auto-generates a unique Course ID, takes name, instructor, credits & capacity |
| 📋 View All Courses | Displays a neatly formatted table of all courses |
| ✏️ Update Course Details | Edit name, instructor, credits, or capacity (press enter to keep old value) |
| ❌ Delete Course | Removes a course **and** all related enrollments automatically |
| 🔍 Search Course | Search by **Course ID** or **Course Name** (partial match supported) |

### 🟩 2. Student Management
| Option | Description |
|--------|-------------|
| ➕ Add New Student | Auto-generates a unique Student ID, takes name, department & semester |
| 📋 View All Students | Displays a table of all registered students |
| ✏️ Update Student Details | Edit name, department, or semester |
| ❌ Delete Student | Removes a student **and** all their enrollments automatically |
| 🔍 Search Student | Search by **Student ID** or **Name** (partial match supported) |

### 🟧 3. Enrollment Management
| Option | Description |
|--------|-------------|
| 📝 Enroll Student in Course | Checks for course capacity & duplicate enrollment before enrolling |
| 📚 View Student Enrollments | Lists all courses a specific student is enrolled in |
| 👥 View Course Enrollments | Lists all students enrolled in a specific course |
| 🔄 Drop Student from Course | Removes an enrollment and frees up a seat in the course |

### 🟪 4. Reports
| Option | Description |
|--------|-------------|
| 📊 Courses with Enrollment Count | Shows every course with `Enrolled / Capacity` |
| 👥 Students in a Specific Course | Lists all students taking a chosen course |
| 📘 Courses of a Specific Student | Lists all courses taken by a chosen student |

### 💾 5. Save & Exit
Saves all current data (Courses, Students, Enrollments) back into their respective `.txt` files and safely exits the program.

---

## 🗂️ Project Structure

```
📦 University-Management-System
 ┣ 📄 university_management_system.cpp   # Main source code
 ┣ 📄 Courses.txt                          # Course records (auto-generated/updated)
 ┣ 📄 Students.txt                         # Student records (auto-generated/updated)
 ┣ 📄 Enrollments.txt                      # Enrollment records (auto-generated/updated)
 ┗ 📄 README.md                            # Project documentation
```

---

## 🧱 Data Structures (Structs)

```cpp
struct Course {
    int  courseID;
    char courseName[100];
    char instructor[50];
    int  credits;
    int  capacity;
    int  enrolled;
};

struct Student {
    int  studentID;
    char studentName[100];
    char department[50];
    int  semester;
};

struct Enrollment {
    int  studentID;
    int  courseID;
    char enrollmentDate[20];
};
```

---

## 📁 Data File Formats

All `.txt` files use **`|` (pipe) as the field separator**.

### 📘 `Courses.txt`
```
CourseID|CourseName|Instructor|Credits|Capacity|Enrolled
```
**Example:**
```
101|Mathematics|Dr. Ahmed|3|40|0
102|Physics|Dr. Sara|3|35|0
103|Programming Fundamentals|Dr. Ali|4|50|0
104|English Composition|Ms. Nadia|2|45|0
105|Calculus|Dr. Usman|3|30|0
```

### 📗 `Students.txt`
```
StudentID|StudentName|Department|Semester
```
> ⚠️ **Note:** Student IDs are auto-generated starting from **2001**.

### 📙 `Enrollments.txt`
```
StudentID|CourseID|EnrollmentDate(YYYY-MM-DD)
```
**Example:**
```
2001|101|2025-01-10
2001|103|2025-01-10
2002|102|2025-01-11
2003|105|2025-01-12
2004|101|2025-01-13
```

---

## ⚙️ How It Works (Behind the Scenes)

- 🚀 **On Startup:** The program reads `courses.txt`, `students.txt`, and `enrollments.txt` into dynamic arrays.
- 🔄 **Enrollment Count Sync:** All `enrolled` counters are reset to `0` and **recalculated** from `Enrollments.txt`, so course capacity always stays accurate.
- 🧮 **Dynamic Resizing:** Whenever a record is added or deleted, a new array of the correct size is allocated, data is copied over, and the old array is freed (`delete[]`) — simulating dynamic lists without using `vector`.
- 🔗 **Cascading Deletes:** Deleting a course or student automatically removes all related enrollment records.
- 🆔 **Auto ID Generation:**
  - Course IDs start from **101**
  - Student IDs start from **2001**
- 🧹 **Custom String Trimming:** A custom `ignore_space()` function trims leading/trailing spaces, tabs, and `\r\n` line endings (important for Windows-saved files).

---

## ▶️ How to Compile & Run

### 🖥️ Using g++ (Linux / Mac / Windows with MinGW)
```bash
g++ university_management_system.cpp -o university_system
./university_system
```

### 🪟 Using Windows (CMD)
```bash
g++ university_management_system.cpp -o university_system.exe
university_system.exe
```

> 💡 Make sure `Courses.txt`, `Students.txt`, and `Enrollments.txt` are in the **same folder** as the executable (or they will be created fresh on first run).

---

## 🧭 Menu Flow

```
=== Course Management System ===

1. Course Management
   ├─ 1. Add New Course
   ├─ 2. View All Courses
   ├─ 3. Update Course Details
   ├─ 4. Delete Course
   ├─ 5. Search Course
   └─ 6. Back

2. Student Management
   ├─ 1. Add New Student
   ├─ 2. View All Students
   ├─ 3. Update Student Details
   ├─ 4. Delete Student
   ├─ 5. Search Student
   └─ 6. Back

3. Enrollment Management
   ├─ 1. Enroll Student in Course
   ├─ 2. View Student Enrollments
   ├─ 3. View Course Enrollments
   ├─ 4. Drop Student from Course
   └─ 5. Back

4. Reports
   ├─ 1. Courses with Enrollment Count
   ├─ 2. Students Enrolled in Specific Course
   ├─ 3. Courses Taken by Specific Student
   └─ 4. Back

5. Save & Exit
```

---

## 🛡️ Validation & Safety Checks

- ✅ Credits are restricted between **1–6**
- ✅ Capacity must be **positive** (defaults to 10 if invalid)
- ✅ Semester must be **positive** (defaults to 1 if invalid)
- ✅ Enrollment blocked if **course is full**
- ✅ Enrollment blocked if **student is already enrolled**
- ✅ Update fields can be **left empty to keep old values**
- ✅ Deleting a course/student **cleans up** all related enrollments

---

## 🚀 Possible Future Enhancements

- 🔐 Add login/authentication for admin access
- 📈 Add GPA / grading system per course
- 🗄️ Migrate from `.txt` files to a proper database (SQLite/MySQL)
- 🖼️ Build a GUI version using Qt or a web frontend
- 📤 Export reports to CSV/PDF

---

## 👨‍💻 Author

**Project:** University Management System
**Course:** Programming Fundamentals (1st Semester)
**Language:** C++ (Standard, file-handling based)

---



⭐ **If you find this project useful, give it a star on GitHub!** ⭐



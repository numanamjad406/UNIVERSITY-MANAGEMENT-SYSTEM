#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

using namespace std;


struct Course
{
    int courseID;
    char courseName[100];
    char instructor[50];
    int credits;
    int capacity;
    int enrolled;
};

struct Student
{
    int studentID;
    char studentName[100];
    char department[50];
    int semester;
};

struct Enrollment
{
    int studentID;
    int courseID;
    char enrollmentDate[20];
};


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}

int strCMP_custom(const char* a, const char* b)
{
    int i = 0;
    while (a[i] != '\0' and b[i] != '\0')
    {
        if (a[i] > b[i])
        {
            return 1;
        }
        if (a[i] < b[i])
        {
            return -1;
        }
        i++;
    }
    if (a[i] == '\0' and b[i] == '\0')
    {
        return 0;
    }
    if (a[i] == '\0')
    {
        return -1;
    }
    return 1;
}

void strCopy_custom(char* dest, const char* src, int maxLen)
{
    int i = 0;
    while (src[i] != '\0' and i < maxLen - 1)
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void ignore_space(char* s)
{
    int len = 0;
    while (s[len] != '\0')
    {
        len++;
    }
    int start = 0;
    while (s[start] == ' ' and s[start] != '\0')
    {
        start++;
    }
    int end = len - 1;
    while (end >= start and (s[end] == ' ' or s[end] == '\r' or s[end] == '\n' or s[end] == '\t'))
    {
        end--;
    }
    int idx = 0;
    for (int i = start; i <= end; ++i)
    {
        s[idx++] = s[i];
    }
    s[idx] = '\0';
}


void loadCoursesFromFile(const char* filename, Course*& courses, int& courseCount);
void saveCoursesToFile(const char* filename, Course* courses, int courseCount);

void loadStudentsFromFile(const char* filename, Student*& students, int& studentCount);
void saveStudentsToFile(const char* filename, Student* students, int studentCount);

void loadEnrollmentsFromFile(const char* filename, Enrollment*& enrollments, int& enrollCount);
void saveEnrollmentsToFile(const char* filename, Enrollment* enrollments, int enrollCount);


int generateCourseID(Course* courses, int courseCount)
{
    int maxID = 100;
    for (int i = 0; i < courseCount; ++i)
    {
        if (courses[i].courseID > maxID)
        {
            maxID = courses[i].courseID;
        }
    }
    return maxID + 1;
}

int generateStudentID(Student* students, int studentCount)
{
    int maxID = 2000;
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].studentID > maxID)
        {
            maxID = students[i].studentID;
        }
    }
    return maxID + 1;
}


int findCourseIndexByID(Course* courses, int courseCount, int id)
{
    for (int i = 0; i < courseCount; i++)
    {
        if (courses[i].courseID == id)
        {
            return i;
        }
    }
    return -1;
}

int findStudentIndexByID(Student* students, int studentCount, int id)
{
    for (int i = 0; i < studentCount; i++)
    {
        if (students[i].studentID == id)
        {
            return i;
        }
    }
    return -1;
}


int enrollmentExists(Enrollment* enrollments, int enrollCount, int studentID, int courseID)
{
    for (int i = 0; i < enrollCount; i++)
    {
        if (enrollments[i].studentID == studentID and enrollments[i].courseID == courseID)
        {
            return i;
        }
    }
    return -1;
}


void addCourse(Course*& courses, int& courseCount)
{
    Course temp;
    clearScreen();
    cout << "--- Add New Course ---\n";

    temp.courseID = generateCourseID(courses, courseCount);
    cout << "Generated Course ID: " << temp.courseID << "\n";

    cout << "Enter Course Name: ";
    cin.ignore();
    cin.getline(temp.courseName, 100);
    ignore_space(temp.courseName);

    cout << "Enter Instructor Name: ";
    cin.getline(temp.instructor, 50);
    ignore_space(temp.instructor);

    cout << "Enter Credits (1-6): ";
    cin >> temp.credits;
    if (temp.credits < 1)
    {
        temp.credits = 1;
    }
    if (temp.credits > 6)
    {
        temp.credits = 6;
    }

    cout << "Enter Capacity (positive): ";
    cin >> temp.capacity;
    if (temp.capacity <= 0)
    {
        temp.capacity = 10;
    }

    temp.enrolled = 0;
    Course* newArr = new Course[courseCount + 1];
    for (int i = 0; i < courseCount; ++i) newArr[i] = courses[i];
    {
        newArr[courseCount] = temp;
    }

    if (courses != nullptr)
    {
        delete[] courses;
    }
    courses = newArr;
    courseCount++;

    cout << "\n Course added successfully.\n";
    pauseScreen();
}

void viewAllCourses(Course* courses, int courseCount)
{
    clearScreen();
    cout << "--- All Courses ---\n\n";
    if (courseCount == 0)
    {
        cout << "No courses available.\n";
        pauseScreen();
        return;
    }
    cout << left << setw(8) << "ID" << setw(30) << "Course Name" << setw(20) << "Instructor" << setw(8) << "Credits" << setw(10) << "Cap" << setw(10) << "Enr" << "\n";
    cout << "--------------------------------------------------------------------------------\n";
    for (int i = 0; i < courseCount; ++i) {
        cout << left << setw(8) << courses[i].courseID
            << setw(30) << courses[i].courseName
            << setw(20) << courses[i].instructor
            << setw(8) << courses[i].credits
            << setw(10) << courses[i].capacity
            << setw(10) << courses[i].enrolled << "\n";
    }
    pauseScreen();
}

void updateCourse(Course* courses, int courseCount)
{
    clearScreen();
    cout << "--- Update Course ---\n";
    if (courseCount == 0)
    {
        cout << "No courses available.\n";
        pauseScreen();
        return;
    }
    int id;
    cout << "Enter Course ID to update: ";
    cin >> id;
    int idx = findCourseIndexByID(courses, courseCount, id);
    if (idx == -1)
    {
        cout << "Course not found.\n";
        pauseScreen();
        return;
    }
    Course& c = courses[idx];
    cout << "Editing Course: " << c.courseName << " (ID: " << c.courseID << ")\n";
    cin.ignore();
    cout << "Enter new Course Name (leave empty to keep): ";
    char buff[100];
    cin.getline(buff, 100);
    ignore_space(buff);
    if (buff[0] != '\0')
    {
        strCopy_custom(c.courseName, buff, 100);
    }

    cout << "Enter new Instructor (leave empty to keep): ";
    cin.getline(buff, 50);
    ignore_space(buff);
    if (buff[0] != '\0')
    {
        strCopy_custom(c.instructor, buff, 50);
    }

    cout << "Enter new Credits (1-6, 0 to keep): ";
    int cr;
    cin >> cr;
    if (cr >= 1 and cr <= 6)
    {
        c.credits = cr;
    }

    cout << "Enter new Capacity (>= enrolled, 0 to keep): ";
    int cap;
    cin >> cap;
    if (cap >= c.enrolled and cap > 0)
    {
        c.capacity = cap;
    }

    cout << "\n Course updated.\n";
    pauseScreen();
}

void deleteCourse(Course*& courses, int& courseCount,
    Enrollment*& enrollments, int& enrollCount)
{
    clearScreen();
    cout << "--- Delete Course ---\n";

    if (courseCount == 0)
    {
        cout << "No courses available.\n";
        pauseScreen();
        return;
    }

    int id;
    cout << "Enter Course ID to delete: ";
    cin >> id;

    int idx = findCourseIndexByID(courses, courseCount, id);
    if (idx == -1)
    {
        cout << "Course not found.\n";
        pauseScreen();
        return;
    }


    int newEnrollCount = 0;


    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].courseID != id)
        {
            newEnrollCount++;
        }
    }

    Enrollment* newEnroll = new Enrollment[newEnrollCount];

    int j = 0;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].courseID != id)
        {
            newEnroll[j] = enrollments[i];
            j++;
        }
    }

    delete[] enrollments;
    enrollments = newEnroll;
    enrollCount = newEnrollCount;



    int newSize = courseCount - 1;

    Course* newArr = nullptr;

    if (newSize > 0)
    {
        newArr = new Course[newSize];
    }
    else
        newArr = nullptr;

    int k = 0;
    for (int i = 0; i < courseCount; ++i)
    {
        if (i != idx)
        {
            newArr[k] = courses[i];
            k++;
        }
    }

    delete[] courses;
    courses = newArr;
    courseCount--;

    cout << "\n Course deleted and related enrollments removed.\n";
    pauseScreen();
}

void searchCourse(Course* courses, int courseCount)
{
    clearScreen();
    cout << "--- Search Course ---\n";
    if (courseCount == 0)
    {
        cout << "No courses available.\n";
        pauseScreen();
        return;
    }
    cout << "Search by: 1) ID  2) Name\nChoice: ";
    int ch;
    cin >> ch;
    if (ch == 1)
    {
        int id;
        cout << "Enter Course ID: ";
        cin >> id;
        int idx = findCourseIndexByID(courses, courseCount, id);
        if (idx == -1)
        {
            cout << "Not found.\n";
        }
        else
        {
            cout << "\nFound:\n";
            cout << "ID: " << courses[idx].courseID << "\n";
            cout << "Name: " << courses[idx].courseName << "\n";
            cout << "Instructor: " << courses[idx].instructor << "\n";
            cout << "Credits: " << courses[idx].credits << "\n";
            cout << "Capacity: " << courses[idx].capacity << "\n";
            cout << "Enrolled: " << courses[idx].enrolled << "\n";
        }
    }
    else if (ch == 2)
    {
        cin.ignore();
        char q[100];
        cout << "Enter part or full name: ";
        cin.getline(q, 100);
        ignore_space(q);
        bool found = false;
        for (int i = 0; i < courseCount; ++i)
        {

            if (strstr(courses[i].courseName, q) != nullptr)
            {
                if (!found) cout << "\nMatches:\n";
                found = true;
                cout << courses[i].courseID << " - " << courses[i].courseName << " (" << courses[i].instructor << ")\n";
            }
        }
        if (!found) cout << "No matches.\n";
    }
    else
    {
        cout << "Invalid choice.\n";
    }
    pauseScreen();
}

void addStudent(Student*& students, int& studentCount)
{
    Student temp;
    clearScreen();
    cout << "--- Add New Student ---\n";
    temp.studentID = generateStudentID(students, studentCount);
    cout << "Generated Student ID: " << temp.studentID << "\n";

    cout << "Enter Student Name: ";
    cin.ignore();
    cin.getline(temp.studentName, 100);
    ignore_space(temp.studentName);

    cout << "Enter Department: ";
    cin.getline(temp.department, 50);
    ignore_space(temp.department);

    cout << "Enter Semester (positive): ";
    cin >> temp.semester;
    if (temp.semester <= 0)
    {
        temp.semester = 1;
    }

    Student* newArr = new Student[studentCount + 1];
    for (int i = 0; i < studentCount; ++i)
    {
        newArr[i] = students[i];
    }
    newArr[studentCount] = temp;
    if (students != nullptr)
    {
        delete[] students;
    }
    students = newArr;
    studentCount++;

    cout << "\n Student added.\n";
    pauseScreen();
}

void viewAllStudents(Student* students, int studentCount)
{
    clearScreen();
    cout << "--- All Students ---\n\n";
    if (studentCount == 0)
    {
        cout << "No students registered.\n";
        pauseScreen();
        return;
    }
    cout << left << setw(8) << "ID" << setw(30) << "Name" << setw(25) << "Department" << setw(10) << "Sem" << "\n";
    cout << "---------------------------------------------------------------------\n";
    for (int i = 0; i < studentCount; ++i)
    {
        cout << left << setw(8) << students[i].studentID
            << setw(30) << students[i].studentName
            << setw(25) << students[i].department
            << setw(10) << students[i].semester << "\n";
    }
    pauseScreen();
}

void updateStudent(Student* students, int studentCount)
{
    clearScreen();
    cout << "--- Update Student ---\n";
    if (studentCount == 0)
    {
        cout << "No students registered.\n";
        pauseScreen();
        return;
    }
    int id;
    cout << "Enter Student ID: ";
    cin >> id;
    int idx = findStudentIndexByID(students, studentCount, id);
    if (idx == -1)
    {
        cout << "Student not found.\n";
        pauseScreen();
        return;
    }
    Student& s = students[idx];
    cin.ignore();
    cout << "Enter new Name (leave empty to keep): ";
    char buff[100];
    cin.getline(buff, 100);
    ignore_space(buff);
    if (buff[0] != '\0')
    {
        strCopy_custom(s.studentName, buff, 100);
    }

    cout << "Enter new Department (leave empty to keep): ";
    cin.getline(buff, 50);
    ignore_space(buff);
    if (buff[0] != '\0')
    {
        strCopy_custom(s.department, buff, 50);
    }

    cout << "Enter new Semester (0 to keep): ";
    int sem;
    cin >> sem;
    if (sem > 0)
    {
        s.semester = sem;
    }

    cout << "\n Student updated.\n";
    pauseScreen();
}

void deleteStudent(Student*& students, int& studentCount, Enrollment*& enrollments, int& enrollCount)
{
    clearScreen();
    cout << "--- Delete Student ---\n";
    if (studentCount == 0)
    {
        cout << "No students registered.\n";
        pauseScreen();
        return;
    }
    int id;
    cout << "Enter Student ID to delete: ";
    cin >> id;
    int idx = findStudentIndexByID(students, studentCount, id);
    if (idx == -1)
    {
        cout << "Student not found.\n";
        pauseScreen();
        return;
    }


    int newEnrollCount = 0;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].studentID != id)
        {
            newEnrollCount++;
        }
    }
    Enrollment* newEnroll = new Enrollment[newEnrollCount];
    int j = 0;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].studentID != id)
        {
            newEnroll[j++] = enrollments[i];
        }
    }
    delete[] enrollments;
    enrollments = newEnroll;
    enrollCount = newEnrollCount;

    Student* newArr = new Student[studentCount - 1 > 0 ? studentCount - 1 : 0];
    int k = 0;
    for (int i = 0; i < studentCount; ++i)
    {
        if (i != idx)
        {
            newArr[k++] = students[i];
        }
    }
    delete[] students;
    students = newArr;
    studentCount--;

    cout << "\n Student deleted and related enrollments removed.\n";
    pauseScreen();
}

void searchStudent(Student* students, int studentCount)
{
    clearScreen();
    cout << "--- Search Student ---\n";
    if (studentCount == 0)
    {
        cout << "No students registered.\n";
        pauseScreen();
        return;
    }
    cout << "Search by: 1) ID  2) Name\nChoice: ";
    int ch;
    cin >> ch;
    if (ch == 1)
    {
        int id;
        cout << "Enter Student ID: ";
        cin >> id;
        int idx = findStudentIndexByID(students, studentCount, id);
        if (idx == -1)
        {
            cout << "Not found.\n";
        }
        else
        {
            cout << "\nFound:\n";
            cout << "ID: " << students[idx].studentID << "\n";
            cout << "Name: " << students[idx].studentName << "\n";
            cout << "Department: " << students[idx].department << "\n";
            cout << "Semester: " << students[idx].semester << "\n";
        }
    }
    else if (ch == 2)
    {
        cin.ignore();
        char q[100];
        cout << "Enter part or full name: ";
        cin.getline(q, 100);
        ignore_space(q);
        bool found = false;
        for (int i = 0; i < studentCount; ++i)
        {
            if (strstr(students[i].studentName, q) != nullptr)
            {
                if (!found)
                {
                    cout << "\nMatches:\n";
                }
                found = true;
                cout << students[i].studentID
                    << " - " << students[i].studentName
                    << " (" << students[i].department << ")\n";
            }
        }
        if (!found)
        {
            cout << "No matches.\n";
        }
    }
    else {
        cout << "Invalid choice.\n";
    }
    pauseScreen();
}


void enrollStudent(Course* courses, int courseCount,
    Student* students, int studentCount, Enrollment*& enrollments, int& enrollCount)
{
    clearScreen();
    cout << "--- Enroll Student in Course ---\n";
    if (courseCount == 0 || studentCount == 0)
    {
        cout << "Courses or Students are empty. Cannot enroll.\n";
        pauseScreen();
        return;
    }
    int sid, cid;
    cout << "Enter Student ID: ";
    cin >> sid;
    int sidx = findStudentIndexByID(students, studentCount, sid);
    if (sidx == -1) {
        cout << "Student not found.\n";
        pauseScreen();
        return;
    }
    cout << "Enter Course ID: ";
    cin >> cid;
    int cidx = findCourseIndexByID(courses, courseCount, cid);
    if (cidx == -1)
    {
        cout << "Course not found.\n";
        pauseScreen();
        return;
    }
    if (courses[cidx].enrolled >= courses[cidx].capacity)
    {
        cout << "Course capacity full.\n";
        pauseScreen();
        return;
    }
    if (enrollmentExists(enrollments, enrollCount, sid, cid) != -1)
    {
        cout << "Student already enrolled in this course.\n";
        pauseScreen();
        return;
    }

    Enrollment temp;
    temp.studentID = sid;
    temp.courseID = cid;

    cout << "Enter enrollment date (YYYY-MM-DD): ";
    cin.ignore();
    cin.getline(temp.enrollmentDate, 20);
    ignore_space(temp.enrollmentDate);
    if (temp.enrollmentDate[0] == '\0')
    {

        strCopy_custom(temp.enrollmentDate, "1970-01-01", 20);
    }

    Enrollment* newArr = new Enrollment[enrollCount + 1];
    for (int i = 0; i < enrollCount; ++i) newArr[i] = enrollments[i];
    newArr[enrollCount] = temp;
    if (enrollments != nullptr) delete[] enrollments;
    enrollments = newArr;
    enrollCount++;

    courses[cidx].enrolled += 1;

    cout << "\n Enrollment successful.\n";
    pauseScreen();
}

void viewStudentEnrollments(Enrollment* enrollments, int enrollCount,
    Course* courses, int courseCount, Student* students, int studentCount)
{
    clearScreen();
    cout << "--- View Student Enrollments ---\n";
    if (enrollCount == 0)
    {
        cout << "No enrollments.\n";
        pauseScreen();
        return;
    }
    int sid;
    cout << "Enter Student ID: ";
    cin >> sid;
    int sidx = findStudentIndexByID(students, studentCount, sid);
    if (sidx == -1)
    {
        cout << "Student not found.\n";
        pauseScreen();
        return;
    }
    cout << "Enrollments for " << students[sidx].studentName << ":\n";
    bool found = false;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].studentID == sid)
        {
            int cidx = findCourseIndexByID(courses, courseCount, enrollments[i].courseID);
            if (cidx != -1)
            {
                cout << "- " << courses[cidx].courseID
                    << " : " << courses[cidx].courseName
                    << " | Date: " << enrollments[i].enrollmentDate << "\n";
                found = true;
            }
        }
    }
    if (!found)
    {
        cout << "No enrollments for this student.\n";
    }
    pauseScreen();
}

void viewCourseEnrollments(Enrollment* enrollments, int enrollCount,
    Course* courses, int courseCount, Student* students, int studentCount)
{
    clearScreen();
    cout << "--- View Course Enrollments ---\n";
    if (enrollCount == 0)
    {
        cout << "No enrollments.\n";
        pauseScreen();
        return;
    }
    int cid;
    cout << "Enter Course ID: ";
    cin >> cid;
    int cidx = findCourseIndexByID(courses, courseCount, cid);
    if (cidx == -1)
    {
        cout << "Course not found.\n";
        pauseScreen();
        return;
    }
    cout << "Students enrolled in " << courses[cidx].courseName << ":\n";
    bool found = false;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].courseID == cid)
        {
            int sidx = findStudentIndexByID(students, studentCount, enrollments[i].studentID);
            if (sidx != -1)
            {
                cout << "- " << students[sidx].studentID
                    << " : " << students[sidx].studentName
                    << " | Date: " << enrollments[i].enrollmentDate << "\n";
                found = true;
            }
        }
    }
    if (!found) cout << "No students enrolled in this course.\n";
    pauseScreen();
}

void dropCourse(Course* courses, int courseCount, Student* students,
    int studentCount, Enrollment*& enrollments, int& enrollCount)
{
    clearScreen();
    cout << "--- Drop Student from Course ---\n";
    if (enrollCount == 0) {
        cout << "No enrollments.\n";
        pauseScreen();
        return;
    }
    int sid, cid;
    cout << "Enter Student ID: ";
    cin >> sid;
    cout << "Enter Course ID: ";
    cin >> cid;
    int eidx = enrollmentExists(enrollments, enrollCount, sid, cid);
    if (eidx == -1)
    {
        cout << "Enrollment not found.\n";
        pauseScreen();
        return;
    }

    int newSize = enrollCount - 1;
    if (newSize < 0)
    {
        newSize = 0;
    }
    Enrollment* newArr = new Enrollment[newSize];

    int k = 0;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (i != eidx)
        {
            newArr[k++] = enrollments[i];
        }
    }
    delete[] enrollments;
    enrollments = newArr;
    enrollCount--;

    int cidx = findCourseIndexByID(courses, courseCount, cid);
    if (cidx != -1)
    {
        if (courses[cidx].enrolled > 0)
        {
            courses[cidx].enrolled -= 1;
        }
    }

    cout << "\n Student dropped from course.\n";
    pauseScreen();
}
void reportCoursesWithCount(Course* courses, int courseCount)
{
    clearScreen();
    cout << "--- Courses with Enrollment Count ---\n";
    if (courseCount == 0)
    {
        cout << "No courses.\n";
        pauseScreen();
        return;
    }
    for (int i = 0; i < courseCount; ++i)
    {
        cout << courses[i].courseID
            << " | " << courses[i].courseName
            << " | Enrolled: " << courses[i].enrolled
            << " / " << courses[i].capacity << "\n";
    }
    pauseScreen();
}

void reportStudentsInCourse(Enrollment* enrollments, int enrollCount, Course* courses,
    int courseCount, Student* students, int studentCount)
{
    clearScreen();
    cout << "--- Students Enrolled in Specific Course ---\n";
    if (enrollCount == 0)
    {
        cout << "No enrollments.\n";
        pauseScreen();
        return;
    }
    int cid;
    cout << "Enter Course ID: ";
    cin >> cid;
    int cidx = findCourseIndexByID(courses, courseCount, cid);
    if (cidx == -1)
    {
        cout << "Course not found.\n";
        pauseScreen();
        return;
    }
    cout << "Students in " << courses[cidx].courseName << ":\n";
    bool found = false;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].courseID == cid)
        {
            int sidx = findStudentIndexByID(students, studentCount, enrollments[i].studentID);
            if (sidx != -1)
            {
                cout << "- " << students[sidx].studentID << " : " << students[sidx].studentName << "\n";
                found = true;
            }
        }
    }
    if (!found) cout << "No students.\n";
    pauseScreen();
}

void reportCoursesOfStudent(Enrollment* enrollments, int enrollCount, Course* courses, int courseCount, Student* students, int studentCount) {
    clearScreen();
    cout << "--- Courses Taken by Specific Student ---\n";
    if (enrollCount == 0)
    {
        cout << "No enrollments.\n";
        pauseScreen();
        return;
    }
    int sid;
    cout << "Enter Student ID: ";
    cin >> sid;
    int sidx = findStudentIndexByID(students, studentCount, sid);
    if (sidx == -1)
    {
        cout << "Student not found.\n";
        pauseScreen();
        return;
    }
    cout << "Courses for " << students[sidx].studentName << ":\n";
    bool found = false;
    for (int i = 0; i < enrollCount; ++i)
    {
        if (enrollments[i].studentID == sid)
        {
            int cidx = findCourseIndexByID(courses, courseCount, enrollments[i].courseID);
            if (cidx != -1)
            {
                cout << "- " << courses[cidx].courseID << " : " << courses[cidx].courseName << "\n";
                found = true;
            }
        }
    }
    if (!found) cout << "No courses.\n";
    pauseScreen();
}

void loadCoursesFromFile(const char* filename, Course*& courses, int& courseCount)
{
    ifstream Rdr(filename);
    if (!Rdr.is_open())
    {
        courseCount = 0;
        courses = nullptr;
        return;
    }

    const int MAX_LINES = 1000;
    char lines[MAX_LINES][512];
    int count = 0;


    while (count < MAX_LINES && Rdr.getline(lines[count], 512))
    {
        if (lines[count][0] != '\0') count++;
    }

    Rdr.close();

    if (count == 0)
    {
        courseCount = 0;
        courses = nullptr;
        return;
    }

    courses = new Course[count];
    courseCount = count;

    for (int i = 0; i < count; ++i)
    {
        char* ptr = lines[i];


        courses[i].courseID = atoi(ptr);
        while (*ptr != '|' && *ptr != '\0')
        {
            ptr++;
        }
        if (*ptr == '|')
        {
            ptr++;
        }


        int j = 0;
        while (*ptr != '|' && *ptr != '\0')
        {
            courses[i].courseName[j++] = *ptr;
            ptr++;
        }
        courses[i].courseName[j] = '\0';
        if (*ptr == '|')
        {
            ptr++;
        }


        j = 0;
        while (*ptr != '|' && *ptr != '\0')
        {
            courses[i].instructor[j++] = *ptr;
            ptr++;
        }
        courses[i].instructor[j] = '\0';
        if (*ptr == '|')
        {
            ptr++;
        }


        courses[i].credits = atoi(ptr);
        while (*ptr != '|' and *ptr != '\0')
        {
            ptr++;
        }
        if (*ptr == '|')
        {
            ptr++;
        }


        courses[i].capacity = atoi(ptr);
        while (*ptr != '|' and *ptr != '\0')
        {
            ptr++;
        }
        if (*ptr == '|')
        {
            ptr++;
        }


        courses[i].enrolled = atoi(ptr);


        ignore_space(courses[i].courseName);
        ignore_space(courses[i].instructor);
    }
}

void saveCoursesToFile(const char* filename, Course* courses, int courseCount)
{
    ofstream Rdr(filename);
    if (!Rdr.is_open())
    {
        return;
    }
    for (int i = 0; i < courseCount; ++i)
    {
        Rdr << courses[i].courseID
            << "|" << courses[i].courseName
            << "|" << courses[i].instructor
            << "|" << courses[i].credits
            << "|" << courses[i].capacity
            << "|" << courses[i].enrolled
            << "\n";
    }
    Rdr.close();
}

void loadStudentsFromFile(const char* filename, Student*& students, int& studentCount)
{
    ifstream Rdr(filename);
    if (!Rdr.is_open())
    {
        studentCount = 0;
        students = nullptr;
        return;
    }


    const int MAX_LINES = 1000;
    char lines[MAX_LINES][512];
    int count = 0;

    while (count < MAX_LINES && Rdr.getline(lines[count], 512))
    {
        if (lines[count][0] != '\0')
        {
            count++;
        }
    }

    Rdr.close();

    if (count == 0)
    {
        studentCount = 0;
        students = nullptr;
        return;
    }

    students = new Student[count];
    studentCount = count;

    for (int i = 0; i < count; ++i)
    {

        char* ptr = lines[i];


        students[i].studentID = atoi(ptr);
        while (*ptr != '|' and *ptr != '\0')
        {
            ptr++;
        }
        if (*ptr == '|')
        {
            ptr++;
        }


        int j = 0;
        while (*ptr != '|' and *ptr != '\0')
        {
            students[i].studentName[j++] = *ptr;
            ptr++;
        }
        students[i].studentName[j] = '\0';
        if (*ptr == '|')
        {
            ptr++;
        }


        j = 0;
        while (*ptr != '|' and *ptr != '\0')
        {
            students[i].department[j++] = *ptr;
            ptr++;
        }
        students[i].department[j] = '\0';
        if (*ptr == '|')
        {
            ptr++;
        }


        students[i].semester = atoi(ptr);


        ignore_space(students[i].studentName);
        ignore_space(students[i].department);
    }
}

void saveStudentsToFile(const char* filename, Student* students, int studentCount)
{
    ofstream Rdr(filename);
    if (!Rdr.is_open())
    {
        return;
    }
    for (int i = 0; i < studentCount; ++i)
    {
        Rdr << students[i].studentID
            << "|" << students[i].studentName << "|" << students[i].department
            << "|" << students[i].semester << "\n";
    }
    Rdr.close();
}

void loadEnrollmentsFromFile(const char* filename, Enrollment*& enrollments, int& enrollCount)
{
    ifstream Rdr(filename);
    if (!Rdr.is_open())
    {
        enrollCount = 0;
        enrollments = nullptr;
        return;
    }
    char line[512];
    int cnt = 0;
    while (Rdr.getline(line, 512))
    {
        if (line[0] != '\0') cnt++;
    }
    Rdr.clear();
    Rdr.seekg(0);

    if (cnt == 0)
    {
        enrollCount = 0;
        enrollments = nullptr;
        Rdr.close();
        return;
    }
    enrollments = new Enrollment[cnt];
    enrollCount = cnt;
    for (int i = 0; i < cnt; ++i)
    {
        Rdr >> enrollments[i].studentID;
        Rdr.get();
        Rdr >> enrollments[i].courseID;
        Rdr.get();
        Rdr.getline(enrollments[i].enrollmentDate, 20);
        ignore_space(enrollments[i].enrollmentDate);
    }
    Rdr.close();
}

void saveEnrollmentsToFile(const char* filename, Enrollment* enrollments, int enrollCount)
{
    ofstream Rdr(filename);
    if (!Rdr.is_open())
    {
        return;
    }
    for (int i = 0; i < enrollCount; ++i)
    {
        Rdr << enrollments[i].studentID
            << "|" << enrollments[i].courseID
            << "|" << enrollments[i].enrollmentDate << "\n";
    }
    Rdr.close();
}


void displayMainMenu() {
    cout << "\n=== Course Management System ===\n\n";
    cout << "1. Course Management\n2. Student Management\n3. Enrollment Management\n4. Reports\n5. Save & Exit\n\nEnter your choice: ";
}

void courseMenu() {
    cout << "\n--- Course Management ---\n";
    cout << "1. Add New Course\n2. View All Courses\n3. Update Course Details\n4. Delete Course\n5. Search Course\n6. Back\n\nEnter your choice: ";
}

void studentMenu() {
    cout << "\n--- Student Management ---\n";
    cout << "1. Add New Student\n2. View All Students\n3. Update Student Details\n4. Delete Student\n5. Search Student\n6. Back\n\nEnter your choice: ";
}

void enrollmentMenu() {
    cout << "\n--- Enrollment Management ---\n";
    cout << "1. Enroll Student in Course\n2. View Student Enrollments\n3. View Course Enrollments\n4. Drop Student from Course\n5. Back\n\nEnter your choice: ";
}

void reportsMenu()
{
    cout << "\n--- Reports ---\n";
    cout << "1. Display All Courses with Enrollment Count\n2. Display Students Enrolled in Specific Course\n3. Display Courses Taken by Specific Student\n4. Back\n\nEnter your choice: ";
}
int main()
{
    Course* courses = nullptr;
    int courseCount = 0;

    Student* students = nullptr;
    int studentCount = 0;

    Enrollment* enrollments = nullptr;
    int enrollCount = 0;

    loadCoursesFromFile("courses.txt", courses, courseCount);
    loadStudentsFromFile("students.txt", students, studentCount);
    loadEnrollmentsFromFile("enrollments.txt", enrollments, enrollCount);


    for (int i = 0; i < courseCount; ++i)
    {
        courses[i].enrolled = 0;
    }

    for (int i = 0; i < enrollCount; ++i)
    {
        int idx = findCourseIndexByID(courses, courseCount, enrollments[i].courseID);
        if (idx != -1)
        {
            courses[idx].enrolled++;
        }
    }

    bool running = true;

    while (running)
    {
        clearScreen();
        displayMainMenu();

        int mainChoice;
        cin >> mainChoice;

        switch (mainChoice)
        {
        case 1:
        {
            bool back = false;
            while (!back)
            {
                clearScreen();
                courseMenu();
                int ch;
                cin >> ch;

                switch (ch)
                {
                case 1:
                {
                    addCourse(courses, courseCount);
                    break;
                }
                case 2:
                {
                    viewAllCourses(courses, courseCount);
                    break;
                }
                case 3:
                {

                    updateCourse(courses, courseCount);
                    break;
                }
                case 4:
                {
                    deleteCourse(courses, courseCount, enrollments, enrollCount);
                    break;
                }
                case 5:
                {
                    searchCourse(courses, courseCount);
                    break;
                }
                case 6:
                {
                    back = true;
                    break;
                }
                }
            }
            break;
        }

        case 2:
        {
            bool back = false;
            while (!back)
            {
                clearScreen();
                studentMenu();
                int ch;
                cin >> ch;

                switch (ch)
                {
                case 1:
                {
                    addStudent(students, studentCount);
                    break;
                }
                case 2:
                {
                    viewAllStudents(students, studentCount);
                    break;
                }
                case 3:
                {
                    updateStudent(students, studentCount);
                    break;
                }
                case 4:
                {
                    deleteStudent(students, studentCount, enrollments, enrollCount);
                    break;
                }
                case 5:
                {
                    searchStudent(students, studentCount);
                    break;
                }
                case 6:
                {
                    back = true;
                    break;
                }
                }
            }
            break;
        }

        case 3:
        {
            bool back = false;
            while (!back)
            {
                clearScreen();
                enrollmentMenu();

                int ch;
                cin >> ch;

                switch (ch)
                {
                case 1:
                {
                    enrollStudent(courses, courseCount, students, studentCount, enrollments, enrollCount);
                    break;
                }
                case 2:
                {
                    viewStudentEnrollments(enrollments, enrollCount, courses, courseCount, students, studentCount);
                    break;
                }
                case 3:
                {
                    viewCourseEnrollments(enrollments, enrollCount, courses, courseCount, students, studentCount);
                    break;
                }
                case 4:
                {
                    dropCourse(courses, courseCount, students, studentCount, enrollments, enrollCount);
                    break;
                }
                case 5:
                {
                    back = true;
                    break;
                }
                }
            }
            break;
        }

        case 4:
        {
            bool back = false;
            while (!back)
            {
                clearScreen();
                reportsMenu();

                int ch;
                cin >> ch;

                switch (ch)
                {
                case 1:
                {
                    reportCoursesWithCount(courses, courseCount);
                    break;
                }
                case 2:
                {
                    reportStudentsInCourse(enrollments, enrollCount, courses, courseCount, students, studentCount);
                    break;
                }
                case 3:
                {
                    reportCoursesOfStudent(enrollments, enrollCount, courses, courseCount, students, studentCount);
                    break;
                }
                case 4:
                {
                    back = true;
                    break;
                }
                }
            }
            break;
        }

        case 5:
        {
            saveCoursesToFile("courses.txt", courses, courseCount);
            saveStudentsToFile("students.txt", students, studentCount);
            saveEnrollmentsToFile("enrollments.txt", enrollments, enrollCount);
            cout << "\nData saved. Exiting...\n";
            running = false;
            break;
        }
        }
    }


    if (courses)
    {
        delete[] courses;
    }
    if (students)
    {
        delete[] students;
    }
    if (enrollments)
    {
        delete[] enrollments;
    }

    return 0;
}

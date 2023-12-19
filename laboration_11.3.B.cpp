#include <iostream>
#include <string>
#include <Windows.h>
#include <iomanip>
#include <fstream>

using namespace std;

enum Specialnost { Computer_Science, Informatics, Mathematics_And_Economics, Physics_And_Informatics, Labor_Education };
string specStr[] = { "Computer_Science", "Informatics", "Mathematics_And_Economics", "Physics_And_Informatics", "Labor_Education" };

union Grade {
    double programmingGrade;
    double numericalMethodsGrade;
    double pedagogyGrade;
};

struct Student {
    int studentNumber;
    char prizv[60];
    int course;
    Specialnost spec;
    double physicsGrade;
    double mathGrade;
    Grade thirdGrade;
};

void Create(string filename);
void Print(string filename);
void BubbleSort(string filename);
int* Index_Sort(string filename);
void PrintIndexSorted(string filename, int* indexArray);
int BinSearch(string filename, const string prizv, const Specialnost specialnost, const double grade);

int main() {
    string filename;
    cout << "Input name of file: ";
    cin >> filename;
    string searchName;
    Specialnost searchSpecialnost;
    double searchGrade;
    int result;
    int menuItem;
    do {
        cout << endl << endl << endl;
        cout << "Select an action: " << endl << endl;
        cout << " [1] - entering data from the keyboard " << endl;
        cout << " [2] - cout data in display" << endl;
        cout << " [3] - sort" << endl;
        cout << " [4] - index sort" << endl;
        cout << " [5] - binary search" << endl;
        cout << " [0] - Exit" << endl << endl;
        cout << "Input value: "; cin >> menuItem;
        cout << endl << endl << endl;

        switch (menuItem)
        {
        case 1:
            Create(filename);
            break;
        case 2:
            Print(filename);
            break;
        case 3:
            BubbleSort(filename);
            Print(filename);
            break;
        case 4:
            PrintIndexSorted(filename, Index_Sort(filename));
            break;
        case 5:
            cout << "Enter the student's last name: ";
            cin >> searchName;

            cout << "Enter the student's specialty (0 - Computer_Science, 1 - Informatics, 2 - Mathematics_And_Economics, 3 - Physics_And_Informatics, 4 - Labor_Education): ";
            int spec;
            cin >> spec;
            searchSpecialnost = static_cast<Specialnost>(spec);

            cout << "Enter the student's grade: ";
            cin >> searchGrade;
            result = BinSearch(filename, searchName, searchSpecialnost, searchGrade);
            if (result != -1) {
                cout << "Student found at position " << result + 1 << "." << endl;
            }
            else {
                cout << "Student not found." << endl;
            }
            break;
        case 0:
            break;
        default:
            cout << "You choose fake value! " << endl;
        }
    } while (menuItem != 0);

    return 0;
}

void Create(string filename) {
    ofstream f(filename, ios::binary);
    Student s;
    char ch;
    int spec, i = 0;



    do {
        i++;
        s.studentNumber;
        cin.get();

        cout << "Student #" << i << ":" << endl;
        cout << "LastName: "; cin.getline(s.prizv, 60);
        cout << "Course: "; cin >> s.course;



        cout << "Specialnost (0 - Computer_Science, 1 - Informatics, 2 - Mathematics_And_Economics, 3 - Physics_And_Informatics, 4 - Labor_Education): ";
        cin >> spec;
        s.spec = (Specialnost)spec;

        cout << "Math_Grade: "; cin >> s.mathGrade;
        cout << "Phisics_Grade: "; cin >> s.physicsGrade;


        switch (s.spec) {
        case Computer_Science:
            cout << "Programming_Grade: ";
            cin >> s.thirdGrade.programmingGrade;
            break;
        case Informatics:
            cout << "Numerical_Methods Grade: ";
            cin >> s.thirdGrade.numericalMethodsGrade;
            break;
        case Mathematics_And_Economics:
        case Physics_And_Informatics:
        case Labor_Education:
            cout << "Pedagogy_Grade: ";
            cin >> s.thirdGrade.pedagogyGrade;
            break;
        }
        cin.get();

        f.write((char*)&s, sizeof(Student));

        cout << "Continue? (Y/N): "; cin >> ch; cout << endl;

    } while (ch == 'Y' || ch == 'y');
    f.close();
    cout << endl;
}

void Print(string filename) {
    ifstream f(filename, ios::binary);
    Student s;
    int i = 0;


    cout << "======================================================================================================="
        << endl;
    cout << "| # | Last Name | Course | Specialnist        | Physics | Math | Programming | Num. Meth.  | Pedagogy |"
        << endl;

    while (f.read((char*)&s, sizeof(Student))) {
        i++;
        cout << "| " << i << " | " << setw(10) << left << s.prizv
            << " | " << setw(4) << left << s.course
            << " | " << setw(13) << left << specStr[s.spec];
        cout << " | " << setw(10) << left << s.mathGrade
            << " | " << setw(6) << left << s.physicsGrade;

        switch (s.spec) {
        case Computer_Science:
            cout << " | " << setw(13) << left << s.thirdGrade.programmingGrade <<
                " | " << setw(10) << left << "" <<
                " | " << setw(15) << left << "" << " |";
            break;
        case Informatics:
            cout << " | " << setw(13) << left << "" <<
                " | " << setw(10) << left << "" <<
                " | " << setw(15) << left << s.thirdGrade.numericalMethodsGrade << " |";
            break;
        default:
            cout << " | " << setw(13) << left << "" <<
                " | " << setw(10) << left << s.thirdGrade.pedagogyGrade <<
                " | " << setw(15) << left << "" << " |";
            break;
        }
        cout << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;
    }
    cout << "=======================================================================================================" << endl;

    f.close();

}

void BubbleSort(string filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "Error opening the file for reading." << endl;
        return;
    }

    const int maxStudents = 100;
    Student students[maxStudents];
    int N = 0;

    while (inFile.read(reinterpret_cast<char*>(&students[N]), sizeof(Student))) {
        N++;
        if (N >= maxStudents) {
            cout << "Maximum number of students reached. Cannot read more data." << endl;
            break;
        }
    }

    inFile.close();

    for (int i0 = 0; i0 < N - 1; i0++) {
        for (int i1 = 0; i1 < N - i0 - 1; i1++) {
            double grade1, grade2;

            if (students[i1].spec == Computer_Science) {
                grade1 = students[i1].thirdGrade.programmingGrade;
            }
            else if (students[i1].spec == Informatics) {
                grade1 = students[i1].thirdGrade.numericalMethodsGrade;
            }
            else {
                grade1 = students[i1].thirdGrade.pedagogyGrade;
            }

            if (students[i1 + 1].spec == Computer_Science) {
                grade2 = students[i1 + 1].thirdGrade.programmingGrade;
            }
            else if (students[i1 + 1].spec == Informatics) {
                grade2 = students[i1 + 1].thirdGrade.numericalMethodsGrade;
            }
            else {
                grade2 = students[i1 + 1].thirdGrade.pedagogyGrade;
            }

            if (grade1 < grade2) {
                swap(students[i1], students[i1 + 1]);
            }

            else if (grade1 == grade2) {
                if (students[i1].spec < students[i1 + 1].spec) {
                    swap(students[i1], students[i1 + 1]);
                }
                else if (students[i1].spec == students[i1 + 1].spec && students[i1].prizv < students[i1 + 1].prizv) {
                    swap(students[i1], students[i1 + 1]);
                }
            }
        }
    }

    ofstream outFile(filename, ios::binary);
    if (!outFile.is_open()) {
        cout << "Error opening the file for writing." << endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(students), N * sizeof(Student));

    outFile.close();
}

int* Index_Sort(string filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "Error opening the file for reading." << endl;
        return nullptr;
    }

    inFile.seekg(0, ios::end);
    int fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    int maxStudents = fileSize / sizeof(Student);
    int* indexArray = new int[maxStudents];
    Student student;

    for (int i = 0; i < maxStudents; ++i) {
        indexArray[i] = i;
    }

    for (int i0 = 0; i0 < maxStudents - 1; i0++) {
        for (int i1 = 0; i1 < maxStudents - i0 - 1; i1++) {
            double grade1, grade2;

            inFile.seekg(indexArray[i1] * sizeof(Student), ios::beg);
            inFile.read(reinterpret_cast<char*>(&student), sizeof(Student));

            if (student.spec == Computer_Science) {
                grade1 = student.thirdGrade.programmingGrade;
            }
            else if (student.spec == Informatics) {
                grade1 = student.thirdGrade.numericalMethodsGrade;
            }
            else {
                grade1 = student.thirdGrade.pedagogyGrade;
            }

            inFile.seekg(indexArray[i1 + 1] * sizeof(Student), ios::beg);
            inFile.read(reinterpret_cast<char*>(&student), sizeof(Student));

            if (student.spec == Computer_Science) {
                grade2 = student.thirdGrade.programmingGrade;
            }
            else if (student.spec == Informatics) {
                grade2 = student.thirdGrade.numericalMethodsGrade;
            }
            else {
                grade2 = student.thirdGrade.pedagogyGrade;
            }

            if (grade1 < grade2) {
                swap(indexArray[i1], indexArray[i1 + 1]);
            }

            else if (grade1 == grade2) {
                if (student.spec < student.spec) {
                    swap(indexArray[i1], indexArray[i1 + 1]);
                }

                else if (student.spec == student.spec && student.prizv < student.prizv) {
                    swap(indexArray[i1], indexArray[i1 + 1]);
                }
            }
        }
    }

    inFile.close();
    return indexArray;
}



void PrintIndexSorted(string filename, int* indexArray) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "Error opening the file for reading." << endl;
        return;
    }

    const int maxStudents = 100;
    Student students[maxStudents];

    cout << "=========================================================================================================================="
        << endl;
    cout << "| # | Last Name             | Course | Specialnist                | Physics | Math | Programming | Num. Meth.  | Pedagogy |"
        << endl;

    for (int i = 0; i < maxStudents; ++i) {
        inFile.seekg(indexArray[i] * sizeof(Student), ios::beg);
        inFile.read(reinterpret_cast<char*>(&students[i]), sizeof(Student));

        cout << "| " << setw(1) << left << students[indexArray[i]].studentNumber << " ";
        cout << "| " << setw(21) << left << students[indexArray[i]].prizv << " | " << setw(6) << left << students[indexArray[i]].course;

        switch (students[indexArray[i]].spec) {
        case Computer_Science:
            cout << " | Computer_Science ";
            break;
        case Informatics:
            cout << " | Informatics ";
            break;
        case Mathematics_And_Economics:
            cout << " | Mathematics_And_Economics ";
            break;
        case Physics_And_Informatics:
            cout << " | Physics_And_Informatics ";
            break;
        case Labor_Education:
            cout << " | Labor_Education ";
            break;
        }

        cout << "          | " << setw(7) << right << students[indexArray[i]].physicsGrade << " | " << setw(4) << left << students[indexArray[i]].mathGrade;

        switch (students[indexArray[i]].spec) {
        case Computer_Science:
            cout << " | " << setw(11) << left << students[indexArray[i]].thirdGrade.programmingGrade;
            cout << " | " << setw(11) << left << "N/A";
            cout << " | " << setw(11) << left << "N/A";
            break;
        case Informatics:
            cout << " | " << setw(11) << left << "N/A";
            cout << " | " << setw(11) << left << students[indexArray[i]].thirdGrade.numericalMethodsGrade;
            cout << " | " << setw(11) << left << "N/A";
            break;
        case Mathematics_And_Economics:
        case Physics_And_Informatics:
        case Labor_Education:
            cout << " | " << setw(11) << left << "N/A";
            cout << " | " << setw(11) << left << "N/A";
            cout << " | " << setw(11) << left << students[indexArray[i]].thirdGrade.pedagogyGrade;
            break;
        }

        cout << "|   " << endl;
    }

    cout << "=========================================================================================================================="
        << endl;
    cout << endl;

    inFile.close();

    delete[] indexArray;  
}

int BinSearch(string filename, const string prizv, const Specialnost specialnost, const double grade) {
    ifstream inFile(filename, ios::binary);
    if (!inFile.is_open()) {
        cout << "Error opening the file for reading." << endl;
        return -1;
    }

    inFile.seekg(0, ios::end);
    int fileSize = inFile.tellg();
    inFile.seekg(0, ios::beg);

    int maxStudents = fileSize / sizeof(Student);
    int left = 0, right = maxStudents - 1, mid;

    while (left <= right) {
        mid = (left + right) / 2;

        Student student;
        inFile.seekg(mid * sizeof(Student), ios::beg);
        inFile.read(reinterpret_cast<char*>(&student), sizeof(Student));

        if (student.prizv == prizv && student.spec == specialnost && student.thirdGrade.programmingGrade == grade) {
            inFile.close();
            return mid;
        }

        if (student.thirdGrade.programmingGrade < grade || student.thirdGrade.numericalMethodsGrade < grade || student.thirdGrade.pedagogyGrade < grade) {
            left = mid + 1;
        }
        else if (student.thirdGrade.programmingGrade == grade || student.thirdGrade.numericalMethodsGrade == grade || student.thirdGrade.pedagogyGrade == grade) {
            if (student.spec < specialnost) {
                left = mid + 1;
            }
            else if (student.spec == specialnost && student.prizv < prizv) {
                left = mid + 1;
            }
        }
        else {
            right = mid - 1;
        }
    }

    inFile.close();
    return -1;
}





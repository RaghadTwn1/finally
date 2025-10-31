#include <iostream>
#include <fstream>
#include <cstring>

#ifdef _WIN32
#define PAUSE system("pause")
#else
#define PAUSE system("read -p 'Press Enter to continue...' var")
#endif
//Hi
using namespace std;

void create_user();
bool login();
void main_menu();
void gpa(float*, char*, float[], int);
void create_record();
void display_record();
void display_specific();
void delete_record();
void save_records();
void load_records();

const int MAX_STUDENTS = 100;
const int MAX_PASSWORD_LENGTH = 20;

struct Student {
    int id;
    char name[50];
    float marks[5];
    float gpa;
    char grade;
};

Student students[MAX_STUDENTS];
int student_count = 0;

char username[50] = "admin";
char password[MAX_PASSWORD_LENGTH] = "admin123";

//---------------------------------------------------------
int main() {
    create_user();
    if (login()) {
        load_records();
        main_menu();
    }
    save_records();
    return 0;
}

//---------------------------------------------------------
void create_user() {
    ifstream user_name_file_check("user_name.txt");
    ifstream pass_file_check("pass.txt");

    if (!user_name_file_check.is_open() || !pass_file_check.is_open()) {
        ofstream user_name_file("user_name.txt");
        user_name_file << username;
        user_name_file.close();

        ofstream pass_file("pass.txt");
        pass_file << password;
        pass_file.close();

        cout << "Default credentials created and saved.\n";
    }
    else {
        cout << "Credential files already exist. Skipping creation.\n";
    }

    user_name_file_check.close();
    pass_file_check.close();
}

//---------------------------------------------------------
bool login() {
    char input_username[50];
    char input_password[MAX_PASSWORD_LENGTH];

    cout << "\nEnter username: ";
    cin >> input_username;
    cout << "Enter password: ";
    cin >> input_password;

    ifstream file("user_name.txt");
    file >> username;
    file.close();

    file.open("pass.txt");
    file >> password;
    file.close();

    if (strcmp(input_username, username) == 0 && strcmp(input_password, password) == 0) {
        cout << "\nLogin successful.\n";
        return true;
    }

    cout << "\nInvalid login.\n";
    cout << "Wanna try again? (Y/N): ";
    char x;
    cin >> x;
    if (x == 'Y' || x == 'y')
        return login();

    return false;
}

//---------------------------------------------------------
void main_menu() {
    int choice;
    do {
        cout << "\n----- Main Menu -----\n";
        cout << "1. Create Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Display Specific Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            create_record();
            break;
        case 2:
            display_record();
            break;
        case 3:
            display_specific();
            break;
        case 4:
            delete_record();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);
}

//---------------------------------------------------------
void gpa(float* gpa_val, char* grade, float marks[], int num_subjects) {
    float total = 0;
    for (int i = 0; i < num_subjects; i++)
        total += marks[i];

    float percentage = total / (num_subjects * 100.0);
    *gpa_val = percentage * 5;

    if (*gpa_val >= 4.5)
        *grade = 'A';
    else if (*gpa_val >= 4.0)
        *grade = 'B';
    else if (*gpa_val >= 3.5)
        *grade = 'C';
    else if (*gpa_val >= 3.0)
        *grade = 'D';
    else if (*gpa_val >= 2.5)
        *grade = 'E';
    else
        *grade = 'F';
}

//---------------------------------------------------------
void create_record() {
    if (student_count >= MAX_STUDENTS) {
        cout << "Database is full. Cannot add more students.\n";
        return;
    }

    cout << "Enter student ID: "; cin >> students[student_count].id;

    for (int i = 0; i < student_count; i++) {
        if (students[i].id == students[student_count].id) {
            cout << "This ID already exists! Enter a unique ID: ";
            cin >> students[student_count].id;
        }
    }

    cout << "Enter student name: ";
    cin.ignore();
    cin.getline(students[student_count].name, 50);

    cout << "Enter marks for 5 subjects (0-100):\n";
    for (int i = 0; i < 5; i++) {
        do {
            cout << "Mark " << (i + 1) << ": ";
            cin >> students[student_count].marks[i];
        } while (students[student_count].marks[i] < 0 || students[student_count].marks[i] > 100);
    }

    gpa(&students[student_count].gpa, &students[student_count].grade, students[student_count].marks, 5);

    student_count++;
    cout << "\nStudent record created successfully.\n";
    PAUSE;
}

//---------------------------------------------------------
void display_record() {
    cout << "\n=== All Student Records ===\n";
    if (student_count == 0) {
        cout << "No student records found.\n";
        PAUSE;
        return;
    }

    for (int i = 0; i < student_count; i++) {
        cout << "\nStudent #" << (i + 1) << endl;
        cout << "ID: " << students[i].id << endl;
        cout << "Name: " << students[i].name << endl;
        cout << "GPA: " << students[i].gpa << endl;
        cout << "Grade: " << students[i].grade << endl;
    }

    cout << "\nEnd of list.\n";
    PAUSE;
}

//---------------------------------------------------------
void display_specific() {
    cout << "\n=== Display Specific Student ===\n";

    if (student_count == 0) {
        cout << "No records available.\n";
        PAUSE;
        return;
    }

    int search_id;
    cout << "Enter student ID: ";
    cin >> search_id;

    bool found = false;
    for (int i = 0; i < student_count; i++) {
        if (students[i].id == search_id) {
            cout << "\nStudent found:\n";
            cout << "ID: " << students[i].id << endl;
            cout << "Name: " << students[i].name << endl;
            cout << "GPA: " << students[i].gpa << endl;
            cout << "Grade: " << students[i].grade << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Student with ID " << search_id << " not found.\n";

    PAUSE;
}

//---------------------------------------------------------
void delete_record() {
    int id;
    cout << "Enter student ID to delete: ";
    cin >> id;
    bool found = false;

    for (int i = 0; i < student_count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < student_count - 1; j++)
                students[j] = students[j + 1];
            student_count--;
            found = true;
            break;
        }
    }

    if (found)
        cout << "Record deleted successfully.\n";
    else
        cout << "Record not found.\n";

    PAUSE;
}

//---------------------------------------------------------
void save_records() {
    ofstream file("students.dat", ios::binary);
    if (!file) {
        cout << "Error saving file.\n";
        return;
    }

    file.write((char*)&student_count, sizeof(student_count));
    file.write((char*)students, sizeof(Student) * student_count);
    file.close();

    cout << "All records saved successfully.\n";
}

//---------------------------------------------------------
void load_records() {
    ifstream file("students.dat", ios::binary);
    if (!file) {
        cout << "No existing records found. Starting new database.\n";
        return;
    }

    file.read((char*)&student_count, sizeof(student_count));
    if (student_count < 0 || student_count > MAX_STUDENTS) {
        cout << "Data corrupted. Starting empty database.\n";
        student_count = 0;
        return;
    }

    file.read((char*)students, sizeof(Student) * student_count);
    file.close();

    cout << "Records loaded successfully.\n";
}
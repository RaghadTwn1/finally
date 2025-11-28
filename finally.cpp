#include <iostream>
#include <fstream>
#include <cstring>

//cross-platform pause macro
#ifdef _WIN32
#define PAUSE system("pause")
#else
#define PAUSE system("read -p 'Press Enter to continue...' var")
#endif

using namespace std;
//function prototypes
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

//global student limit and password length
const int MAX_STUDENTS = 100;
const int MAX_PASSWORD_LENGTH = 20;

//student structure definition  : id, name, marks, gpa, grade
struct Student {
    int id;
    char name[50];
    float marks[5];
    float gpa;
    char grade;
};

//Stores all student records
Student students[MAX_STUDENTS];
int student_count = 0;

//default admin username and pass
char username[50] = "admin";
char password[MAX_PASSWORD_LENGTH] = "admin123";

//main function
int main() {
    
	// User setup and login check , then load records and run main menu , finally save records on exit
    create_user();
    if (login()) {
        load_records();
        main_menu();
    }
    save_records();
    return 0;
}

// Create Credentials files if not exist
void create_user() {
    ifstream user_name_file_check("user_name.txt");
    ifstream pass_file_check("pass.txt");

	//if files don't exist , create them with default   Username and Password
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

	//Ask for username and password
    cout << "\nEnter username: ";
    cin >> input_username;
    cout << "Enter password: ";
    cin >> input_password;

	//Read stored username and password from files
    ifstream file("user_name.txt");
    file >> username;
    file.close();

    file.open("pass.txt");
    file >> password;
    file.close();

	//Compare input with stored credentials and allow retry on failure
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


void main_menu() {
    int choice;
	//Show the main menu and get user choices
    do {
        cout << "\n----- Main Menu -----\n";
        cout << "1. Create Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Display Specific Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

		//Run functions based on user choice
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


void gpa(float* gpa_val, char* grade, float marks[], int num_subjects) {
    float total = 0;
    
	//Calculate total marks
    for (int i = 0; i < num_subjects; i++)
        total += marks[i];

    float percentage = total / (num_subjects * 100.0);
    *gpa_val = percentage * 5;

	//Determine grade based on GPA
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


void create_record() {

	//Check if database is full before adding new student
    if (student_count >= MAX_STUDENTS) {
        cout << "Database is full. Cannot add more students.\n";
        return;
    }

    cout << "Enter student ID: "; cin >> students[student_count].id;

	// Make sure the entered ID is unique
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

	// Calculate GPA and Grade for the new student
    gpa(&students[student_count].gpa, &students[student_count].grade, students[student_count].marks, 5);

    student_count++;
    cout << "\nStudent record created successfully.\n";
    PAUSE;
}

//---------------------------------------------------------
void display_record() {
	// Show all student records
    cout << "\n=== All Student Records ===\n";

	//Check if there are any student records
    if (student_count == 0) {
        cout << "No student records found.\n";
        PAUSE;
        return;
    }

	// loop through and display each student's details
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


void display_specific() {
	// Display a specific student record by ID
    cout << "\n=== Display Specific Student ===\n";

	//if no records exist , show message and return
    if (student_count == 0) {
        cout << "No records available.\n";
        PAUSE;
        return;
    }

	// Ask for student ID to search
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


void delete_record() {
    int id;
    cout << "Enter student ID to delete: ";
    cin >> id;
    bool found = false;

	// Search for the student by ID and delete if found
    for (int i = 0; i < student_count; i++) {
        if (students[i].id == id) {
            for (int j = i; j < student_count - 1; j++)
                students[j] = students[j + 1];
            student_count--;
            found = true;
            break;
        }
    }

	//print result message
    if (found)
        cout << "Record deleted successfully.\n";
    else
        cout << "Record not found.\n";

    PAUSE;
}

//---------------------------------------------------------
void save_records() {
	//Open the binary file to save student records
    ofstream file("students.dat", ios::binary);
    if (!file) {
        cout << "Error saving file.\n";
        return;
    }

	//Write student count and records to file
    file.write((char*)&student_count, sizeof(student_count));
    file.write((char*)students, sizeof(Student) * student_count);
    file.close();

	// Confirmation message
    cout << "All records saved successfully.\n";
}


void load_records() {
    
	// Try to open the binary file to load student data
    ifstream file("students.dat", ios::binary);
    if (!file) {
        cout << "No existing records found. Starting new database.\n";
        return;
    }

	// Read student count and records from file
    file.read((char*)&student_count, sizeof(student_count));
    if (student_count < 0 || student_count > MAX_STUDENTS) {
        cout << "Data corrupted. Starting empty database.\n";
        student_count = 0;
        return;
    }

	// Load all Student records into memory
    file.read((char*)students, sizeof(Student) * student_count);
    file.close();

    cout << "Records loaded successfully.\n";
}
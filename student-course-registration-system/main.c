#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADMIN 1
#define ADVISOR 2
#define STUDENT 3

void login(int role);          // Function prototype for login function (asif-249)
void adminPanel();             // Function prototype for admin panel (asif-249)
void addStudent();             // Function prototype for adding a student by admin (asif-249)
void viewAllStudents();        // Function prototype for viewing all students by admin (asif-249)
void searchStudent();          // Function prototype for searching a student by admin (asif-249)
void updateStudent();          // Function prototype for updating a student's information by admin (asif-249)
void deleteStudent();          // Function prototype for deleting a student by admin (asif-249)
void saveStudents();           // Function prototype for saving student records (asif-249)
void loadStudents();           // Function prototype for loading student records (asif-249)
void freeStudents();           // Function prototype for freeing student memory (asif-249)
void addAdvisor();             // Function prototype for adding an advisor by admin (asif-249)
void viewAllAdvisors();        // Function prototype for viewing all advisors by admin (asif-249)
void searchAdvisor();          // Function prototype for searching an advisor by admin (asif-249)
void updateAdvisor();          // Function prototype for updating advisor information by admin (asif-249)
void deleteAdvisor();          // Function prototype for deleting an advisor by admin (asif-249)
void saveAdvisors();           // Function prototype for saving advisor records (asif-249)
void loadAdvisors();           // Function prototype for loading advisor records (asif-249)
void freeAdvisors();           // Function prototype for freeing advisor memory (asif-249)
void assignStudentToAdvisor(); // Function prototype for assigning a student to an advisor by admin (asif-249)
void viewAdvisorStudents();    // Function prototype for viewing students assigned to an advisor by admin (asif-249)
void viewAdvisorWorkload();    // Function prototype for viewing advisor workload by admin (asif-249)
void viewUnassignedStudents(); // Function prototype for viewing unassigned students by admin (asif-249)

typedef struct Setudnt // Structure to represent a student (asif-249)
{
    char studentID[21];
    char name[101];
    char department[21];
    int batch;
    char section[21];
    char email[101];
    char phone[21];
    char advisorID[51];
    char password[101];
    struct Student *nxt;
} Student;

void displayStudentDetails(Student *student); // Function prototype for displaying student details (asif-249)

typedef struct StudentFileRecord
{
    char studentID[21];
    char name[101];
    char department[21];
    int batch;
    char section[21];
    char email[101];
    char phone[21];
    char advisorID[51];
    char password[101];
} StudentFileRecord;

typedef struct Advisor
{
    char advisorID[51];
    char name[101];
    char department[21];
    char email[101];
    char phone[21];
    char password[101];
    struct Advisor *nxt;
} Advisor;

void displayAdvisorDetails(Advisor *advisor); // Function prototype for displaying advisor details (asif-249)

typedef struct AdvisorFileRecord
{
    char advisorID[51];
    char name[101];
    char department[21];
    char email[101];
    char phone[21];
    char password[101];
} AdvisorFileRecord;

Student *head = NULL;
Advisor *advisorHead = NULL;

int main()
{
    loadStudents();
    loadAdvisors();
    int choice;

    while (1)
    {
        printf("\n================================\n");
        printf(" DIU COURSE REGISTRATION SYSTEM\n");
        printf("================================\n\n");

        printf("1. Admin Login\n");
        printf("2. Advisor Login\n");
        printf("3. Student Login\n");
        printf("0. Exit\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            login(ADMIN);
            break;
        case 2:
            login(ADVISOR);
            break;
        case 3:
            login(STUDENT);
            break;
        case 0:
            freeStudents();
            freeAdvisors();
            printf("\nThank you for using DIU Course Registration System!\n");
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

// Function to handle login for different roles (asif-249)
void login(int role)
{
    char inputID[101];
    char inputPassword[101];

    char expectedID[101];
    char expectedPassword[101];
    char roleName[101];

    if (role == ADMIN)
    {
        strcpy(expectedID, "A001");
        strcpy(expectedPassword, "admin123");
        strcpy(roleName, "Admin");
    }
    else if (role == ADVISOR)
    {
        strcpy(expectedID, "ADV001");
        strcpy(expectedPassword, "advisor123");
        strcpy(roleName, "Advisor");
    }
    else if (role == STUDENT)
    {
        strcpy(expectedID, "261-15-249");
        strcpy(expectedPassword, "student123");
        strcpy(roleName, "Student");
    }
    else
    {
        printf("Invalid role!\n");
        return;
    }

    printf("\n--- %s Login ---\n", roleName);
    printf("Enter ID: ");
    scanf("%100s", inputID);
    printf("Enter Password: ");
    scanf("%100s", inputPassword);

    if (!strcmp(inputID, expectedID) && !strcmp(inputPassword, expectedPassword))
    {
        printf("\n[SUCCESS] Welcome, %s! Login successful.\n", roleName);
        if (role == ADMIN)
        {
            adminPanel();
        }
    }
    else
    {
        printf("\n[ERROR] Invalid ID or Password for %s login.\n", roleName);
    }
}

// Function to display the admin panel and handle admin operations (asif-249)
void adminPanel()
{
    int choice;

    while (1)
    {
        printf("\n========== ADMIN PANEL ==========\n");

        printf("\n---------- STUDENT MANAGEMENT ----------\n");
        printf("1. Add Student\n");
        printf("2. View All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");

        printf("\n---------- ADVISOR MANAGEMENT ----------\n");
        printf("6. Add Advisor\n");
        printf("7. View All Advisors\n");
        printf("8. Search Advisor\n");
        printf("9. Update Advisor\n");
        printf("10. Delete Advisor\n");
        printf("11. Assign Student to Advisor\n");
        printf("12. View Advisor's Assigned Students\n");
        printf("13. View Advisor Workload\n");
        printf("14. View Unassigned Students\n");

        printf("\n---------- DEPARTMENT & BATCH ----------\n");
        printf("15. Manage Departments\n");
        printf("16. Manage Batches\n");

        printf("\n---------- SECTION MANAGEMENT ----------\n");
        printf("17. Manage Sections\n");

        printf("\n---------- COURSE MANAGEMENT ----------\n");
        printf("18. Manage Courses\n");

        printf("\n---------- REGISTRATION MANAGEMENT ----------\n");
        printf("19. Manage Registration\n");

        printf("\n---------- REPORTS ----------\n");
        printf("20. View Reports\n");

        printf("\n0. Logout\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addStudent();
            break;

        case 2:
            viewAllStudents();
            break;

        case 3:
            searchStudent();
            break;

        case 4:
            updateStudent();
            break;

        case 5:
            deleteStudent();
            break;

        case 6:
            addAdvisor();
            break;

        case 7:
            viewAllAdvisors();
            break;

        case 8:
            searchAdvisor();
            break;

        case 9:
            updateAdvisor();
            break;

        case 10:
            deleteAdvisor();
            break;

        case 11:
            assignStudentToAdvisor();
            break;

        case 12:
            viewAdvisorStudents();
            break;

        case 13:
            viewAdvisorWorkload();
            break;

        case 14:
            viewUnassignedStudents();
            break;

        case 15:
            printf("\nDepartment Management feature is coming soon!\n");
            break;

        case 16:
            printf("\nBatch Management feature is coming soon!\n");
            break;

        case 17:
            printf("\nSection Management feature is coming soon!\n");
            break;

        case 18:
            printf("\nCourse Management feature is coming soon!\n");
            break;

        case 19:
            printf("\nRegistration Management feature is coming soon!\n");
            break;

        case 20:
            printf("\nReports feature is coming soon!\n");
            break;

        case 0:
            printf("\nLogging out from Admin Panel...\n");
            return;

        default:
            printf("\nInvalid choice! Please try again.\n");
        }
    }
}

// Function to add a new student to the system by admin (asif-249)
void addStudent()
{
    printf("\n--- Add New Student ---\n\n");

    char newStudentID[21];

    printf("Enter Student ID: ");
    scanf("%20s", newStudentID);

    Student *temp = head;
    while (temp != NULL)
    {
        if (!strcmp(temp->studentID, newStudentID))
        {
            printf("\nError! This Student ID already exists.\n");
            return;
        }
        temp = temp->nxt;
    }

    Student *newStudent = malloc(sizeof(Student));

    if (newStudent == NULL)
    {
        printf("\nMemory allocation failed!\n");
        return;
    }

    strcpy(newStudent->studentID, newStudentID);

    printf("Enter Student Name: ");
    scanf(" %100[^\n]", newStudent->name);
    printf("Enter Department: ");
    scanf("%20s", newStudent->department);
    printf("Enter Batch: ");
    scanf("%d", &newStudent->batch);
    printf("Enter Section: ");
    scanf("%20s", newStudent->section);
    printf("Enter Email: ");
    scanf("%100s", newStudent->email);
    printf("Enter Phone: ");
    scanf("%20s", newStudent->phone);
    // printf("Enter Advisor ID: ");
    // scanf("%50s", newStudent->advisorID);
    strcpy(newStudent->advisorID, ""); // Initialize advisor ID as empty becase it will be assigned later with assignStudentToAdvisor();
    printf("Enter Password: ");
    scanf("%100s", newStudent->password);

    newStudent->nxt = NULL;

    if (head == NULL)
    {
        head = newStudent;
    }
    else
    {
        temp = head;
        while (temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = newStudent;
    }

    printf("\n[SUCCESS] Student added successfully!\n");
    saveStudents();
}

// Function to display student details (asif-249)
void displayStudentDetails(Student *student)
{
    printf("Student ID: %s\n", student->studentID);
    printf("Name: %s\n", student->name);
    printf("Department: %s\n", student->department);
    printf("Batch: %d\n", student->batch);
    printf("Section: %s\n", student->section);
    printf("Email: %s\n", student->email);
    printf("Phone: %s\n", student->phone);
    printf("Advisor ID: %s\n", student->advisorID);

    printf("=====================================\n");
}

// Function to view all students in the system by admin (asif-249)
void viewAllStudents()
{
    printf("\n--- View All Students ---\n");

    if (head == NULL)
    {
        printf("No students found in the system.\n");
        return;
    }

    int count = 0;
    Student *temp = head;
    while (temp != NULL)
    {
        printf("\n--- Student %d ---\n", count + 1);
        displayStudentDetails(temp);
        temp = temp->nxt;
        count++;
    }
    printf("\nTotal students in the system: %d\n", count);
}

// Function to search for a student in the system by admin (asif-249)
void searchStudent()
{
    printf("\n--- Search Student ---\n");

    if (head == NULL)
    {
        printf("\nNo students available to search!\n");
        return;
    }

    char searchID[21];
    printf("Enter Student ID: ");
    scanf("%20s", searchID);

    Student *temp = head;
    while (temp != NULL)
    {
        if (!strcmp(temp->studentID, searchID))
        {
            printf("\n[SUCCESS] Student found!\n");
            printf("\n========== Student Details ==========\n");
            displayStudentDetails(temp);
            return;
        }
        temp = temp->nxt;
    }
    printf("\n[ERROR] No student found with ID: %s\n", searchID);
}

// Function to update a student's information by admin (asif-249)
void updateStudent()
{
    printf("\n--- Update Student Information ---\n");

    if (head == NULL)
    {
        printf("\nNo students available to update!\n");
        return;
    }

    char searchID[21];
    printf("Enter Student ID: ");
    scanf("%20s", searchID);

    Student *temp = head;
    while (temp != NULL)
    {
        if (!strcmp(temp->studentID, searchID))
        {
            break;
        }
        temp = temp->nxt;
    }

    if (temp == NULL)
    {
        printf("\n[ERROR] No student found with ID: %s\n", searchID);
        return;
    }

    printf("\n[SUCCESS] Student found!\n");

    printf("\n--- Current Student Information ---\n");
    displayStudentDetails(temp);

    int choice;

    while (1)
    {
        printf("\n========== Update Menu ==========\n");
        printf("1. Update Name\n");
        printf("2. Update Department\n");
        printf("3. Update Batch\n");
        printf("4. Update Section\n");
        printf("5. Update Email\n");
        printf("6. Update Phone\n");
        // printf("7. Update Advisor ID\n");
        printf("0. Finish Updating\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter New Name: ");
            scanf(" %100[^\n]", temp->name);
            printf("\n[SUCCESS] Name updated successfully!\n");
            break;

        case 2:
            printf("Enter New Department: ");
            scanf("%20s", temp->department);
            printf("\n[SUCCESS] Department updated successfully!\n");
            break;

        case 3:
            printf("Enter New Batch: ");
            scanf("%d", &temp->batch);
            printf("\n[SUCCESS] Batch updated successfully!\n");
            break;

        case 4:
            printf("Enter New Section: ");
            scanf("%20s", temp->section);
            printf("\n[SUCCESS] Section updated successfully!\n");
            break;

        case 5:
            printf("Enter New Email: ");
            scanf("%100s", temp->email);
            printf("\n[SUCCESS] Email updated successfully!\n");
            break;

        case 6:
            printf("Enter New Phone: ");
            scanf("%20s", temp->phone);
            printf("\n[SUCCESS] Phone updated successfully!\n");
            break;

            // case 7:
            //     printf("Enter New Advisor ID: ");
            //     scanf("%50s", temp->advisorID);
            //     printf("\n[SUCCESS] Advisor ID updated successfully!\n");
            //     break;

        case 0:
            printf("\n[INFO] Updating process completed!\n");
            printf("\n--- Updated Student Information ---\n");
            displayStudentDetails(temp);
            saveStudents();
            return;

        default:
            printf("\n[ERROR] Invalid choice! Please try again.\n");
        }
    }
}

// Function to delete a student from the system by admin (asif-249)
void deleteStudent()
{
    printf("\n--- Delete Student ---\n");

    if (head == NULL)
    {
        printf("\nNo students available to delete!\n");
        return;
    }

    char searchID[21];
    printf("\nEnter Student ID: ");
    scanf("%20s", searchID);

    Student *temp = head;
    Student *prev = NULL;

    while (temp != NULL)
    {
        if (!strcmp(temp->studentID, searchID))
        {
            break;
        }
        prev = temp;
        temp = temp->nxt;
    }

    if (temp == NULL)
    {
        printf("\n[ERROR] No student found with ID: %s\n", searchID);
        return;
    }

    printf("\n--- Student Information ---\n");
    displayStudentDetails(temp);

    char confirm;
    printf("\nAre you sure you want to delete this student? (Y/N): ");
    scanf(" %c", &confirm);

    if (confirm == 'Y' || confirm == 'y')
    {
        if (prev == NULL)
        {
            head = temp->nxt;
        }
        else
        {
            prev->nxt = temp->nxt;
        }

        free(temp);
        printf("\n[SUCCESS] Student deleted successfully!\n");
        saveStudents();
    }
    else
    {
        printf("\n[INFO] Student deletion cancelled.\n");
    }
}

// Function to save student records to a binary file (asif-249)
void saveStudents()
{
    FILE *fp = fopen("students.dat", "wb");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open student file!\n");
        return;
    }

    Student *temp = head;
    StudentFileRecord record;

    while (temp != NULL)
    {
        strcpy(record.studentID, temp->studentID);
        strcpy(record.name, temp->name);
        strcpy(record.department, temp->department);
        record.batch = temp->batch;
        strcpy(record.section, temp->section);
        strcpy(record.email, temp->email);
        strcpy(record.phone, temp->phone);
        strcpy(record.advisorID, temp->advisorID);
        strcpy(record.password, temp->password);

        fwrite(&record, sizeof(StudentFileRecord), 1, fp);

        temp = temp->nxt;
    }

    fclose(fp);

    printf("\n[SUCCESS] Student records saved successfully!\n");
}

// Function to load student records from a binary file (asif-249)
void loadStudents()
{
    FILE *fp = fopen("students.dat", "rb");

    if (fp == NULL)
    {
        printf("\n[INFO] No existing student records found.\n");
        return;
    }

    StudentFileRecord record;

    while (fread(&record, sizeof(StudentFileRecord), 1, fp) == 1)
    {
        Student *newStudent = malloc(sizeof(Student));

        if (newStudent == NULL)
        {
            printf("\n[ERROR] Memory allocation failed!\n");
            fclose(fp);
            return;
        }

        strcpy(newStudent->studentID, record.studentID);
        strcpy(newStudent->name, record.name);
        strcpy(newStudent->department, record.department);
        newStudent->batch = record.batch;
        strcpy(newStudent->section, record.section);
        strcpy(newStudent->email, record.email);
        strcpy(newStudent->phone, record.phone);
        strcpy(newStudent->advisorID, record.advisorID);
        strcpy(newStudent->password, record.password);

        newStudent->nxt = NULL;

        if (head == NULL)
        {
            head = newStudent;
        }
        else
        {
            Student *temp = head;

            while (temp->nxt != NULL)
            {
                temp = temp->nxt;
            }

            temp->nxt = newStudent;
        }
    }

    fclose(fp);

    printf("\n[SUCCESS] Student records loaded successfully!\n");
}

// Function to free all dynamically allocated student nodes
void freeStudents()
{
    Student *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->nxt;

        free(temp);
    }

    printf("\n[SUCCESS] Student memory released successfully!\n");
}

// Function to add a new advisor to the system by admin (asif-249)
void addAdvisor()
{
    printf("\n--- Add New Advisor ---\n\n");

    char newAdvisorID[51];

    printf("Enter Advisor ID: ");
    scanf("%50s", newAdvisorID);

    Advisor *temp = advisorHead;
    while (temp != NULL)
    {
        if (!strcmp(temp->advisorID, newAdvisorID))
        {
            printf("\n[ERROR] This Advisor ID already exists!\n");
            return;
        }
        temp = temp->nxt;
    }

    Advisor *newAdvisor = malloc(sizeof(Advisor));

    if (newAdvisor == NULL)
    {
        printf("\n[ERROR] Memory allocation failed!\n");
        return;
    }

    strcpy(newAdvisor->advisorID, newAdvisorID);

    printf("Enter Advisor Name: ");
    scanf(" %100[^\n]", newAdvisor->name);
    printf("Enter Department: ");
    scanf("%20s", newAdvisor->department);
    printf("Enter Email: ");
    scanf("%100s", newAdvisor->email);
    printf("Enter Phone: ");
    scanf("%20s", newAdvisor->phone);
    printf("Enter Password: ");
    scanf("%100s", newAdvisor->password);

    newAdvisor->nxt = NULL;

    if (advisorHead == NULL)
    {
        advisorHead = newAdvisor;
    }
    else
    {
        temp = advisorHead;
        while (temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = newAdvisor;
    }

    printf("\n[SUCCESS] Advisor added successfully!\n");
    saveAdvisors();
}

// Function to display advisor details (asif-249)
void displayAdvisorDetails(Advisor *advisor)
{
    printf("Advisor ID: %s\n", advisor->advisorID);
    printf("Name: %s\n", advisor->name);
    printf("Department: %s\n", advisor->department);
    printf("Email: %s\n", advisor->email);
    printf("Phone: %s\n", advisor->phone);

    printf("=====================================\n");
}

// Function to view all advisors in the system by admin (asif-249)
void viewAllAdvisors()
{
    printf("\n--- View All Advisors ---\n");

    if (advisorHead == NULL)
    {
        printf("\nNo advisors found in the system.\n");
        return;
    }

    int count = 0;
    Advisor *temp = advisorHead;

    while (temp != NULL)
    {
        printf("\n--- Advisor %d ---\n", count + 1);
        displayAdvisorDetails(temp);
        temp = temp->nxt;
        count++;
    }
    printf("\nTotal advisors in the system: %d\n", count);
}

// Function to search for an advisor in the system by admin (asif-249)
void searchAdvisor()
{
    printf("\n--- Search Advisor ---\n");

    if (advisorHead == NULL)
    {
        printf("\nNo advisors available to search!\n");
        return;
    }

    char searchID[51];
    printf("Enter Advisor ID: ");
    scanf("%50s", searchID);

    Advisor *temp = advisorHead;
    while (temp != NULL)
    {
        if (!strcmp(temp->advisorID, searchID))
        {
            printf("\n[SUCCESS] Advisor found!\n");
            printf("\n========== Advisor Details ==========\n");
            displayAdvisorDetails(temp);
            return;
        }
        temp = temp->nxt;
    }
    printf("\n[ERROR] No advisor found with ID: %s\n", searchID);
}

// Function to update advisor information by admin (asif-249)
void updateAdvisor()
{
    printf("\n--- Update Advisor ---\n");

    if (advisorHead == NULL)
    {
        printf("\nNo advisors available to update!\n");
        return;
    }

    char searchID[51];
    printf("Enter Advisor ID: ");
    scanf("%50s", searchID);

    Advisor *temp = advisorHead;
    while (temp != NULL)
    {
        if (!strcmp(temp->advisorID, searchID))
        {
            break;
        }
        temp = temp->nxt;
    }

    if (temp == NULL)
    {
        printf("\n[ERROR] No advisor found with ID: %s\n", searchID);
        return;
    }

    printf("\n[SUCCESS] Advisor found!\n");
    printf("\n========== Current Advisor Details ==========\n");
    displayAdvisorDetails(temp);

    int choice;
    while (1)
    {
        printf("\n========== Update Advisor Information ==========\n");
        printf("1. Update Name\n");
        printf("2. Update Department\n");
        printf("3. Update Email\n");
        printf("4. Update Phone\n");
        printf("5. Update Password\n");
        printf("0. Finish Updating\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter New Name: ");
            scanf(" %100[^\n]", temp->name);
            printf("\n[SUCCESS] Name updated successfully!\n");
            break;

        case 2:
            printf("Enter New Department: ");
            scanf("%20s", temp->department);
            printf("\n[SUCCESS] Department updated successfully!\n");
            break;

        case 3:
            printf("Enter New Email: ");
            scanf("%100s", temp->email);
            printf("\n[SUCCESS] Email updated successfully!\n");
            break;

        case 4:
            printf("Enter New Phone: ");
            scanf("%20s", temp->phone);
            printf("\n[SUCCESS] Phone updated successfully!\n");
            break;

        case 5:
            printf("Enter New Password: ");
            scanf("%100s", temp->password);
            printf("\n[SUCCESS] Password updated successfully!\n");
            break;

        case 0:
            printf("\n[SUCCESS] Advisor information updating finished!\n");
            printf("\n========== Updated Advisor Details ==========\n");
            displayAdvisorDetails(temp);
            saveAdvisors();
            return;

        default:
            printf("\n[ERROR] Invalid choice! Please try again.\n");
        }
    }
}

// Function to delete an advisor from the system by admin (asif-249)
void deleteAdvisor()
{
    printf("\n--- Delete Advisor ---\n");

    if (advisorHead == NULL)
    {
        printf("\nNo advisors available to delete!\n");
        return;
    }

    char searchID[51];
    printf("Enter Advisor ID: ");
    scanf("%50s", searchID);

    Advisor *temp = advisorHead;
    Advisor *prev = NULL;
    while (temp != NULL)
    {
        if (!strcmp(temp->advisorID, searchID))
        {
            break;
        }
        prev = temp;
        temp = temp->nxt;
    }

    if (temp == NULL)
    {
        printf("\n[ERROR] No advisor found with ID: %s\n", searchID);
        return;
    }

    printf("\n========== Advisor Details ==========\n");
    displayAdvisorDetails(temp);

    // Check whether any students are assigned to this advisor (asif-249)
    Student *studentTemp = head;
    while (studentTemp != NULL)
    {
        if (!strcmp(studentTemp->advisorID, searchID))
        {
            printf("\n[ERROR] Cannot delete this advisor!\n");
            printf("This advisor currently has assigned students.\n");
            printf("Please reassign those students before deleting the advisor.\n");
            return;
        }
        studentTemp = studentTemp->nxt;
    }

    char confirm;
    printf("\nAre you sure you want to delete this advisor? (Y/N): ");
    scanf(" %c", &confirm);

    if (confirm == 'Y' || confirm == 'y')
    {
        if (prev == NULL)
        {
            advisorHead = temp->nxt;
        }
        else
        {
            prev->nxt = temp->nxt;
        }

        free(temp);
        printf("\n[SUCCESS] Advisor deleted successfully!\n");
        saveAdvisors();
    }
    else
    {
        printf("\n[INFO] Advisor deletion cancelled.\n");
    }
}

// Function to save advisor records to a binary file (asif-249)
void saveAdvisors()
{
    FILE *fp = fopen("advisors.dat", "wb");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open advisor file!\n");
        return;
    }

    Advisor *temp = advisorHead;
    AdvisorFileRecord record;

    while (temp != NULL)
    {
        strcpy(record.advisorID, temp->advisorID);
        strcpy(record.name, temp->name);
        strcpy(record.department, temp->department);
        strcpy(record.email, temp->email);
        strcpy(record.phone, temp->phone);
        strcpy(record.password, temp->password);

        fwrite(&record, sizeof(AdvisorFileRecord), 1, fp);

        temp = temp->nxt;
    }

    fclose(fp);

    printf("\n[SUCCESS] Advisor records saved successfully!\n");
}

// Function to load advisor records from a binary file (asif-249)
void loadAdvisors()
{
    FILE *fp = fopen("advisors.dat", "rb");

    if (fp == NULL)
    {
        printf("\n[INFO] No existing advisor records found.\n");
        return;
    }

    AdvisorFileRecord record;

    while (fread(&record, sizeof(AdvisorFileRecord), 1, fp) == 1)
    {
        Advisor *newAdvisor = malloc(sizeof(Advisor));

        if (newAdvisor == NULL)
        {
            printf("\n[ERROR] Memory allocation failed!\n");
            fclose(fp);
            return;
        }

        strcpy(newAdvisor->advisorID, record.advisorID);
        strcpy(newAdvisor->name, record.name);
        strcpy(newAdvisor->department, record.department);
        strcpy(newAdvisor->email, record.email);
        strcpy(newAdvisor->phone, record.phone);
        strcpy(newAdvisor->password, record.password);

        newAdvisor->nxt = NULL;

        if (advisorHead == NULL)
        {
            advisorHead = newAdvisor;
        }
        else
        {
            Advisor *temp = advisorHead;
            while (temp->nxt != NULL)
            {
                temp = temp->nxt;
            }
            temp->nxt = newAdvisor;
        }
    }

    fclose(fp);
    printf("\n[SUCCESS] Advisor records loaded successfully!\n");
}

// Function to free all dynamically allocated advisor nodes (asif-249)
void freeAdvisors()
{
    Advisor *temp;
    while (advisorHead != NULL)
    {
        temp = advisorHead;
        advisorHead = advisorHead->nxt;
        free(temp);
    }
    printf("\n[SUCCESS] Advisor memory released successfully!\n");
}

// Function to assign an advisor to a student (asif-249)
void assignStudentToAdvisor()
{
    printf("\n--- Assign Student to Advisor ---\n");

    if (head == NULL)
    {
        printf("\n[ERROR] No students available!\n");
        return;
    }
    if (advisorHead == NULL)
    {
        printf("\n[ERROR] No advisors available!\n");
        return;
    }

    char searchStudentID[21];
    char searchAdvisorID[51];
    printf("\nEnter Student ID: ");
    scanf("%20s", searchStudentID);

    Student *studentTemp = head;
    while (studentTemp != NULL)
    {
        if (!strcmp(studentTemp->studentID, searchStudentID))
        {
            break;
        }
        studentTemp = studentTemp->nxt;
    }
    if (studentTemp == NULL)
    {
        printf("\n[ERROR] Student not found!\n");
        return;
    }
    printf("\n[SUCCESS] Student found!\n");
    displayStudentDetails(studentTemp);

    printf("\nEnter Advisor ID: ");
    scanf("%50s", searchAdvisorID);

    Advisor *advisorTemp = advisorHead;
    while (advisorTemp != NULL)
    {
        if (!strcmp(advisorTemp->advisorID, searchAdvisorID))
        {
            break;
        }
        advisorTemp = advisorTemp->nxt;
    }
    if (advisorTemp == NULL)
    {
        printf("\n[ERROR] Advisor not found!\n");
        return;
    }
    printf("\n[SUCCESS] Advisor found!\n");
    displayAdvisorDetails(advisorTemp);

    if (strcmp(studentTemp->department, advisorTemp->department) != 0)
    {
        printf("\n[ERROR] Department mismatch!\n");
        printf("Student and advisor must belong to the same department.\n");
        return;
    }

    if (studentTemp->advisorID[0] != '\0')
    {
        printf("\n[INFO] This student already has an assigned advisor.\n");
        printf("Current Advisor ID: %s\n", studentTemp->advisorID);

        char confirm;
        printf("\nDo you want to change the assigned advisor? (Y/N): ");
        scanf(" %c", &confirm);

        if (confirm != 'Y' && confirm != 'y')
        {
            printf("\n[INFO] Advisor reassignment cancelled.\n");
            return;
        }
    }

    strcpy(studentTemp->advisorID, advisorTemp->advisorID);

    printf("\n[SUCCESS] Advisor assigned successfully!\n");
    printf("Student ID: %s\n", studentTemp->studentID);
    printf("Assigned Advisor ID: %s\n", studentTemp->advisorID);
    saveStudents();
}

// Function to view students assigned to a particular advisor (asif-249)
void viewAdvisorStudents()
{
    printf("\n--- View Advisor's Assigned Students ---\n");

    if (advisorHead == NULL)
    {
        printf("\n[ERROR] No advisors available!\n");
        return;
    }

    char searchID[51];
    printf("\nEnter Advisor ID: ");
    scanf("%50s", searchID);

    Advisor *advisorTemp = advisorHead;
    while (advisorTemp != NULL)
    {
        if (!strcmp(advisorTemp->advisorID, searchID))
        {
            break;
        }
        advisorTemp = advisorTemp->nxt;
    }
    if (advisorTemp == NULL)
    {
        printf("\n[ERROR] Advisor not found!\n");
        return;
    }
    printf("\n========== Advisor Details ==========\n");
    displayAdvisorDetails(advisorTemp);

    Student *studentTemp = head;
    int count = 0;

    printf("\n========== Assigned Students ==========\n");

    while (studentTemp != NULL)
    {
        if (!strcmp(studentTemp->advisorID, searchID))
        {
            count++;
            printf("\n--- Student %d ---\n", count);
            displayStudentDetails(studentTemp);
        }
        studentTemp = studentTemp->nxt;
    }
    if (count == 0)
    {
        printf("\nNo students are currently assigned to this advisor.\n");
    }
    printf("\nTotal assigned students: %d\n", count);
}

// Function to view the workload of all advisors (asif-249)
void viewAdvisorWorkload()
{
    printf("\n--- View Advisor Workload ---\n");

    if (advisorHead == NULL)
    {
        printf("\n[ERROR] No advisors available!\n");
        return;
    }

    Advisor *advisorTemp = advisorHead;
    int totalAdvisors = 0;
    int totalAssignedStudents = 0;

    printf("\n========== ADVISOR WORKLOAD ==========\n");

    while (advisorTemp != NULL)
    {
        Student *studentTemp = head;
        int studentCount = 0;

        while (studentTemp != NULL)
        {
            if (!strcmp(studentTemp->advisorID, advisorTemp->advisorID))
            {
                studentCount++;
            }

            studentTemp = studentTemp->nxt;
        }

        printf("\nAdvisor ID: %s\n", advisorTemp->advisorID);
        printf("Advisor Name: %s\n", advisorTemp->name);
        printf("Department: %s\n", advisorTemp->department);
        printf("Assigned Students: %d\n", studentCount);
        printf("-------------------------------------\n");

        totalAdvisors++;
        totalAssignedStudents += studentCount;

        advisorTemp = advisorTemp->nxt;
    }

    // int totalStudents = 0;
    // Student *studentTemp = head;
    // while (studentTemp != NULL)
    // {
    //     totalStudents++;
    //     studentTemp = studentTemp->nxt;
    // }

    int totalUnassignedStudents = 0;
    Student *studentTemp = head;
    while (studentTemp != NULL)
    {
        if (studentTemp->advisorID[0] == '\0')
        {
            totalUnassignedStudents++;
        }
        studentTemp = studentTemp->nxt;
    }

    printf("\n========== WORKLOAD SUMMARY ==========\n");
    printf("Total Advisors: %d\n", totalAdvisors);
    printf("Total Assigned Students: %d\n", totalAssignedStudents);
    printf("Total Unassigned Students: %d\n", totalUnassignedStudents);
}

// Function to view all students without an assigned advisor (asif-249)
void viewUnassignedStudents()
{
    printf("\n--- View All Unassigned Students ---\n");

    if (head == NULL)
    {
        printf("\n[ERROR] No students available!\n");
        return;
    }

    Student *studentTemp = head;
    int count = 0;

    printf("\n========== UNASSIGNED STUDENTS ==========\n");

    while (studentTemp != NULL)
    {
        if (studentTemp->advisorID[0] == '\0')
        {
            count++;

            printf("\n--- Student %d ---\n", count);
            displayStudentDetails(studentTemp);
        }
        studentTemp = studentTemp->nxt;
    }
    if (count == 0)
    {
        printf("\nAll students have assigned advisors!\n");
    }
    printf("\nTotal unassigned students: %d\n", count);
}

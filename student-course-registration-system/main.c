#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADMIN 1
#define ADVISOR 2
#define STUDENT 3

// Function Prototypes
void loadAdmins();
void loadStudents();
void loadAdvisors();               

void login(int role);                          
void adminPanel();                             
void addStudent();                             
void viewAllStudents();                        
void searchStudent();                          
void updateStudent();                          
void deleteStudent();                          
void saveStudents();                           
void freeStudents();                           
void addAdvisor();                             
void viewAllAdvisors();                        
void searchAdvisor();                          
void updateAdvisor();                          
void deleteAdvisor();                          
void saveAdvisors();                           
void freeAdvisors();                           
void assignStudentToAdvisor();                 
void viewAdvisorStudents();                    
void viewAdvisorWorkload();                    
void viewUnassignedStudents();                 
void advisorPanel(char advisorID[]);           
void viewMyAssignedStudents(char advisorID[]); 
void studentPanel(char studentID[]); 
void displayStudentDetails(Student *student); 

// Data Structures
typedef struct Admin
{
    char adminID[51];
    char name[101];
    char email[101];
    char password[101];
    struct Admin *nxt;
} Admin;

typedef struct Student 
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


Admin *adminHead = NULL;
Student *studentHead = NULL;
Advisor *advisorHead = NULL;


int main()
{
    loadAdmins();
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
            freeAdmins();
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


//Load Persistent Records from Text Files into Singly Linked Lists (Admin, Student, Advisor)
void loadAdmins()
{
    FILE *fp = fopen("admins.txt", "r");

    if (fp == NULL)
    {
        printf("\n[INFO] No existing admin records found.\n");
        return;
    }

    while (1)
    {
        Admin *newAdmin = malloc(sizeof(Admin));

        if (newAdmin == NULL)
        {
            printf("\n[ERROR] Memory allocation failed!\n");
            fclose(fp);
            return;
        }

        int result = fscanf(fp, "%50[^|]|%100[^|]|%100[^|]|%100[^\n]\n",
                            newAdmin->adminID,
                            newAdmin->name,
                            newAdmin->email,
                            newAdmin->password);

        if (result != 4)
        {
            free(newAdmin);
            break;
        }

        newAdmin->nxt = NULL;

        if (adminHead == NULL)
        {
            adminHead = newAdmin;
        }
        else
        {
            Admin *temp = adminHead;

            while (temp->nxt != NULL)
            {
                temp = temp->nxt;
            }

            temp->nxt = newAdmin;
        }
    }

    fclose(fp);
    printf("\n[SUCCESS] Admin records loaded successfully!\n");
}

void loadStudents()
{
    FILE *fp = fopen("students.txt", "r");

    if (fp == NULL)
    {
        printf("\n[INFO] No existing student records found.\n");
        return;
    }

    Student *newStudent;
    while (1)
    {
        newStudent = malloc(sizeof(Student));
        if (newStudent == NULL)
        {
            printf("\n[ERROR] Memory allocation failed!\n");
            fclose(fp);
            return;
        }
        int result = fscanf(fp, "%20[^|]|%100[^|]|%20[^|]|%d|%20[^|]|%100[^|]|%20[^|]|%50[^|]|%100[^\n]\n",
                            newStudent->studentID,
                            newStudent->name,
                            newStudent->department,
                            &newStudent->batch,
                            newStudent->section,
                            newStudent->email,
                            newStudent->phone,
                            newStudent->advisorID,
                            newStudent->password);

        if (result != 9)
        {
            free(newStudent);
            break;
        }
        if (strcmp(newStudent->advisorID, "-") == 0)
        {
            newStudent->advisorID[0] = '\0';
        }
        newStudent->nxt = NULL;
        if (studentHead == NULL)
        {
            studentHead = newStudent;
        }
        else
        {
            Student *temp = studentHead;
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

void loadAdvisors()
{
    FILE *fp = fopen("advisors.txt", "r");

    if (fp == NULL)
    {
        printf("\n[INFO] No existing advisor records found.\n");
        return;
    }

    while (1)
    {
        Advisor *newAdvisor = malloc(sizeof(Advisor));

        if (newAdvisor == NULL)
        {
            printf("\n[ERROR] Memory allocation failed!\n");
            fclose(fp);
            return;
        }

        int result = fscanf(fp, "%50[^|]|%100[^|]|%20[^|]|%100[^|]|%20[^|]|%100[^\n]\n",
                            newAdvisor->advisorID,
                            newAdvisor->name,
                            newAdvisor->department,
                            newAdvisor->email,
                            newAdvisor->phone,
                            newAdvisor->password);

        if (result != 6)
        {
            free(newAdvisor);
            break;
        }

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



void login(int role)
{
    char inputID[101];
    char inputPassword[101];
    char roleName[101];

    if (role == ADMIN)
    {
        strcpy(roleName, "Admin");
    }
    else if (role == ADVISOR)
    {
        strcpy(roleName, "Advisor");
    }
    else if (role == STUDENT)
    {
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

    if (role == ADMIN)
    {
        Admin *temp = adminHead;
        int found = 0;

        while (temp != NULL)
        {
            if (!strcmp(inputID, temp->adminID) && !strcmp(inputPassword, temp->password))
            {
                found = 1;
                break;
            }
            temp = temp->nxt;
        }
        if (found)
        {
            printf("\n[SUCCESS] Welcome, %s! Login successful.\n", temp->name);
            adminPanel();
        }
        else
        {
            printf("\n[ERROR] Invalid ID or Password for Admin login.\n");
        }
    }
    else if (role == ADVISOR)
    {
        Advisor *temp = advisorHead;
        int found = 0;

        while (temp != NULL)
        {
            if (!strcmp(inputID, temp->advisorID) && !strcmp(inputPassword, temp->password))
            {
                found = 1;
                break;
            }
            temp = temp->nxt;
        }
        if (found)
        {
            printf("\n[SUCCESS] Welcome, %s! Login successful.\n", temp->name);
            advisorPanel(inputID);
        }
        else
        {
            printf("\n[ERROR] Invalid ID or Password for Advisor login.\n");
        }
    }
    else if (role == STUDENT)
    {
        Student *temp = studentHead;
        int found = 0;

        while (temp != NULL)
        {
            if (!strcmp(inputID, temp->studentID) && !strcmp(inputPassword, temp->password))
            {
                found = 1;
                break;
            }
            temp = temp->nxt;
        }
        if (found)
        {
            printf("\n[SUCCESS] Welcome, %s! Login successful.\n", temp->name);
            studentPanel(inputID);
        }
        else
        {
            printf("\n[ERROR] Invalid ID or Password for Student login.\n");
        }
    }
}

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

void saveAdmins()
{
    FILE *fp = fopen("admins.txt", "w");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open admin file!\n");
        return;
    }

    Admin *temp = adminHead;
    while (temp != NULL)
    {
        fprintf(fp, "%s|%s|%s|%s\n",
                temp->adminID,
                temp->name,
                temp->email,
                temp->password);

        temp = temp->nxt;
    }

    fclose(fp);
    printf("\n[SUCCESS] Admin records saved successfully!\n");
}


void addStudent()
{
    printf("\n--- Add New Student ---\n\n");

    char newStudentID[21];

    printf("Enter Student ID: ");
    scanf("%20s", newStudentID);

    Student *temp = studentHead;
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

    if (studentHead == NULL)
    {
        studentHead = newStudent;
    }
    else
    {
        temp = studentHead;
        while (temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = newStudent;
    }

    printf("\n[SUCCESS] Student added successfully!\n");
    saveStudents();
}

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

void viewAllStudents()
{
    printf("\n--- View All Students ---\n");

    if (studentHead == NULL)
    {
        printf("No students found in the system.\n");
        return;
    }

    int count = 0;
    Student *temp = studentHead;
    while (temp != NULL)
    {
        printf("\n--- Student %d ---\n", count + 1);
        displayStudentDetails(temp);
        temp = temp->nxt;
        count++;
    }
    printf("\nTotal students in the system: %d\n", count);
}

void searchStudent()
{
    printf("\n--- Search Student ---\n");

    if (studentHead == NULL)
    {
        printf("\nNo students available to search!\n");
        return;
    }

    char searchID[21];
    printf("Enter Student ID: ");
    scanf("%20s", searchID);

    Student *temp = studentHead;
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

void updateStudent()
{
    printf("\n--- Update Student Information ---\n");

    if (studentHead == NULL)
    {
        printf("\nNo students available to update!\n");
        return;
    }

    char searchID[21];
    printf("Enter Student ID: ");
    scanf("%20s", searchID);

    Student *temp = studentHead;
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

void deleteStudent()
{
    printf("\n--- Delete Student ---\n");

    if (studentHead == NULL)
    {
        printf("\nNo students available to delete!\n");
        return;
    }

    char searchID[21];
    printf("\nEnter Student ID: ");
    scanf("%20s", searchID);

    Student *temp = studentHead;
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
            studentHead = temp->nxt;
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

void saveStudents()
{
    FILE *fp = fopen("students.txt", "w");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open student file!\n");
        return;
    }

    Student *temp = studentHead;
    while (temp != NULL)
    {
        fprintf(fp, "%s|%s|%s|%d|%s|%s|%s|%s|%s\n",
                temp->studentID,
                temp->name,
                temp->department,
                temp->batch,
                temp->section,
                temp->email,
                temp->phone,
                (temp->advisorID[0] == '\0') ? "-" : temp->advisorID,
                temp->password);

        temp = temp->nxt;
    }

    fclose(fp);
    printf("\n[SUCCESS] Student records saved successfully!\n");
}



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

void displayAdvisorDetails(Advisor *advisor)
{
    printf("Advisor ID: %s\n", advisor->advisorID);
    printf("Name: %s\n", advisor->name);
    printf("Department: %s\n", advisor->department);
    printf("Email: %s\n", advisor->email);
    printf("Phone: %s\n", advisor->phone);

    printf("=====================================\n");
}

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

    Student *studentTemp = studentHead;
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

void saveAdvisors()
{
    FILE *fp = fopen("advisors.txt", "w");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open advisor file!\n");
        return;
    }

    Advisor *temp = advisorHead;
    while (temp != NULL)
    {
        fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
                temp->advisorID,
                temp->name,
                temp->department,
                temp->email,
                temp->phone,
                temp->password);

        temp = temp->nxt;
    }

    fclose(fp);

    printf("\n[SUCCESS] Advisor records saved successfully!\n");
}

void assignStudentToAdvisor()
{
    printf("\n--- Assign Student to Advisor ---\n");

    if (studentHead == NULL)
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

    Student *studentTemp = studentHead;
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

    Student *studentTemp = studentHead;
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
        Student *studentTemp = studentHead;
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
    Student *studentTemp = studentHead;
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

void viewUnassignedStudents()
{
    printf("\n--- View All Unassigned Students ---\n");

    if (studentHead == NULL)
    {
        printf("\n[ERROR] No students available!\n");
        return;
    }

    Student *studentTemp = studentHead;
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

void advisorPanel(char advisorID[])
{
    int choice;

    while (1)
    {
        printf("\n========== ADVISOR DASHBOARD ==========\n");
        printf("1. View My Profile\n");
        printf("2. View My Assigned Students\n");
        printf("3. Search Assigned Student\n");
        printf("4. View Student Registration\n");
        printf("0. Logout\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\n--- My Profile ---\n");

            Advisor *advisorTemp = advisorHead;
            while (advisorTemp != NULL)
            {
                if (!strcmp(advisorID, advisorTemp->advisorID))
                {
                    displayAdvisorDetails(advisorTemp);
                    break;
                }
                advisorTemp = advisorTemp->nxt;
            }

            if (advisorTemp == NULL)
            {
                printf("\n[ERROR] Advisor record not found!\n");
            }
            break;

        case 2:
            viewMyAssignedStudents(advisorID);
            break;

        case 3:
        {
            char searchID[21];
            printf("\n--- Search Assigned Student ---\n");
            printf("Enter Student ID: ");
            scanf("%20s", searchID);

            Student *studentTemp = studentHead;
            int found = 0;
            while (studentTemp != NULL)
            {
                if (!strcmp(studentTemp->studentID, searchID) && !strcmp(studentTemp->advisorID, advisorID))
                {
                    found = 1;
                    break;
                }

                studentTemp = studentTemp->nxt;
            }

            if (found)
            {
                printf("\n[SUCCESS] Student found!\n");
                displayStudentDetails(studentTemp);
            }
            else
            {
                printf("\n[ERROR] Student not found in your assigned student list!\n");
            }
            break;
        }

        case 4:
            printf("\n[INFO] Student registration feature is under development.\n");
            break;

        case 0:
            printf("\nLogging out...\n");
            return;

        default:
            printf("\nInvalid choice! Try again.\n");
        }
    }
}

void viewMyAssignedStudents(char advisorID[])
{
    Advisor *advisorTemp = advisorHead;
    while (advisorTemp != NULL)
    {
        if (!strcmp(advisorTemp->advisorID, advisorID))
            break;

        advisorTemp = advisorTemp->nxt;
    }
    if (advisorTemp == NULL)
    {
        printf("\n[ERROR] Advisor not found!\n");
        return;
    }

    printf("\n--- My Assigned Students ---\n");
    displayAdvisorDetails(advisorTemp);

    Student *studentTemp = studentHead;
    int count = 0;
    while (studentTemp != NULL)
    {
        if (!strcmp(studentTemp->advisorID, advisorID))
        {
            displayStudentDetails(studentTemp);
            count++;
        }
        studentTemp = studentTemp->nxt;
    }
    if (count == 0)
        printf("\nNo students are currently assigned to you.\n");
    else
        printf("\nTotal Assigned Students: %d\n", count);
}

void studentPanel(char studentID[])
{
    Student *temp = studentHead;
    while (temp != NULL)
    {
        if (!strcmp(studentID, temp->studentID))
        {
            break;
        }

        temp = temp->nxt;
    }
    if (temp == NULL)
    {
        printf("\n[ERROR] Student record not found!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n========== STUDENT PANEL ==========\n");

        printf("\n1. View My Profile\n");
        printf("2. View My Advisor\n");
        printf("3. View Batch & Section Information\n");

        printf("\n---------- SECTION ALLOCATION ----------\n");
        printf("4. View Available Sections\n");
        printf("5. Submit Section Preferences\n");
        printf("6. View Section Request Status\n");
        printf("7. Respond to Alternative Section\n");
        printf("8. Request Section Change\n");
        printf("9. View Section Change Status\n");

        printf("\n---------- COURSE REGISTRATION ----------\n");
        printf("10. View Available Courses\n");
        printf("11. Search Course\n");
        printf("12. View Course Details\n");
        printf("13. Register Course\n");
        printf("14. Drop Course\n");
        printf("15. View Registered Courses\n");
        printf("16. View Total Credits\n");
        printf("17. View Waiting List Status\n");
        printf("18. View Registration History\n");
        printf("19. View Course Prerequisites\n");

        printf("\n0. Logout\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\n--- My Profile ---\n");
            displayStudentDetails(temp);
            break;

        case 2:
            if (temp->advisorID[0] == '\0')
            {
                printf("\n[INFO] You do not have an assigned advisor yet.\n");
            }
            else
            {
                Advisor *advisorTemp = advisorHead;
                while (advisorTemp != NULL)
                {
                    if (!strcmp(advisorTemp->advisorID, temp->advisorID))
                    {
                        printf("\n--- My Advisor ---\n");
                        displayAdvisorDetails(advisorTemp);
                        break;
                    }
                    advisorTemp = advisorTemp->nxt;
                }
                if (advisorTemp == NULL)
                {
                    printf("\n[ERROR] Your assigned advisor record was not found!\n");
                }
            }
            break;

        case 3:
            printf("\n--- My Batch & Section Information ---\n");
            printf("Department: %s\n", temp->department);
            printf("Batch: %d\n", temp->batch);

            if (temp->section[0] == '\0')
            {
                printf("Section: Not allocated yet\n");
            }
            else
            {
                printf("Section: %s\n", temp->section);
            }
            break;

        case 0:
            printf("\nLogging out from Student Panel...\n");
            return;

        default:
            if (choice >= 4 && choice <= 19)
            {
                printf("\n[INFO] This feature is under development.\n");
            }
            else
            {
                printf("\n[ERROR] Invalid choice! Please try again.\n");
            }
        }
    }
}


// Release Heap Memory of Admin, Student & Advisor (Singly) Linked Lists
void freeAdmins()
{
    Admin *temp;
    while (adminHead != NULL)
    {
        temp = adminHead;
        adminHead = adminHead->nxt;
        free(temp);
    }
    printf("\n[SUCCESS] Admin memory released successfully!\n");
}

void freeStudents()
{
    Student *temp;
    while (studentHead != NULL)
    {
        temp = studentHead;
        studentHead = studentHead->nxt;
        free(temp);
    }
    printf("\n[SUCCESS] Student memory released successfully!\n");
}

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
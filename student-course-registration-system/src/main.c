#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/reports.h"
#include "../include/dept_batch_section.h"
#include "../include/course.h"
#include "../include/registration.h"
#include "../include/section.h"
#include "../include/utils.h"

#define ADMIN 1
#define ADVISOR 2
#define STUDENT 3


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
    int batch;
    char section[21];
    int currentSemester;
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
    char email[101];
    char phone[21];
    char password[101];
    struct Advisor *nxt;
} Advisor;


// Function Prototypes
void loadAdmins();
void loadStudents();
void loadAdvisors();

void login(int role);

void adminPanel();
void addStudent();
void updateStudent();
void deleteStudent();
void saveStudents();
void addAdvisor();
void viewAllAdvisors();
void searchAdvisor();
void updateAdvisor();
void deleteAdvisor();
void saveAdvisors();
void assignStudentToAdvisor();
void viewAdvisorStudents();
void viewAdvisorWorkload();
void viewUnassignedStudents();

void advisorPanel(char advisorID[]);
void viewMyAssignedStudents(char advisorID[]);

void studentPanel(char studentID[]);
Student *findStudentByID(const char *studentID);
Advisor *findAdvisorByID(const char *advisorID);
int studentExists(const char *studentID);
int advisorExists(const char *advisorID);

void viewAllStudents();
void searchStudent();

void displayStudentDetails(Student *student);
void displayAdvisorDetails(Advisor *advisor);

void freeAdmins();
void freeStudents();
void freeAdvisors();



Admin *adminHead = NULL;
Student *studentHead = NULL;
Advisor *advisorHead = NULL;


int main()
{
    ensureDataDir();
    ensureReportsDir();
    loadAdmins();
    loadStudents();
    loadAdvisors();
    loadBatches();
    loadSections();
    loadCourses();
    loadPrerequisites();
    loadRegistrations();
    loadWaitlists();
    loadHistory();
    loadRegistrationRequests();
    loadSectionRequests();
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

        choice = readInt("Enter your choice: ");

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
            freeBatches();
            freeSections();
            freeCourses();
            freePrerequisites();
            freeRegistrations();
            freeWaitlists();
            freeHistory();
            freeRegistrationRequests();
            freeSectionRequests();
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
    FILE *fp = fopen("data/admins.txt", "r");

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
    FILE *fp = fopen("data/students.txt", "r");

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
        int result = fscanf(fp, "%20[^|]|%100[^|]|%d|%20[^|]|%d|%100[^|]|%20[^|]|%50[^|]|%100[^\n]\n",
                            newStudent->studentID,
                            newStudent->name,
                            &newStudent->batch,
                            newStudent->section,
                            &newStudent->currentSemester,
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
    FILE *fp = fopen("data/advisors.txt", "r");

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

        int result = fscanf(fp, "%50[^|]|%100[^|]|%100[^|]|%20[^|]|%100[^\n]\n",
                            newAdvisor->advisorID,
                            newAdvisor->name,
                            newAdvisor->email,
                            newAdvisor->phone,
                            newAdvisor->password);

        if (result != 5)
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
    readToken("Enter ID: ", inputID, 100);
    readToken("Enter Password: ", inputPassword, 100);

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


// ADMIN FUNCTIONS
void adminPanel()
{
    int choice;

    while (1)
    {
        printf("\n========== ADMIN PANEL ==========\n");
        printf("\n---------- STUDENT MANAGEMENT ----------\n");
        printf("1. Add Student\n2. View All Students\n3. Search Student\n4. Update Student\n5. Delete Student\n");
        printf("\n---------- ADVISOR MANAGEMENT ----------\n");
        printf("6. Add Advisor\n7. View All Advisors\n8. Search Advisor\n9. Update Advisor\n10. Delete Advisor\n");
        printf("11. Assign Student to Advisor\n12. View Advisor's Assigned Students\n13. View Advisor Workload\n14. View Unassigned Students\n");
        printf("\n---------- BATCH MANAGEMENT ----------\n15. Manage Batches\n");
        printf("\n---------- SECTION MANAGEMENT ----------\n16. Manage Sections\n17. Manage Section Requests\n");
        printf("\n---------- COURSE MANAGEMENT ----------\n18. Manage Courses\n");
        printf("\n---------- REGISTRATION MANAGEMENT ----------\n19. Manage Registration\n");
        printf("\n---------- REPORTS ----------\n20. View Reports\n");
        printf("\n0. Logout\n");
        choice = readInt("\nEnter your choice: ");

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
            manageBatches();
            break;

        case 16:
            manageSections();
            break;

        case 17:
            manageSectionRequests();
            break;

        case 18:
            manageCourses();
            break;

        case 19:
            manageRegistration();
            break;

        case 20:
            viewReports();
            break;

        case 0:
            printf("\nLogging out from Admin Panel...\n");
            return;

        default:
            printError("Invalid choice! Please try again.");
        }
    }
}

void addStudent()
{
    char newStudentID[21];
    Student *temp;
    Student *newStudent;
    printf("\n--- Add New Student ---\n\n");
    readToken("Enter Student ID: ", newStudentID, 20);
    if (studentExists(newStudentID)) { printError("This Student ID already exists."); return; }
    newStudent = malloc(sizeof(Student));
    if (newStudent == NULL) { printError("Memory allocation failed!"); return; }
    strcpy(newStudent->studentID, newStudentID);
    readLine("Enter Student Name: ", newStudent->name, sizeof(newStudent->name));
    newStudent->batch = readInt("Enter Batch: ");
    if (findBatch(newStudent->batch) == NULL) { printError("Batch not found."); free(newStudent); return; }
    readToken("Enter Section (or - for none): ", newStudent->section, 20);
    if (strcmp(newStudent->section, "-") == 0) newStudent->section[0] = '\0';
    if (newStudent->section[0] != '\0')
    {
        Section *sec = findSection(newStudent->batch, newStudent->section);
        if (sec == NULL) { printError("Section not found for batch."); free(newStudent); return; }
        if (countStudentsInSection(newStudent->batch, newStudent->section) >= sec->capacity)
        { printError("Section is full."); free(newStudent); return; }
    }
    newStudent->currentSemester = readIntRange("Enter Current Semester (1-12): ", 1, 12);
    readToken("Enter Email: ", newStudent->email, 100);
    readToken("Enter Phone: ", newStudent->phone, 20);
    newStudent->advisorID[0] = '\0';
    readToken("Enter Password: ", newStudent->password, 100);
    newStudent->nxt = NULL;
    if (studentHead == NULL) studentHead = newStudent;
    else { temp = studentHead; while (temp->nxt != NULL) temp = temp->nxt; temp->nxt = newStudent; }
    printSuccess("Student added successfully!");
    saveStudents();
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
    readToken("Enter Student ID: ", searchID, 20);

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
        printf("2. Update Batch\n");
        printf("3. Update Section\n");
        printf("4. Update Email\n");
        printf("5. Update Phone\n");
        printf("6. Update Current Semester\n");
        // printf("7. Update Advisor ID\n");
        printf("0. Finish Updating\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice)
        {
        case 1:
            readLine("Enter New Name: ", temp->name, sizeof(temp->name));
            printf("\n[SUCCESS] Name updated successfully!\n");
            break;

        case 2:
        {
            int batch = readInt("Enter New Batch: ");
            if (findBatch(batch) == NULL) { printError("Batch not found."); break; }
            temp->batch = batch;
            temp->section[0] = '\0';
            printSuccess("Batch updated. Section cleared; reassign as needed.");
            break;
        }
        case 3:
        {
            char secName[21];
            readToken("Enter New Section (- to clear): ", secName, 20);
            if (strcmp(secName, "-") == 0) { temp->section[0] = '\0'; printSuccess("Section cleared."); break; }
            {
                Section *sec = findSection(temp->batch, secName);
                if (sec == NULL) { printError("Section not found for batch."); break; }
                if (countStudentsInSection(temp->batch, secName) >= sec->capacity) { printError("Section is full."); break; }
                strcpy(temp->section, secName);
            }
            printSuccess("Section updated successfully!");
            break;
        }

        case 4:
            readToken("Enter New Email: ", temp->email, 100);
            printf("\n[SUCCESS] Email updated successfully!\n");
            break;

        case 5:
            readToken("Enter New Phone: ", temp->phone, 20);
            printf("\n[SUCCESS] Phone updated successfully!\n");
            break;

        case 6:
            temp->currentSemester = readIntRange("Enter New Current Semester (1-12): ", 1, 12);
            printSuccess("Current semester updated successfully!");
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
    readToken("\nEnter Student ID: ", searchID, 20);

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

    if (confirmYesNo("\nAre you sure you want to delete this student? (Y/N): "))
    {
        Registration *r = registrationHead, *rp = NULL, *rn;
        while (r != NULL)
        {
            rn = r->nxt;
            if (strcmp(r->studentID, searchID) == 0)
            {
                char code[21], section[21];
                int wasRegistered = (strcmp(r->status, "REGISTERED") == 0);
                strcpy(code, r->courseCode);
                strcpy(section, r->section);
                if (strcmp(r->status, "WAITING") == 0)
                {
                    WaitQueue *q = waitQueueHead;
                    while (q != NULL && !(strcmp(q->courseCode, r->courseCode) == 0 &&
                                          strcmp(q->section, r->section) == 0)) q = q->nxt;
                    if (q != NULL)
                    {
                        WaitNode *wn = q->front, *wp = NULL;
                        while (wn != NULL)
                        {
                            if (strcmp(wn->studentID, searchID) == 0)
                            {
                                if (wp != NULL) wp->nxt = wn->nxt; else q->front = wn->nxt;
                                if (q->rear == wn) q->rear = wp;
                                free(wn);
                                break;
                            }
                            wp = wn;
                            wn = wn->nxt;
                        }
                    }
                }
                if (rp != NULL) rp->nxt = rn; else registrationHead = rn;
                pushHistory(searchID, "DROP", code, section);
                free(r);
                if (wasRegistered)
                {
                    char nextID[21];
                    if (dequeueWait(code, section, nextID))
                    {
                        Registration *wr = registrationHead;
                        while (wr != NULL)
                        {
                            if (strcmp(wr->studentID, nextID) == 0 && strcmp(wr->courseCode, code) == 0 &&
                                strcmp(wr->section, section) == 0 && strcmp(wr->status, "WAITING") == 0)
                            {
                                strcpy(wr->status, "REGISTERED");
                                pushHistory(nextID, "PROMOTED", code, section);
                                break;
                            }
                            wr = wr->nxt;
                        }
                    }
                }
            }
            else rp = r;
            r = rn;
        }
        saveHistory();
        {
            RegistrationRequest *rr = registrationRequestHead, *rpReq = NULL, *rnReq;
            while (rr != NULL)
            {
                rnReq = rr->nxt;
                if (strcmp(rr->studentID, searchID) == 0)
                {
                    if (rpReq != NULL) rpReq->nxt = rnReq; else registrationRequestHead = rnReq;
                    free(rr);
                }
                else rpReq = rr;
                rr = rnReq;
            }
        }
        {
            SectionRequest *sr = sectionRequestHead, *sp = NULL, *sn;
            while (sr != NULL)
            {
                sn = sr->nxt;
                if (strcmp(sr->studentID, searchID) == 0) { if (sp) sp->nxt = sn; else sectionRequestHead = sn; free(sr); }
                else sp = sr;
                sr = sn;
            }
        }
        saveRegistrations(); saveWaitlists(); saveRegistrationRequests(); saveSectionRequests();
        if (prev == NULL) studentHead = temp->nxt; else prev->nxt = temp->nxt;
        free(temp);
        printSuccess("Student deleted successfully!");
        saveStudents();
    }
    else
    {
        printf("\n[INFO] Student deletion cancelled.\n");
    }
}

void saveStudents()
{
    FILE *fp = fopen("data/students.txt", "w");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open student file!\n");
        return;
    }

    Student *temp = studentHead;
    while (temp != NULL)
    {
        fprintf(fp, "%s|%s|%d|%s|%d|%s|%s|%s|%s\n",
                temp->studentID,
                temp->name,
                temp->batch,
                temp->section,
                temp->currentSemester,
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
    readToken("Enter Advisor ID: ", newAdvisorID, 50);

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
    readLine("Enter Advisor Name: ", newAdvisor->name, sizeof(newAdvisor->name));
    printf("Enter Email: ");
    readToken("Enter Email: ", newAdvisor->email, 100);
    printf("Enter Phone: ");
    readToken("Enter Phone: ", newAdvisor->phone, 20);
    printf("Enter Password: ");
    readToken("Enter Password: ", newAdvisor->password, 100);

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
    readToken("Enter Advisor ID: ", searchID, 50);

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
    readToken("Enter Advisor ID: ", searchID, 50);

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
        printf("2. Update Email\n");
        printf("3. Update Phone\n");
        printf("4. Update Password\n");
        printf("0. Finish Updating\n");

        choice = readInt("\nEnter your choice: ");

        switch (choice)
        {
        case 1:
            readLine("Enter New Name: ", temp->name, sizeof(temp->name));
            printf("\n[SUCCESS] Name updated successfully!\n");
            break;

        case 2:
            readToken("Enter New Email: ", temp->email, 100);
            printf("\n[SUCCESS] Email updated successfully!\n");
            break;

        case 3:
            readToken("Enter New Phone: ", temp->phone, 20);
            printf("\n[SUCCESS] Phone updated successfully!\n");
            break;

        case 4:
            readToken("Enter New Password: ", temp->password, 100);
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
    readToken("Enter Advisor ID: ", searchID, 50);

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

    if (confirmYesNo("\nAre you sure you want to delete this advisor? (Y/N): "))
    {
        Student *st = studentHead;
        while (st != NULL)
        {
            if (strcmp(st->advisorID, searchID) == 0) st->advisorID[0] = '\0';
            st = st->nxt;
        }
        saveStudents();
        if (prev == NULL) advisorHead = temp->nxt; else prev->nxt = temp->nxt;
        free(temp);
        printSuccess("Advisor deleted successfully!");
        saveAdvisors();
    }
    else
    {
        printf("\n[INFO] Advisor deletion cancelled.\n");
    }
}

void saveAdvisors()
{
    FILE *fp = fopen("data/advisors.txt", "w");

    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to open advisor file!\n");
        return;
    }

    Advisor *temp = advisorHead;
    while (temp != NULL)
    {
        fprintf(fp, "%s|%s|%s|%s|%s\n",
                temp->advisorID,
                temp->name,
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
    readToken("Enter Student ID: ", searchStudentID, 20);

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
    readToken("Enter Advisor ID: ", searchAdvisorID, 50);

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

    if (studentTemp->advisorID[0] != '\0')
    {
        printf("\n[INFO] This student already has an assigned advisor.\n");
        printf("Current Advisor ID: %s\n", studentTemp->advisorID);

        if (!confirmYesNo("\nDo you want to change the assigned advisor? (Y/N): "))
        {
            printInfo("Advisor reassignment cancelled.");
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
    readToken("Enter Advisor ID: ", searchID, 50);

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


// ADVISOR FUNCTIONS
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
        printf("5. Manage Registration Requests\n");
        printf("0. Logout\n");

        choice = readInt("\nEnter your choice: ");

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
            readToken("Enter ID: ", searchID, 20);

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
            viewStudentRegistrationForAdvisor(advisorID);
            break;

        case 5:
            manageRegistrationRequestsForAdvisor(advisorID);
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



// STUDENT FUNCTION
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
        printf("13. Submit Registration Request\n");
        printf("14. Drop Course\n");
        printf("15. View Registered Courses\n");
        printf("16. View Total Credits\n");
        printf("17. View Waiting List Status\n");
        printf("18. View Registration History\n");
        printf("19. View Course Prerequisites\n");
        printf("20. View Registration Request Status\n");

        printf("\n0. Logout\n");

        choice = readInt("\nEnter your choice: ");

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
            printf("Batch: %d\n", temp->batch);
            printf("Current Semester: %d\n", temp->currentSemester);

            if (temp->section[0] == '\0')
            {
                printf("Section: Not allocated yet\n");
            }
            else
            {
                printf("Section: %s\n", temp->section);
            }
            break;

        case 4:
            viewAvailableSectionsForStudent(studentID);
            break;

        case 5:
            submitSectionPreferences(studentID);
            temp = findStudentByID(studentID);
            break;

        case 6:
            viewSectionRequestStatus(studentID);
            break;

        case 7:
            respondToAlternativeSection(studentID);
            temp = findStudentByID(studentID);
            break;

        case 8:
            requestSectionChange(studentID);
            temp = findStudentByID(studentID);
            break;

        case 9:
            viewSectionChangeStatus(studentID);
            break;

        case 10:
            viewAvailableCourses();
            break;

        case 11:
            searchCourse();
            break;

        case 12:
            viewCourseDetails();
            break;

        case 13:
            submitRegistrationRequest(studentID);
            break;

        case 14:
            dropCourseForStudent(studentID);
            break;

        case 15:
            viewRegisteredCoursesForStudent(studentID);
            break;

        case 16:
            viewTotalCreditsForStudent(studentID);
            break;

        case 17:
            viewWaitingListStatusForStudent(studentID);
            break;

        case 18:
            viewRegistrationHistoryForStudent(studentID);
            break;

        case 19:
            viewCoursePrerequisites();
            break;

        case 20:
            viewRegistrationRequestStatus(studentID);
            break;

        case 0:
            printf("\nLogging out from Student Panel...\n");
            return;

        default:
            printf("\n[ERROR] Invalid choice! Please try again.\n");
        }
    }
}


Student *findStudentByID(const char *studentID)
{
    Student *temp = studentHead;
    while (temp != NULL)
    {
        if (strcmp(temp->studentID, studentID) == 0)
        {
            return temp;
        }
        temp = temp->nxt;
    }
    return NULL;
}

Advisor *findAdvisorByID(const char *advisorID)
{
    Advisor *temp = advisorHead;
    while (temp != NULL)
    {
        if (strcmp(temp->advisorID, advisorID) == 0)
        {
            return temp;
        }
        temp = temp->nxt;
    }
    return NULL;
}

int studentExists(const char *studentID)
{
    return findStudentByID(studentID) != NULL;
}

int advisorExists(const char *advisorID)
{
    return findAdvisorByID(advisorID) != NULL;
}

// COMMON FUNCTIONS (Admin & Advisor)
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
    readToken("Enter ID: ", searchID, 20);

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


// COMMON FUNCTIONS (Admin, Advisor & Student)
void displayStudentDetails(Student *student)
{
    printf("Student ID: %s\n", student->studentID);
    printf("Name: %s\n", student->name);
    printf("Batch: %d\n", student->batch);
    printf("Section: %s\n", student->section);
    printf("Current Semester: %d\n", student->currentSemester);
    printf("Email: %s\n", student->email);
    printf("Phone: %s\n", student->phone);
    printf("Advisor ID: %s\n", student->advisorID);

    printf("=====================================\n");
}

void displayAdvisorDetails(Advisor *advisor)
{
    printf("Advisor ID: %s\n", advisor->advisorID);
    printf("Name: %s\n", advisor->name);
    printf("Email: %s\n", advisor->email);
    printf("Phone: %s\n", advisor->phone);

    printf("=====================================\n");
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

#include "../include/course.h"
#include "../include/types.h"
#include "../include/registration.h"
#include "../include/utils.h"

Course *courseHead = NULL;
Prerequisite *prereqHead = NULL;

Course *findCourse(const char *code)
{
    Course *c = courseHead;
    while (c) { if (strcmp(c->code, code) == 0) return c; c = c->nxt; }
    return NULL;
}

int prereqExists(const char *courseCode, const char *prereqCode)
{
    Prerequisite *p = prereqHead;
    while (p)
    {
        if (strcmp(p->courseCode, courseCode) == 0 && strcmp(p->prereqCode, prereqCode) == 0) return 1;
        p = p->nxt;
    }
    return 0;
}

int hasPrerequisitesSatisfied(const char *studentID, const char *courseCode)
{
    Prerequisite *p = prereqHead;
    while (p)
    {
        if (strcmp(p->courseCode, courseCode) == 0)
        {
            Registration *r = registrationHead;
            int ok = 0;
            while (r)
            {
                if (strcmp(r->studentID, studentID) == 0 && strcmp(r->courseCode, p->prereqCode) == 0 &&
                    (strcmp(r->status, "REGISTERED") == 0 || strcmp(r->status, "COMPLETED") == 0))
                { ok = 1; break; }
                r = r->nxt;
            }
            if (!ok) return 0;
        }
        p = p->nxt;
    }
    return 1;
}

void loadCourses()
{
    FILE *fp = fopen("data/courses.txt", "r");
    Course *c;
    if (!fp) return;
    while (1)
    {
        c = malloc(sizeof(Course));
        if (!c) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%100[^|]|%20[^|]|%d|%d\n", c->code, c->name, c->department, &c->credits, &c->capacity) != 5)
        { free(c); break; }
        c->nxt = NULL;
        if (!courseHead) courseHead = c; else { Course *t=courseHead; while(t->nxt) t=t->nxt; t->nxt=c; }
    }
    fclose(fp);
}

void saveCourses()
{
    FILE *fp = fopen("data/courses.txt", "w");
    Course *t = courseHead;
    if (!fp) { printf("\n[ERROR] Unable to save courses.\n"); return; }
    while (t) { fprintf(fp, "%s|%s|%s|%d|%d\n", t->code, t->name, t->department, t->credits, t->capacity); t = t->nxt; }
    fclose(fp);
}

void loadPrerequisites()
{
    FILE *fp = fopen("data/prerequisites.txt", "r");
    Prerequisite *p;
    if (!fp) return;
    while (1)
    {
        p = malloc(sizeof(Prerequisite));
        if (!p) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%20[^\n]\n", p->courseCode, p->prereqCode) != 2) { free(p); break; }
        p->nxt = NULL;
        if (!prereqHead) prereqHead = p; else { Prerequisite *t=prereqHead; while(t->nxt) t=t->nxt; t->nxt=p; }
    }
    fclose(fp);
}

void savePrerequisites()
{
    FILE *fp = fopen("data/prerequisites.txt", "w");
    Prerequisite *t = prereqHead;
    if (!fp) { printf("\n[ERROR] Unable to save prerequisites.\n"); return; }
    while (t) { fprintf(fp, "%s|%s\n", t->courseCode, t->prereqCode); t = t->nxt; }
    fclose(fp);
}

void freeCourses() { Course *t; while (courseHead) { t=courseHead; courseHead=courseHead->nxt; free(t); } }
void freePrerequisites() { Prerequisite *t; while (prereqHead) { t=prereqHead; prereqHead=prereqHead->nxt; free(t); } }

void addCourse()
{
    Course *c;
    char code[21];
    readToken("Enter Course Code: ", code, 20);
    if (findCourse(code)) { printError("Course already exists."); return; }
    c = malloc(sizeof(Course));
    if (c == NULL) { printError("Memory allocation failed."); return; }
    strcpy(c->code, code);
    readLine("Enter Course Name: ", c->name, sizeof(c->name));
    readToken("Enter Department: ", c->department, 20);
    c->credits = readIntRange("Enter Credits: ", 1, 30);
    c->capacity = readIntRange("Enter Capacity: ", 1, 500);
    c->nxt = NULL;
    if (courseHead == NULL) courseHead = c; else { Course *t=courseHead; while(t->nxt!=NULL) t=t->nxt; t->nxt=c; }
    saveCourses();
    printSuccess("Course added.");
}

void listCourses()
{
    Course *t = courseHead;
    if (!t) { printf("\nNo courses found.\n"); return; }
    printf("\n%-10s %-25s %-8s %-7s %-8s\n", "Code", "Name", "Dept", "Credit", "Cap");
    while (t) { printf("%-10s %-25.25s %-8s %-7d %-8d\n", t->code, t->name, t->department, t->credits, t->capacity); t = t->nxt; }
}

void deleteCourse()
{
    char code[21];
    Course *t = courseHead, *p = NULL;
    Prerequisite *pr = prereqHead, *pp = NULL, *pn;
    readToken("Enter Course Code: ", code, 20);
    while (t != NULL && strcmp(t->code, code) != 0) { p = t; t = t->nxt; }
    if (t == NULL) { printError("Course not found."); return; }
    if (countCourseRegistrations(code) > 0 || countCourseWaitlist(code) > 0)
    {
        printError("Cannot delete course with active registrations or waitlist.");
        return;
    }
    if (!confirmYesNo("Are you sure you want to delete this course? (Y/N): ")) return;
    while (pr != NULL)
    {
        pn = pr->nxt;
        if (strcmp(pr->courseCode, code) == 0 || strcmp(pr->prereqCode, code) == 0)
        {
            if (pp != NULL) pp->nxt = pn; else prereqHead = pn;
            free(pr);
        }
        else pp = pr;
        pr = pn;
    }
    savePrerequisites();
    if (p != NULL) p->nxt = t->nxt; else courseHead = t->nxt;
    free(t);
    saveCourses();
    printSuccess("Course deleted.");
}

void addPrereq()
{
    char course[21], prereq[21];
    Prerequisite *p, *t;
    readToken("Enter Course Code: ", course, 20);
    readToken("Enter Prerequisite Course Code: ", prereq, 20);
    if (findCourse(course) == NULL || findCourse(prereq) == NULL) { printError("Both courses must exist."); return; }
    if (strcmp(course, prereq) == 0) { printError("Course cannot be its own prerequisite."); return; }
    if (prereqExists(course, prereq)) { printError("Prerequisite already exists."); return; }
    p = malloc(sizeof(Prerequisite));
    if (p == NULL) { printError("Memory allocation failed."); return; }
    strcpy(p->courseCode, course);
    strcpy(p->prereqCode, prereq);
    p->nxt = NULL;
    if (prereqHead == NULL) prereqHead = p; else { t = prereqHead; while (t->nxt != NULL) t = t->nxt; t->nxt = p; }
    savePrerequisites();
    printSuccess("Prerequisite added.");
}

void listPrereqs()
{
    Prerequisite *p = prereqHead;
    if (p == NULL) { printInfo("No prerequisites found."); return; }
    printf("\n%-12s %-12s\n", "Course", "Requires");
    while (p != NULL) { printf("%-12s %-12s\n", p->courseCode, p->prereqCode); p = p->nxt; }
}

void manageCourses()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Courses ---\n1. Add Course\n2. View Courses\n3. Delete Course\n4. Add Prerequisite\n5. View Prerequisites\n0. Back\n");
        c = readInt("Choice: ");
        if (c == 1) addCourse();
        else if (c == 2) listCourses();
        else if (c == 3) deleteCourse();
        else if (c == 4) addPrereq();
        else if (c == 5) listPrereqs();
        else if (c == 0) return;
        else printError("Invalid choice.");
    }
}

void viewAvailableCourses()
{
    listCourses();
}

void searchCourse()
{
    char code[21];
    Course *c;
    readToken("Enter Course Code: ", code, 20);
    c = findCourse(code);
    if (c == NULL) { printError("Course not found."); return; }
    printf("\nCode: %s\nName: %s\nDept: %s\nCredits: %d\nCapacity: %d\n", c->code, c->name, c->department, c->credits, c->capacity);
}

void viewCourseDetails()
{
    searchCourse();
}

void viewCoursePrerequisites()
{
    char code[21];
    Prerequisite *p;
    int found = 0;
    readToken("Enter Course Code: ", code, 20);
    if (findCourse(code) == NULL) { printError("Course not found."); return; }
    printf("\nPrerequisites for %s:\n", code);
    p = prereqHead;
    while (p != NULL)
    {
        if (strcmp(p->courseCode, code) == 0) { printf("- %s\n", p->prereqCode); found = 1; }
        p = p->nxt;
    }
    if (!found) printInfo("None.");
}

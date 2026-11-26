#include "../include/course.h"
#include "../include/types.h"
#include "../include/registration.h"

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
    Course *c; char code[21];
    printf("Enter Course Code: "); scanf("%20s", code);
    if (findCourse(code)) { printf("\n[ERROR] Course already exists.\n"); return; }
    c = malloc(sizeof(Course)); if (!c) return;
    strcpy(c->code, code);
    printf("Enter Course Name: "); scanf(" %100[^\n]", c->name);
    printf("Enter Department: "); scanf("%20s", c->department);
    printf("Enter Credits: "); scanf("%d", &c->credits);
    printf("Enter Capacity: "); scanf("%d", &c->capacity);
    c->nxt = NULL;
    if (!courseHead) courseHead = c; else { Course *t=courseHead; while(t->nxt) t=t->nxt; t->nxt=c; }
    saveCourses(); printf("\n[SUCCESS] Course added.\n");
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
    char code[21]; Course *t=courseHead,*p=NULL;
    printf("Enter Course Code: "); scanf("%20s", code);
    while (t && strcmp(t->code, code)!=0) { p=t; t=t->nxt; }
    if (!t) { printf("\n[ERROR] Course not found.\n"); return; }
    if (p) p->nxt=t->nxt; else courseHead=t->nxt; free(t); saveCourses();
    printf("\n[SUCCESS] Course deleted.\n");
}

void addPrereq()
{
    char course[21], prereq[21]; Prerequisite *p;
    printf("Enter Course Code: "); scanf("%20s", course);
    printf("Enter Prerequisite Course Code: "); scanf("%20s", prereq);
    if (!findCourse(course) || !findCourse(prereq)) { printf("\n[ERROR] Both courses must exist.\n"); return; }
    if (strcmp(course, prereq)==0) { printf("\n[ERROR] Course cannot be its own prerequisite.\n"); return; }
    if (prereqExists(course, prereq)) { printf("\n[ERROR] Prerequisite already exists.\n"); return; }
    p = malloc(sizeof(Prerequisite)); if (!p) return;
    strcpy(p->courseCode, course); strcpy(p->prereqCode, prereq); p->nxt=NULL;
    if (!prereqHead) prereqHead=p; else { Prerequisite *t=prereqHead; while(t->nxt) t=t->nxt; t->nxt=p; }
    savePrerequisites(); printf("\n[SUCCESS] Prerequisite added.\n");
}

void listPrereqs()
{
    Prerequisite *p = prereqHead;
    if (!p) { printf("\nNo prerequisites found.\n"); return; }
    printf("\n%-12s %-12s\n", "Course", "Requires");
    while (p) { printf("%-12s %-12s\n", p->courseCode, p->prereqCode); p = p->nxt; }
}

void manageCourses()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Courses ---\n1. Add Course\n2. View Courses\n3. Delete Course\n4. Add Prerequisite\n5. View Prerequisites\n0. Back\nChoice: ");
        if (scanf("%d", &c) != 1) { int ch; while ((ch=getchar())!='\n'&&ch!=EOF){} continue; }
        if (c==1) addCourse();
        else if (c==2) listCourses();
        else if (c==3) deleteCourse();
        else if (c==4) addPrereq();
        else if (c==5) listPrereqs();
        else if (c==0) return;
    }
}

void viewAvailableCourses()
{
    listCourses();
}

void searchCourse()
{
    char code[21]; Course *c;
    printf("Enter Course Code: "); scanf("%20s", code);
    c = findCourse(code);
    if (!c) { printf("\n[ERROR] Course not found.\n"); return; }
    printf("\nCode: %s\nName: %s\nDept: %s\nCredits: %d\nCapacity: %d\n", c->code, c->name, c->department, c->credits, c->capacity);
}

void viewCourseDetails()
{
    searchCourse();
}

void viewCoursePrerequisites()
{
    char code[21]; Prerequisite *p; int found=0;
    printf("Enter Course Code: "); scanf("%20s", code);
    if (!findCourse(code)) { printf("\n[ERROR] Course not found.\n"); return; }
    printf("\nPrerequisites for %s:\n", code);
    p = prereqHead;
    while (p)
    {
        if (strcmp(p->courseCode, code)==0) { printf("- %s\n", p->prereqCode); found=1; }
        p = p->nxt;
    }
    if (!found) printf("None\n");
}

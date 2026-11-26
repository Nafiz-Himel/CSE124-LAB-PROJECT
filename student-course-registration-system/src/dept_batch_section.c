#include "../include/dept_batch_section.h"
#include "../include/types.h"
#include "../include/course.h"
#include "../include/utils.h"

Department *departmentHead = NULL;
Batch *batchHead = NULL;
Section *sectionHead = NULL;

Department *findDepartment(const char *code)
{
    Department *t = departmentHead;
    while (t) { if (strcmp(t->code, code) == 0) return t; t = t->nxt; }
    return NULL;
}

Batch *findBatch(const char *dept, int year)
{
    Batch *t = batchHead;
    while (t) { if (strcmp(t->departmentCode, dept) == 0 && t->batchYear == year) return t; t = t->nxt; }
    return NULL;
}

Section *findSection(const char *dept, int year, const char *sectionName)
{
    Section *t = sectionHead;
    while (t)
    {
        if (strcmp(t->departmentCode, dept) == 0 && t->batchYear == year && strcmp(t->sectionName, sectionName) == 0)
            return t;
        t = t->nxt;
    }
    return NULL;
}

int countStudentsInSection(const char *dept, int year, const char *sectionName)
{
    Student *s = studentHead;
    int count = 0;
    while (s)
    {
        if (strcmp(s->department, dept) == 0 && s->batch == year && strcmp(s->section, sectionName) == 0)
            count++;
        s = s->nxt;
    }
    return count;
}

void loadDepartments()
{
    FILE *fp = fopen("data/departments.txt", "r");
    Department *d;
    if (!fp) return;
    while (1)
    {
        d = malloc(sizeof(Department));
        if (!d) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%100[^\n]\n", d->code, d->name) != 2) { free(d); break; }
        d->nxt = NULL;
        if (!departmentHead) departmentHead = d;
        else { Department *t = departmentHead; while (t->nxt) t = t->nxt; t->nxt = d; }
    }
    fclose(fp);
}

void saveDepartments()
{
    FILE *fp = fopen("data/departments.txt", "w");
    Department *t = departmentHead;
    if (!fp) { printError("Unable to save departments."); return; }
    while (t) { fprintf(fp, "%s|%s\n", t->code, t->name); t = t->nxt; }
    fclose(fp);
}

void loadBatches()
{
    FILE *fp = fopen("data/batches.txt", "r");
    Batch *b;
    if (!fp) return;
    while (1)
    {
        b = malloc(sizeof(Batch));
        if (!b) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%d\n", b->departmentCode, &b->batchYear) != 2) { free(b); break; }
        b->nxt = NULL;
        if (!batchHead) batchHead = b;
        else { Batch *t = batchHead; while (t->nxt) t = t->nxt; t->nxt = b; }
    }
    fclose(fp);
}

void saveBatches()
{
    FILE *fp = fopen("data/batches.txt", "w");
    Batch *t = batchHead;
    if (!fp) { printError("Unable to save batches."); return; }
    while (t) { fprintf(fp, "%s|%d\n", t->departmentCode, t->batchYear); t = t->nxt; }
    fclose(fp);
}

void loadSections()
{
    FILE *fp = fopen("data/sections.txt", "r");
    Section *s;
    if (!fp) return;
    while (1)
    {
        s = malloc(sizeof(Section));
        if (!s) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%d|%20[^|]|%d\n", s->departmentCode, &s->batchYear, s->sectionName, &s->capacity) != 4) { free(s); break; }
        s->nxt = NULL;
        if (!sectionHead) sectionHead = s;
        else { Section *t = sectionHead; while (t->nxt) t = t->nxt; t->nxt = s; }
    }
    fclose(fp);
}

void saveSections()
{
    FILE *fp = fopen("data/sections.txt", "w");
    Section *t = sectionHead;
    if (!fp) { printError("Unable to save sections."); return; }
    while (t) { fprintf(fp, "%s|%d|%s|%d\n", t->departmentCode, t->batchYear, t->sectionName, t->capacity); t = t->nxt; }
    fclose(fp);
}

void freeDepartments()
{
    Department *t; while (departmentHead) { t = departmentHead; departmentHead = departmentHead->nxt; free(t); }
}
void freeBatches()
{
    Batch *t; while (batchHead) { t = batchHead; batchHead = batchHead->nxt; free(t); }
}
void freeSections()
{
    Section *t; while (sectionHead) { t = sectionHead; sectionHead = sectionHead->nxt; free(t); }
}

int hasDependentBatch(const char *dept)
{
    Batch *b = batchHead;
    while (b != NULL) { if (strcmp(b->departmentCode, dept) == 0) return 1; b = b->nxt; }
    return 0;
}

int hasDependentSection(const char *dept, int year)
{
    Section *s = sectionHead;
    while (s != NULL) { if (strcmp(s->departmentCode, dept) == 0 && s->batchYear == year) return 1; s = s->nxt; }
    return 0;
}

int hasDependentCourse(const char *dept)
{
    Course *c = courseHead;
    while (c != NULL) { if (strcmp(c->department, dept) == 0) return 1; c = c->nxt; }
    return 0;
}

void addDepartment()
{
    char code[21], name[101];
    Department *d;
    readToken("Enter Department Code: ", code, 20);
    if (findDepartment(code)) { printError("Department already exists."); return; }
    readLine("Enter Department Name: ", name, sizeof(name));
    d = malloc(sizeof(Department));
    if (d == NULL) { printError("Memory allocation failed."); return; }
    strcpy(d->code, code);
    strcpy(d->name, name);
    d->nxt = NULL;
    if (departmentHead == NULL) departmentHead = d;
    else { Department *t = departmentHead; while (t->nxt != NULL) t = t->nxt; t->nxt = d; }
    saveDepartments();
    printSuccess("Department added.");
}

void listDepartments()
{
    Department *t = departmentHead;
    if (!t) { printInfo("No departments found."); return; }
    printf("\n%-10s %-30s\n", "Code", "Name");
    while (t) { printf("%-10s %-30s\n", t->code, t->name); t = t->nxt; }
}

void deleteDepartment()
{
    char code[21];
    Department *t = departmentHead, *p = NULL;
    readToken("Enter Department Code: ", code, 20);
    while (t != NULL && strcmp(t->code, code) != 0) { p = t; t = t->nxt; }
    if (t == NULL) { printError("Department not found."); return; }
    {
        Student *st = studentHead; Advisor *ad = advisorHead;
        while (st != NULL) { if (strcmp(st->department, code) == 0) { printError("Cannot delete department with assigned students."); return; } st = st->nxt; }
        while (ad != NULL) { if (strcmp(ad->department, code) == 0) { printError("Cannot delete department with assigned advisors."); return; } ad = ad->nxt; }
    }
    if (hasDependentBatch(code) || hasDependentCourse(code))
    {
        printError("Cannot delete department with dependent batches/courses.");
        return;
    }
    if (!confirmYesNo("Are you sure you want to delete this department? (Y/N): ")) return;
    if (p != NULL) p->nxt = t->nxt; else departmentHead = t->nxt;
    free(t);
    saveDepartments();
    printSuccess("Department deleted.");
}

void manageDepartments()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Departments ---\n1. Add\n2. View All\n3. Delete\n0. Back\n");
        c = readInt("Choice: ");
        if (c==1) addDepartment(); else if (c==2) listDepartments(); else if (c==3) deleteDepartment(); else if (c==0) return;
    }
}

void addBatch()
{
    char dept[21];
    int year;
    Batch *b;
    readToken("Enter Department Code: ", dept, 20);
    if (findDepartment(dept) == NULL) { printError("Department not found. Add department first."); return; }
    year = readIntRange("Enter Batch Year: ", 1, 3000);
    if (findBatch(dept, year) != NULL) { printError("Batch already exists."); return; }
    b = malloc(sizeof(Batch));
    if (b == NULL) { printError("Memory allocation failed."); return; }
    strcpy(b->departmentCode, dept);
    b->batchYear = year;
    b->nxt = NULL;
    if (batchHead == NULL) batchHead = b; else { Batch *t=batchHead; while(t->nxt!=NULL) t=t->nxt; t->nxt=b; }
    saveBatches();
    printSuccess("Batch added.");
}

void listBatches()
{
    Batch *t = batchHead;
    if (!t) { printInfo("No batches found."); return; }
    printf("\n%-10s %-10s\n", "Dept", "Year");
    while (t) { printf("%-10s %-10d\n", t->departmentCode, t->batchYear); t = t->nxt; }
}

void deleteBatch()
{
    char dept[21];
    int year;
    Batch *t = batchHead, *p = NULL;
    readToken("Enter Department Code: ", dept, 20);
    year = readInt("Enter Batch Year: ");
    while (t != NULL && !(strcmp(t->departmentCode, dept) == 0 && t->batchYear == year)) { p = t; t = t->nxt; }
    if (t == NULL) { printError("Batch not found."); return; }
    if (hasDependentSection(dept, year)) { printError("Cannot delete batch with dependent sections."); return; }
    if (!confirmYesNo("Are you sure you want to delete this batch? (Y/N): ")) return;
    if (p != NULL) p->nxt = t->nxt; else batchHead = t->nxt;
    free(t);
    saveBatches();
    printSuccess("Batch deleted.");
}

void manageBatches()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Batches ---\n1. Add\n2. View All\n3. Delete\n0. Back\n");
        c = readInt("Choice: ");
        if (c==1) addBatch(); else if (c==2) listBatches(); else if (c==3) deleteBatch(); else if (c==0) return;
    }
}

void addSection()
{
    char dept[21], name[21];
    int year, cap;
    Section *s;
    readToken("Enter Department Code: ", dept, 20);
    year = readInt("Enter Batch Year: ");
    if (findBatch(dept, year) == NULL) { printError("Batch not found. Add batch first."); return; }
    readToken("Enter Section Name: ", name, 20);
    if (findSection(dept, year, name) != NULL) { printError("Section already exists."); return; }
    cap = readIntRange("Enter Capacity: ", 1, 500);
    s = malloc(sizeof(Section));
    if (s == NULL) { printError("Memory allocation failed."); return; }
    strcpy(s->departmentCode, dept);
    s->batchYear = year;
    strcpy(s->sectionName, name);
    s->capacity = cap;
    s->nxt = NULL;
    if (sectionHead == NULL) sectionHead = s; else { Section *t=sectionHead; while(t->nxt!=NULL) t=t->nxt; t->nxt=s; }
    saveSections();
    printSuccess("Section added.");
}

void listSections()
{
    Section *t = sectionHead;
    if (!t) { printInfo("No sections found."); return; }
    printf("\n%-10s %-8s %-10s %-8s %-8s\n", "Dept", "Batch", "Section", "Cap", "Filled");
    while (t)
    {
        int filled = countStudentsInSection(t->departmentCode, t->batchYear, t->sectionName);
        printf("%-10s %-8d %-10s %-8d %-8d\n", t->departmentCode, t->batchYear, t->sectionName, t->capacity, filled);
        t = t->nxt;
    }
}

void deleteSection()
{
    char dept[21], name[21];
    int year;
    Section *t = sectionHead, *p = NULL;
    readToken("Enter Department Code: ", dept, 20);
    year = readInt("Enter Batch Year: ");
    readToken("Enter Section Name: ", name, 20);
    while (t != NULL && !(strcmp(t->departmentCode, dept) == 0 && t->batchYear == year && strcmp(t->sectionName, name) == 0)) { p = t; t = t->nxt; }
    if (t == NULL) { printError("Section not found."); return; }
    if (countStudentsInSection(dept, year, name) > 0) { printError("Cannot delete section with assigned students."); return; }
    if (!confirmYesNo("Are you sure you want to delete this section? (Y/N): ")) return;
    if (p != NULL) p->nxt = t->nxt; else sectionHead = t->nxt;
    free(t);
    saveSections();
    printSuccess("Section deleted.");
}

void manageSections()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Sections ---\n1. Add\n2. View All\n3. Delete\n0. Back\n");
        c = readInt("Choice: ");
        if (c==1) addSection(); else if (c==2) listSections(); else if (c==3) deleteSection(); else if (c==0) return;
    }
}

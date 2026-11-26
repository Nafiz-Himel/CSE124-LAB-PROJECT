#include "../include/dept_batch_section.h"
#include "../include/types.h"

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
    if (!fp) { printf("\n[ERROR] Unable to save departments.\n"); return; }
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
    if (!fp) { printf("\n[ERROR] Unable to save batches.\n"); return; }
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
    if (!fp) { printf("\n[ERROR] Unable to save sections.\n"); return; }
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

void addDepartment()
{
    char code[21], name[101];
    Department *d;
    printf("Enter Department Code: "); scanf("%20s", code);
    if (findDepartment(code)) { printf("\n[ERROR] Department already exists.\n"); return; }
    d = malloc(sizeof(Department)); if (!d) return;
    strcpy(d->code, code);
    printf("Enter Department Name: "); scanf(" %100[^\n]", name); strcpy(d->name, name);
    d->nxt = NULL;
    if (!departmentHead) departmentHead = d;
    else { Department *t = departmentHead; while (t->nxt) t = t->nxt; t->nxt = d; }
    saveDepartments();
    printf("\n[SUCCESS] Department added.\n");
}

void listDepartments()
{
    Department *t = departmentHead;
    if (!t) { printf("\nNo departments found.\n"); return; }
    printf("\n%-10s %-30s\n", "Code", "Name");
    while (t) { printf("%-10s %-30s\n", t->code, t->name); t = t->nxt; }
}

void deleteDepartment()
{
    char code[21];
    Department *t = departmentHead, *p = NULL;
    printf("Enter Department Code: "); scanf("%20s", code);
    while (t && strcmp(t->code, code) != 0) { p = t; t = t->nxt; }
    if (!t) { printf("\n[ERROR] Department not found.\n"); return; }
    if (p) p->nxt = t->nxt; else departmentHead = t->nxt;
    free(t); saveDepartments();
    printf("\n[SUCCESS] Department deleted.\n");
}

void manageDepartments()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Departments ---\n1. Add\n2. View All\n3. Delete\n0. Back\nChoice: ");
        if (scanf("%d", &c) != 1) { int ch; while ((ch=getchar())!='\n'&&ch!=EOF){} continue; }
        if (c==1) addDepartment(); else if (c==2) listDepartments(); else if (c==3) deleteDepartment(); else if (c==0) return;
    }
}

void addBatch()
{
    char dept[21]; int year; Batch *b;
    printf("Enter Department Code: "); scanf("%20s", dept);
    if (!findDepartment(dept)) { printf("\n[ERROR] Department not found. Add department first.\n"); return; }
    printf("Enter Batch Year: "); scanf("%d", &year);
    if (findBatch(dept, year)) { printf("\n[ERROR] Batch already exists.\n"); return; }
    b = malloc(sizeof(Batch)); if (!b) return;
    strcpy(b->departmentCode, dept); b->batchYear = year; b->nxt = NULL;
    if (!batchHead) batchHead = b; else { Batch *t=batchHead; while(t->nxt) t=t->nxt; t->nxt=b; }
    saveBatches(); printf("\n[SUCCESS] Batch added.\n");
}

void listBatches()
{
    Batch *t = batchHead;
    if (!t) { printf("\nNo batches found.\n"); return; }
    printf("\n%-10s %-10s\n", "Dept", "Year");
    while (t) { printf("%-10s %-10d\n", t->departmentCode, t->batchYear); t = t->nxt; }
}

void deleteBatch()
{
    char dept[21]; int year; Batch *t=batchHead,*p=NULL;
    printf("Enter Department Code: "); scanf("%20s", dept);
    printf("Enter Batch Year: "); scanf("%d", &year);
    while (t && !(strcmp(t->departmentCode, dept)==0 && t->batchYear==year)) { p=t; t=t->nxt; }
    if (!t) { printf("\n[ERROR] Batch not found.\n"); return; }
    if (p) p->nxt=t->nxt; else batchHead=t->nxt; free(t); saveBatches();
    printf("\n[SUCCESS] Batch deleted.\n");
}

void manageBatches()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Batches ---\n1. Add\n2. View All\n3. Delete\n0. Back\nChoice: ");
        if (scanf("%d", &c) != 1) { int ch; while ((ch=getchar())!='\n'&&ch!=EOF){} continue; }
        if (c==1) addBatch(); else if (c==2) listBatches(); else if (c==3) deleteBatch(); else if (c==0) return;
    }
}

void addSection()
{
    char dept[21], name[21]; int year, cap; Section *s;
    printf("Enter Department Code: "); scanf("%20s", dept);
    printf("Enter Batch Year: "); scanf("%d", &year);
    if (!findBatch(dept, year)) { printf("\n[ERROR] Batch not found. Add batch first.\n"); return; }
    printf("Enter Section Name: "); scanf("%20s", name);
    if (findSection(dept, year, name)) { printf("\n[ERROR] Section already exists.\n"); return; }
    printf("Enter Capacity: "); scanf("%d", &cap);
    s = malloc(sizeof(Section)); if (!s) return;
    strcpy(s->departmentCode, dept); s->batchYear=year; strcpy(s->sectionName,name); s->capacity=cap; s->nxt=NULL;
    if (!sectionHead) sectionHead=s; else { Section *t=sectionHead; while(t->nxt) t=t->nxt; t->nxt=s; }
    saveSections(); printf("\n[SUCCESS] Section added.\n");
}

void listSections()
{
    Section *t = sectionHead;
    if (!t) { printf("\nNo sections found.\n"); return; }
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
    char dept[21], name[21]; int year; Section *t=sectionHead,*p=NULL;
    printf("Enter Department Code: "); scanf("%20s", dept);
    printf("Enter Batch Year: "); scanf("%d", &year);
    printf("Enter Section Name: "); scanf("%20s", name);
    while (t && !(strcmp(t->departmentCode,dept)==0 && t->batchYear==year && strcmp(t->sectionName,name)==0)) { p=t; t=t->nxt; }
    if (!t) { printf("\n[ERROR] Section not found.\n"); return; }
    if (p) p->nxt=t->nxt; else sectionHead=t->nxt; free(t); saveSections();
    printf("\n[SUCCESS] Section deleted.\n");
}

void manageSections()
{
    int c;
    while (1)
    {
        printf("\n--- Manage Sections ---\n1. Add\n2. View All\n3. Delete\n0. Back\nChoice: ");
        if (scanf("%d", &c) != 1) { int ch; while ((ch=getchar())!='\n'&&ch!=EOF){} continue; }
        if (c==1) addSection(); else if (c==2) listSections(); else if (c==3) deleteSection(); else if (c==0) return;
    }
}

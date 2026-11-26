#include "../include/dept_batch_section.h"
#include "../include/types.h"
#include "../include/utils.h"

Batch *batchHead = NULL;
Section *sectionHead = NULL;

Batch *findBatch(int year)
{
    Batch *t = batchHead;
    while (t) { if (t->batchYear == year) return t; t = t->nxt; }
    return NULL;
}

Section *findSection(int year, const char *sectionName)
{
    Section *t = sectionHead;
    while (t)
    {
        if (t->batchYear == year && strcmp(t->sectionName, sectionName) == 0)
            return t;
        t = t->nxt;
    }
    return NULL;
}

int countStudentsInSection(int year, const char *sectionName)
{
    Student *s = studentHead;
    int count = 0;
    while (s)
    {
        if (s->batch == year && strcmp(s->section, sectionName) == 0)
            count++;
        s = s->nxt;
    }
    return count;
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
        if (fscanf(fp, "%d\n", &b->batchYear) != 1) { free(b); break; }
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
    while (t) { fprintf(fp, "%d\n", t->batchYear); t = t->nxt; }
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
        if (fscanf(fp, "%d|%20[^|]|%d\n", &s->batchYear, s->sectionName, &s->capacity) != 3) { free(s); break; }
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
    while (t) { fprintf(fp, "%d|%s|%d\n", t->batchYear, t->sectionName, t->capacity); t = t->nxt; }
    fclose(fp);
}

void freeBatches()
{
    Batch *t; while (batchHead) { t = batchHead; batchHead = batchHead->nxt; free(t); }
}

void freeSections()
{
    Section *t; while (sectionHead) { t = sectionHead; sectionHead = sectionHead->nxt; free(t); }
}

int hasDependentSection(int year)
{
    Section *s = sectionHead;
    while (s != NULL) { if (s->batchYear == year) return 1; s = s->nxt; }
    return 0;
}

void addBatch()
{
    int year;
    Batch *b;
    year = readIntRange("Enter Batch Year: ", 1, 3000);
    if (findBatch(year) != NULL) { printError("Batch already exists."); return; }
    b = malloc(sizeof(Batch));
    if (b == NULL) { printError("Memory allocation failed."); return; }
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
    printf("\n%-10s\n", "Year");
    while (t) { printf("%-10d\n", t->batchYear); t = t->nxt; }
}

void deleteBatch()
{
    int year;
    Batch *t = batchHead, *p = NULL;
    year = readInt("Enter Batch Year: ");
    while (t != NULL && t->batchYear != year) { p = t; t = t->nxt; }
    if (t == NULL) { printError("Batch not found."); return; }
    if (hasDependentSection(year)) { printError("Cannot delete batch with dependent sections."); return; }
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
        else printError("Invalid choice.");
    }
}

void addSection()
{
    char name[21];
    int year, cap;
    Section *s;
    year = readInt("Enter Batch Year: ");
    if (findBatch(year) == NULL) { printError("Batch not found. Add batch first."); return; }
    readToken("Enter Section Name: ", name, 20);
    if (findSection(year, name) != NULL) { printError("Section already exists."); return; }
    cap = readIntRange("Enter Capacity: ", 1, 500);
    s = malloc(sizeof(Section));
    if (s == NULL) { printError("Memory allocation failed."); return; }
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
    printf("\n%-10s %-12s %-8s %-8s\n", "Batch", "Section", "Cap", "Filled");
    while (t)
    {
        int filled = countStudentsInSection(t->batchYear, t->sectionName);
        printf("%-10d %-12s %-8d %-8d\n", t->batchYear, t->sectionName, t->capacity, filled);
        t = t->nxt;
    }
}

void deleteSection()
{
    char name[21];
    int year;
    Section *t = sectionHead, *p = NULL;
    year = readInt("Enter Batch Year: ");
    readToken("Enter Section Name: ", name, 20);
    while (t != NULL && !(t->batchYear == year && strcmp(t->sectionName, name) == 0)) { p = t; t = t->nxt; }
    if (t == NULL) { printError("Section not found."); return; }
    if (countStudentsInSection(year, name) > 0) { printError("Cannot delete section with assigned students."); return; }
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
        else printError("Invalid choice.");
    }
}

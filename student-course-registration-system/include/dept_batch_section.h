#ifndef DEPT_BATCH_SECTION_H
#define DEPT_BATCH_SECTION_H

typedef struct Department
{
    char code[21];
    char name[101];
    struct Department *nxt;
} Department;

typedef struct Batch
{
    char departmentCode[21];
    int batchYear;
    struct Batch *nxt;
} Batch;

typedef struct Section
{
    char departmentCode[21];
    int batchYear;
    char sectionName[21];
    int capacity;
    struct Section *nxt;
} Section;

extern Department *departmentHead;
extern Batch *batchHead;
extern Section *sectionHead;

void loadDepartments();
void loadBatches();
void loadSections();
void saveDepartments();
void saveBatches();
void saveSections();
void freeDepartments();
void freeBatches();
void freeSections();

void manageDepartments();
void manageBatches();
void manageSections();

Department *findDepartment(const char *code);
Batch *findBatch(const char *dept, int year);
Section *findSection(const char *dept, int year, const char *sectionName);
int countStudentsInSection(const char *dept, int year, const char *sectionName);

#endif

#ifndef DEPT_BATCH_SECTION_H
#define DEPT_BATCH_SECTION_H

#define SECTION_CAPACITY 40

typedef struct Batch
{
    int batchYear;
    struct Batch *nxt;
} Batch;

typedef struct Section
{
    int batchYear;
    char sectionName[21];
    int capacity;
    struct Section *nxt;
} Section;

extern Batch *batchHead;
extern Section *sectionHead;

void loadBatches();
void loadSections();
void saveBatches();
void saveSections();
void freeBatches();
void freeSections();

void manageBatches();
void manageSections();

Batch *findBatch(int year);
Section *findSection(int year, const char *sectionName);
int countStudentsInSection(int year, const char *sectionName);
int hasDependentSection(int year);

#endif

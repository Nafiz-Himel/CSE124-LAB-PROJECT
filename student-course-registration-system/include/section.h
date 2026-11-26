#ifndef SECTION_H
#define SECTION_H

typedef struct SectionRequest
{
    char requestID[21];
    char studentID[21];
    char department[21];
    int batch;
    char preferredSection[21];
    char alternativeSection[21];
    char status[21];
    struct SectionRequest *nxt;
} SectionRequest;

extern SectionRequest *sectionRequestHead;

void loadSectionRequests();
void saveSectionRequests();
void freeSectionRequests();
void manageSectionRequests();
SectionRequest *findPendingSectionRequestByStudent(const char *studentID);
SectionRequest *findSectionRequestByID(const char *requestID);

void viewAvailableSectionsForStudent(const char *studentID);
void submitSectionPreferences(const char *studentID);
void viewSectionRequestStatus(const char *studentID);
void respondToAlternativeSection(const char *studentID);
void requestSectionChange(const char *studentID);
void viewSectionChangeStatus(const char *studentID);

#endif

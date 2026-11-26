#ifndef REGISTRATION_H
#define REGISTRATION_H

typedef struct Registration
{
    char studentID[21];
    char courseCode[21];
    char status[21];
    struct Registration *nxt;
} Registration;

typedef struct WaitNode
{
    char studentID[21];
    char courseCode[21];
    struct WaitNode *nxt;
} WaitNode;

typedef struct WaitQueue
{
    char courseCode[21];
    WaitNode *front;
    WaitNode *rear;
    struct WaitQueue *nxt;
} WaitQueue;

typedef struct HistoryNode
{
    char studentID[21];
    char action[21];
    char courseCode[21];
    struct HistoryNode *nxt;
} HistoryNode;

extern Registration *registrationHead;
extern WaitQueue *waitQueueHead;
extern HistoryNode *historyTop;

void loadRegistrations();
void loadWaitlists();
void loadHistory();
void saveRegistrations();
void saveWaitlists();
void saveHistory();
void freeRegistrations();
void freeWaitlists();
void freeHistory();

void manageRegistration();
void viewStudentRegistrationForAdvisor();

void viewAvailableSectionsForStudent(const char *studentID);
void submitSectionPreferences(const char *studentID);
void viewSectionRequestStatus(const char *studentID);
void respondToAlternativeSection(const char *studentID);
void requestSectionChange(const char *studentID);
void viewSectionChangeStatus(const char *studentID);

void registerCourseForStudent(const char *studentID);
void dropCourseForStudent(const char *studentID);
void viewRegisteredCoursesForStudent(const char *studentID);
void viewTotalCreditsForStudent(const char *studentID);
void viewWaitingListStatusForStudent(const char *studentID);
void viewRegistrationHistoryForStudent(const char *studentID);

int countRegisteredCredits(const char *studentID);
int countCourseRegistrations(const char *courseCode);
int isStudentRegistered(const char *studentID, const char *courseCode);
void pushHistory(const char *studentID, const char *action, const char *courseCode);

#endif

#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <stddef.h>

typedef struct Registration
{
    char studentID[21];
    char courseCode[21];
    char section[21];
    char status[21];
    struct Registration *nxt;
} Registration;

typedef struct WaitNode
{
    char studentID[21];
    char courseCode[21];
    char section[21];
    struct WaitNode *nxt;
} WaitNode;

typedef struct WaitQueue
{
    char courseCode[21];
    char section[21];
    WaitNode *front;
    WaitNode *rear;
    struct WaitQueue *nxt;
} WaitQueue;

typedef struct HistoryNode
{
    char studentID[21];
    char action[21];
    char courseCode[21];
    char section[21];
    struct HistoryNode *nxt;
} HistoryNode;

typedef struct RegistrationRequest
{
    char requestID[21];
    char studentID[21];
    char courseCode[21];
    char section[21];
    char status[21];
    struct RegistrationRequest *nxt;
} RegistrationRequest;

extern Registration *registrationHead;
extern WaitQueue *waitQueueHead;
extern HistoryNode *historyTop;
extern RegistrationRequest *registrationRequestHead;

void loadRegistrations();
void loadWaitlists();
void loadHistory();
void loadRegistrationRequests();
void saveRegistrations();
void saveWaitlists();
void saveHistory();
void saveRegistrationRequests();
void freeRegistrations();
void freeWaitlists();
void freeHistory();
void freeRegistrationRequests();

void manageRegistration();
void viewStudentRegistrationForAdvisor(const char *advisorID);

void registerCourseForStudent(const char *studentID);
void dropCourseForStudent(const char *studentID);
void viewRegisteredCoursesForStudent(const char *studentID);
void viewTotalCreditsForStudent(const char *studentID);
void viewWaitingListStatusForStudent(const char *studentID);
void viewRegistrationHistoryForStudent(const char *studentID);

void submitRegistrationRequest(const char *studentID);
void viewRegistrationRequestStatus(const char *studentID);
void manageRegistrationRequestsForAdvisor(const char *advisorID);
void viewAllRegistrationRequests();

float countRegisteredCredits(const char *studentID);
float countPendingCredits(const char *studentID);
int countCourseRegistrations(const char *courseCode);
int countCourseSectionRegistrations(const char *courseCode, const char *section);
int countCourseSectionWaitlist(const char *courseCode, const char *section);
int isStudentRegistered(const char *studentID, const char *courseCode);
int validateRegistrationEligibility(const char *studentID, const char *courseCode,
                                    int includePending, int checkCapacity,
                                    char *errorOut, size_t errorSize);
void pushHistory(const char *studentID, const char *action, const char *courseCode, const char *section);
void enqueueWait(const char *courseCode, const char *section, const char *studentID);
int dequeueWait(const char *courseCode, const char *section, char *studentIDOut);

#endif

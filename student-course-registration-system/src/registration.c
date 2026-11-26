#include "../include/registration.h"
#include "../include/types.h"
#include "../include/course.h"
#include "../include/dept_batch_section.h"
#include "../include/utils.h"

Registration *registrationHead = NULL;
WaitQueue *waitQueueHead = NULL;
HistoryNode *historyTop = NULL;
RegistrationRequest *registrationRequestHead = NULL;

static Student *findStudentRecord(const char *studentID)
{
    Student *s = studentHead;
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    return s;
}

static void appendRegistration(Registration *r)
{
    r->nxt = NULL;
    if (registrationHead == NULL) registrationHead = r;
    else
    {
        Registration *t = registrationHead;
        while (t->nxt != NULL) t = t->nxt;
        t->nxt = r;
    }
}

static WaitQueue *getOrCreateQueue(const char *courseCode, const char *section)
{
    WaitQueue *q = waitQueueHead;
    while (q != NULL)
    {
        if (strcmp(q->courseCode, courseCode) == 0 && strcmp(q->section, section) == 0) return q;
        q = q->nxt;
    }
    q = malloc(sizeof(WaitQueue));
    if (q == NULL) return NULL;
    strcpy(q->courseCode, courseCode);
    strcpy(q->section, section);
    q->front = q->rear = NULL;
    q->nxt = waitQueueHead;
    waitQueueHead = q;
    return q;
}

static RegistrationRequest *findRegistrationRequestByID(const char *requestID)
{
    RegistrationRequest *r = registrationRequestHead;
    while (r != NULL)
    {
        if (strcmp(r->requestID, requestID) == 0) return r;
        r = r->nxt;
    }
    return NULL;
}

static int hasPendingRegistrationRequest(const char *studentID, const char *courseCode)
{
    RegistrationRequest *r = registrationRequestHead;
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->courseCode, courseCode) == 0 &&
            strcmp(r->status, "PENDING") == 0) return 1;
        r = r->nxt;
    }
    return 0;
}

static int nextRegistrationRequestSeq(void)
{
    RegistrationRequest *r = registrationRequestHead;
    int maxSeq = 0;
    while (r != NULL)
    {
        if (strncmp(r->requestID, "RR", 2) == 0)
        {
            int n = atoi(r->requestID + 2);
            if (n > maxSeq) maxSeq = n;
        }
        r = r->nxt;
    }
    return maxSeq + 1;
}

void pushHistory(const char *studentID, const char *action, const char *courseCode, const char *section)
{
    HistoryNode *n = malloc(sizeof(HistoryNode));
    if (n == NULL) return;
    strcpy(n->studentID, studentID);
    strcpy(n->action, action);
    strcpy(n->courseCode, (courseCode == NULL || courseCode[0] == '\0') ? "-" : courseCode);
    strcpy(n->section, (section == NULL || section[0] == '\0') ? "-" : section);
    n->nxt = historyTop;
    historyTop = n;
}

void enqueueWait(const char *courseCode, const char *section, const char *studentID)
{
    WaitQueue *q = getOrCreateQueue(courseCode, section);
    WaitNode *n;
    if (q == NULL) return;
    n = malloc(sizeof(WaitNode));
    if (n == NULL) return;
    strcpy(n->studentID, studentID);
    strcpy(n->courseCode, courseCode);
    strcpy(n->section, section);
    n->nxt = NULL;
    if (q->rear == NULL) q->front = q->rear = n;
    else { q->rear->nxt = n; q->rear = n; }
}

int dequeueWait(const char *courseCode, const char *section, char *studentIDOut)
{
    WaitQueue *q = waitQueueHead;
    WaitNode *n;
    while (q != NULL)
    {
        if (strcmp(q->courseCode, courseCode) == 0 && strcmp(q->section, section) == 0) break;
        q = q->nxt;
    }
    if (q == NULL || q->front == NULL) return 0;
    n = q->front;
    strcpy(studentIDOut, n->studentID);
    q->front = n->nxt;
    if (q->front == NULL) q->rear = NULL;
    free(n);
    return 1;
}

int countCourseRegistrations(const char *courseCode)
{
    Registration *r = registrationHead;
    int count = 0;
    while (r != NULL)
    {
        if (strcmp(r->courseCode, courseCode) == 0 && strcmp(r->status, "REGISTERED") == 0) count++;
        r = r->nxt;
    }
    return count;
}

int countCourseSectionRegistrations(const char *courseCode, const char *section)
{
    Registration *r = registrationHead;
    int count = 0;
    while (r != NULL)
    {
        if (strcmp(r->courseCode, courseCode) == 0 && strcmp(r->section, section) == 0 &&
            strcmp(r->status, "REGISTERED") == 0) count++;
        r = r->nxt;
    }
    return count;
}

int countCourseSectionWaitlist(const char *courseCode, const char *section)
{
    WaitQueue *q = waitQueueHead;
    int count = 0;
    while (q != NULL)
    {
        if (strcmp(q->courseCode, courseCode) == 0 && strcmp(q->section, section) == 0)
        {
            WaitNode *n = q->front;
            while (n != NULL) { count++; n = n->nxt; }
            return count;
        }
        q = q->nxt;
    }
    return count;
}

int isStudentRegistered(const char *studentID, const char *courseCode)
{
    Registration *r = registrationHead;
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->courseCode, courseCode) == 0 &&
            (strcmp(r->status, "REGISTERED") == 0 || strcmp(r->status, "WAITING") == 0)) return 1;
        r = r->nxt;
    }
    return 0;
}

float countRegisteredCredits(const char *studentID)
{
    Registration *r = registrationHead;
    float total = 0.0f;
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->status, "REGISTERED") == 0)
        {
            Course *c = findCourse(r->courseCode);
            if (c != NULL) total += c->credits;
        }
        r = r->nxt;
    }
    return total;
}

float countPendingCredits(const char *studentID)
{
    RegistrationRequest *r = registrationRequestHead;
    float total = 0.0f;
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->status, "PENDING") == 0)
        {
            Course *c = findCourse(r->courseCode);
            if (c != NULL) total += c->credits;
        }
        r = r->nxt;
    }
    return total;
}

int validateRegistrationEligibility(const char *studentID, const char *courseCode,
                                    int includePending, int checkCapacity,
                                    char *errorOut, size_t errorSize)
{
    Student *s = findStudentRecord(studentID);
    Course *c;
    Section *sec;
    if (s == NULL)
    {
        snprintf(errorOut, errorSize, "Student not found.");
        return 0;
    }
    c = findCourse(courseCode);
    if (c == NULL)
    {
        snprintf(errorOut, errorSize, "Course not found.");
        return 0;
    }
    if (s->section[0] == '\0')
    {
        snprintf(errorOut, errorSize, "No section allocated.");
        return 0;
    }
    sec = findSection(s->batch, s->section);
    if (sec == NULL)
    {
        snprintf(errorOut, errorSize, "Section not found for your batch.");
        return 0;
    }
    if (isStudentRegistered(studentID, courseCode))
    {
        snprintf(errorOut, errorSize, "Already registered or waiting for this course.");
        return 0;
    }
    if (includePending && hasPendingRegistrationRequest(studentID, courseCode))
    {
        snprintf(errorOut, errorSize, "Registration request already pending.");
        return 0;
    }
    if (!isCourseEligibleForSemester(courseCode, s->currentSemester))
    {
        snprintf(errorOut, errorSize, "Course is not offered in semester %d.", s->currentSemester);
        return 0;
    }
    if (!hasPrerequisitesSatisfied(studentID, courseCode))
    {
        snprintf(errorOut, errorSize, "Prerequisites not satisfied.");
        return 0;
    }
    if (countRegisteredCredits(studentID) + (includePending ? countPendingCredits(studentID) : 0.0f) + c->credits > 18.0f)
    {
        snprintf(errorOut, errorSize, "Credit limit exceeded (max 18).");
        return 0;
    }
    if (checkCapacity && countCourseSectionRegistrations(courseCode, s->section) >= sec->capacity)
    {
        snprintf(errorOut, errorSize, "Section is full.");
        return 0;
    }
    return 1;
}

void loadRegistrations()
{
    FILE *fp = fopen("data/registrations.txt", "r");
    Registration *r;
    if (fp == NULL) return;
    while (1)
    {
        r = malloc(sizeof(Registration));
        if (r == NULL) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%20[^|]|%20[^|]|%20[^\n]\n",
                   r->studentID, r->courseCode, r->section, r->status) != 4)
        { free(r); break; }
        appendRegistration(r);
    }
    fclose(fp);
}

void saveRegistrations()
{
    FILE *fp = fopen("data/registrations.txt", "w");
    Registration *t = registrationHead;
    if (fp == NULL) { printError("Unable to save registrations."); return; }
    while (t != NULL)
    {
        fprintf(fp, "%s|%s|%s|%s\n", t->studentID, t->courseCode, t->section, t->status);
        t = t->nxt;
    }
    fclose(fp);
}

void loadWaitlists()
{
    FILE *fp = fopen("data/waitlists.txt", "r");
    char sid[21], code[21], section[21];
    if (fp == NULL) return;
    while (fscanf(fp, "%20[^|]|%20[^|]|%20[^\n]\n", sid, code, section) == 3)
        enqueueWait(code, section, sid);
    fclose(fp);
}

void saveWaitlists()
{
    FILE *fp = fopen("data/waitlists.txt", "w");
    WaitQueue *q = waitQueueHead;
    if (fp == NULL) { printError("Unable to save waitlists."); return; }
    while (q != NULL)
    {
        WaitNode *n = q->front;
        while (n != NULL)
        {
            fprintf(fp, "%s|%s|%s\n", n->studentID, n->courseCode, n->section);
            n = n->nxt;
        }
        q = q->nxt;
    }
    fclose(fp);
}

void loadHistory()
{
    FILE *fp = fopen("data/history.txt", "r");
    char sid[21], action[21], code[21], section[21];
    HistoryNode *head = NULL, *tail = NULL;
    if (fp == NULL) return;
    while (fscanf(fp, "%20[^|]|%20[^|]|%20[^|]|%20[^\n]\n",
                  sid, action, code, section) == 4)
    {
        HistoryNode *n = malloc(sizeof(HistoryNode));
        if (n == NULL) break;
        strcpy(n->studentID, sid);
        strcpy(n->action, action);
        strcpy(n->courseCode, code);
        strcpy(n->section, section);
        n->nxt = NULL;
        if (head == NULL) head = tail = n;
        else { tail->nxt = n; tail = n; }
    }
    fclose(fp);
    while (head != NULL)
    {
        HistoryNode *next = head->nxt;
        head->nxt = historyTop;
        historyTop = head;
        head = next;
    }
}

void saveHistory()
{
    FILE *fp = fopen("data/history.txt", "w");
    HistoryNode *rev = NULL, *t = historyTop, *n;
    if (fp == NULL) { printError("Unable to save history."); return; }
    while (t != NULL) { n = t->nxt; t->nxt = rev; rev = t; t = n; }
    t = rev;
    while (t != NULL)
    {
        fprintf(fp, "%s|%s|%s|%s\n", t->studentID, t->action, t->courseCode, t->section);
        t = t->nxt;
    }
    t = rev;
    rev = NULL;
    while (t != NULL) { n = t->nxt; t->nxt = rev; rev = t; t = n; }
    historyTop = rev;
    fclose(fp);
}

void loadRegistrationRequests()
{
    FILE *fp = fopen("data/registration_requests.txt", "r");
    RegistrationRequest *r;
    if (fp == NULL) return;
    while (1)
    {
        r = malloc(sizeof(RegistrationRequest));
        if (r == NULL) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%20[^|]|%20[^|]|%20[^|]|%20[^\n]\n",
                   r->requestID, r->studentID, r->courseCode, r->section, r->status) != 5)
        { free(r); break; }
        r->nxt = NULL;
        if (registrationRequestHead == NULL) registrationRequestHead = r;
        else
        {
            RegistrationRequest *t = registrationRequestHead;
            while (t->nxt != NULL) t = t->nxt;
            t->nxt = r;
        }
    }
    fclose(fp);
}

void saveRegistrationRequests()
{
    FILE *fp = fopen("data/registration_requests.txt", "w");
    RegistrationRequest *t = registrationRequestHead;
    if (fp == NULL) { printError("Unable to save registration requests."); return; }
    while (t != NULL)
    {
        fprintf(fp, "%s|%s|%s|%s|%s\n", t->requestID, t->studentID, t->courseCode, t->section, t->status);
        t = t->nxt;
    }
    fclose(fp);
}

void freeRegistrations()
{
    Registration *t;
    while (registrationHead != NULL) { t = registrationHead; registrationHead = registrationHead->nxt; free(t); }
}

void freeWaitlists()
{
    WaitQueue *q;
    while (waitQueueHead != NULL)
    {
        q = waitQueueHead;
        waitQueueHead = waitQueueHead->nxt;
        while (q->front != NULL)
        {
            WaitNode *n = q->front;
            q->front = n->nxt;
            free(n);
        }
        free(q);
    }
}

void freeHistory()
{
    HistoryNode *t;
    while (historyTop != NULL) { t = historyTop; historyTop = historyTop->nxt; free(t); }
}

void freeRegistrationRequests()
{
    RegistrationRequest *t;
    while (registrationRequestHead != NULL)
    {
        t = registrationRequestHead;
        registrationRequestHead = registrationRequestHead->nxt;
        free(t);
    }
}

int countCourseWaitlist(const char *courseCode)
{
    WaitQueue *q = waitQueueHead;
    int count = 0;
    while (q != NULL)
    {
        if (strcmp(q->courseCode, courseCode) == 0)
        {
            WaitNode *n = q->front;
            while (n != NULL) { count++; n = n->nxt; }
        }
        q = q->nxt;
    }
    return count;
}

static void createRegistration(const char *studentID, const char *courseCode,
                               const char *section, const char *status)
{
    Registration *r = malloc(sizeof(Registration));
    if (r == NULL) { printError("Memory allocation failed."); return; }
    strcpy(r->studentID, studentID);
    strcpy(r->courseCode, courseCode);
    strcpy(r->section, section);
    strcpy(r->status, status);
    appendRegistration(r);
}

void registerCourseForStudent(const char *studentID)
{
    char code[21], err[256];
    Student *s = findStudentRecord(studentID);
    Course *c;
    if (s == NULL) { printError("Student not found."); return; }
    readToken("Enter Course Code: ", code, 20);
    if (!validateRegistrationEligibility(studentID, code, 0, 0, err, sizeof(err)))
    {
        printError(err);
        return;
    }
    c = findCourse(code);
    if (countCourseSectionRegistrations(code, s->section) >= SECTION_CAPACITY)
    {
        createRegistration(studentID, code, s->section, "WAITING");
        enqueueWait(code, s->section, studentID);
        saveWaitlists();
        pushHistory(studentID, "WAIT", code, s->section);
        printInfo("Section full. Added to waiting list.");
    }
    else
    {
        createRegistration(studentID, code, s->section, "REGISTERED");
        pushHistory(studentID, "REGISTER", code, s->section);
        printSuccess("Course registered.");
    }
    (void)c;
    saveRegistrations();
    saveHistory();
}

void submitRegistrationRequest(const char *studentID)
{
    char code[21], err[256];
    Student *s = findStudentRecord(studentID);
    RegistrationRequest *r;
    if (s == NULL) { printError("Student not found."); return; }
    readToken("Enter Course Code: ", code, 20);
    if (!validateRegistrationEligibility(studentID, code, 1, 1, err, sizeof(err)))
    {
        printError(err);
        return;
    }
    r = malloc(sizeof(RegistrationRequest));
    if (r == NULL) { printError("Memory allocation failed."); return; }
    snprintf(r->requestID, sizeof(r->requestID), "RR%04d", nextRegistrationRequestSeq());
    strcpy(r->studentID, studentID);
    strcpy(r->courseCode, code);
    strcpy(r->section, s->section);
    strcpy(r->status, "PENDING");
    r->nxt = NULL;
    if (registrationRequestHead == NULL) registrationRequestHead = r;
    else
    {
        RegistrationRequest *t = registrationRequestHead;
        while (t->nxt != NULL) t = t->nxt;
        t->nxt = r;
    }
    saveRegistrationRequests();
    pushHistory(studentID, "REG_REQ", code, s->section);
    saveHistory();
    printSuccess("Registration request submitted. Waiting for advisor approval.");
}

void viewRegistrationRequestStatus(const char *studentID)
{
    RegistrationRequest *r = registrationRequestHead;
    int found = 0;
    printf("\n--- Registration Requests ---\n");
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0)
        {
            printf("%s | %s | %s | %s\n", r->requestID, r->courseCode, r->section, r->status);
            found = 1;
        }
        r = r->nxt;
    }
    if (!found) printInfo("No registration requests found.");
}

static int isRequestForAdvisor(const RegistrationRequest *r, const char *advisorID)
{
    Student *s = findStudentRecord(r->studentID);
    return s != NULL && strcmp(s->advisorID, advisorID) == 0;
}

void manageRegistrationRequestsForAdvisor(const char *advisorID)
{
    int choice;
    while (1)
    {
        printf("\n--- Advisor Registration Requests ---\n");
        printf("1. View Pending Requests\n2. Approve Request\n3. Reject Request\n0. Back\n");
        choice = readInt("Enter choice: ");
        if (choice == 0) return;

        if (choice == 1)
        {
            RegistrationRequest *r = registrationRequestHead;
            int found = 0;
            while (r != NULL)
            {
                if (strcmp(r->status, "PENDING") == 0 && isRequestForAdvisor(r, advisorID))
                {
                    printf("%s | %s | %s | %s | %s\n",
                           r->requestID, r->studentID, r->courseCode, r->section, r->status);
                    found = 1;
                }
                r = r->nxt;
            }
            if (!found) printInfo("No pending registration requests.");
        }
        else if (choice == 2)
        {
            char reqID[21], err[256];
            RegistrationRequest *r;
            Student *s;
            readToken("Enter Request ID: ", reqID, 20);
            r = findRegistrationRequestByID(reqID);
            if (r == NULL) { printError("Request not found."); continue; }
            if (strcmp(r->status, "PENDING") != 0) { printError("Request is not pending."); continue; }
            if (!isRequestForAdvisor(r, advisorID)) { printError("Student is not assigned to you."); continue; }
            if (!validateRegistrationEligibility(r->studentID, r->courseCode, 0, 0, err, sizeof(err)))
            {
                printError(err);
                continue;
            }
            s = findStudentRecord(r->studentID);
            {
                Section *sec = findSection(s->batch, r->section);
                int cap = (sec != NULL) ? sec->capacity : SECTION_CAPACITY;
                if (countCourseSectionRegistrations(r->courseCode, r->section) >= cap)
                {
                    createRegistration(r->studentID, r->courseCode, r->section, "WAITING");
                    enqueueWait(r->courseCode, r->section, r->studentID);
                    saveWaitlists();
                    printInfo("Section full. Student added to waiting list.");
                }
                else
                {
                    createRegistration(r->studentID, r->courseCode, r->section, "REGISTERED");
                    printSuccess("Registration request approved.");
                }
            }
            strcpy(r->status, "APPROVED");
            pushHistory(r->studentID, "REG_APPROVE", r->courseCode, r->section);
            saveRegistrations();
            saveRegistrationRequests();
            saveHistory();
        }
        else if (choice == 3)
        {
            char reqID[21];
            RegistrationRequest *r;
            readToken("Enter Request ID: ", reqID, 20);
            r = findRegistrationRequestByID(reqID);
            if (r == NULL) { printError("Request not found."); continue; }
            if (strcmp(r->status, "PENDING") != 0) { printError("Request is not pending."); continue; }
            if (!isRequestForAdvisor(r, advisorID)) { printError("Student is not assigned to you."); continue; }
            strcpy(r->status, "REJECTED");
            pushHistory(r->studentID, "REG_REJECT", r->courseCode, r->section);
            saveRegistrationRequests();
            saveHistory();
            printSuccess("Registration request rejected.");
        }
        else printError("Invalid choice.");
    }
}

void viewAllRegistrationRequests()
{
    RegistrationRequest *r = registrationRequestHead;
    int count = 0;
    printf("\n--- All Registration Requests ---\n");
    while (r != NULL)
    {
        printf("%s | %s | %s | %s | %s\n", r->requestID, r->studentID, r->courseCode, r->section, r->status);
        count++;
        r = r->nxt;
    }
    if (count == 0) printInfo("No registration requests.");
    else printf("Total requests: %d\n", count);
}

void dropCourseForStudent(const char *studentID)
{
    char code[21];
    Registration *r = registrationHead;
    int wasRegistered = 0;
    readToken("Enter Course Code to drop: ", code, 20);
    while (r != NULL && !(strcmp(r->studentID, studentID) == 0 && strcmp(r->courseCode, code) == 0 &&
           (strcmp(r->status, "REGISTERED") == 0 || strcmp(r->status, "WAITING") == 0)))
    {
        r = r->nxt;
    }
    if (r == NULL) { printError("Active registration not found."); return; }

    wasRegistered = (strcmp(r->status, "REGISTERED") == 0);
    if (strcmp(r->status, "WAITING") == 0)
    {
        WaitQueue *q = waitQueueHead;
        while (q != NULL && !(strcmp(q->courseCode, r->courseCode) == 0 && strcmp(q->section, r->section) == 0))
            q = q->nxt;
        if (q != NULL)
        {
            WaitNode *wn = q->front, *wp = NULL;
            while (wn != NULL && strcmp(wn->studentID, studentID) != 0)
            {
                wp = wn;
                wn = wn->nxt;
            }
            if (wn != NULL)
            {
                if (wp != NULL) wp->nxt = wn->nxt; else q->front = wn->nxt;
                if (q->rear == wn) q->rear = wp;
                free(wn);
                saveWaitlists();
            }
        }
    }

    {
        char savedCode[21], savedSection[21];
        strcpy(savedCode, r->courseCode);
        strcpy(savedSection, r->section);
        strcpy(r->status, "DROPPED");
        pushHistory(studentID, "DROP", savedCode, savedSection);

        if (wasRegistered)
        {
            char nextID[21];
            if (dequeueWait(savedCode, savedSection, nextID))
            {
                Registration *wr = registrationHead;
                while (wr != NULL)
                {
                    if (strcmp(wr->studentID, nextID) == 0 && strcmp(wr->courseCode, savedCode) == 0 &&
                        strcmp(wr->section, savedSection) == 0 && strcmp(wr->status, "WAITING") == 0)
                    {
                        strcpy(wr->status, "REGISTERED");
                        break;
                    }
                    wr = wr->nxt;
                }
                saveRegistrations();
                saveWaitlists();
                pushHistory(nextID, "PROMOTED", savedCode, savedSection);
                saveHistory();
                printf("\n[INFO] Waitlisted student %s promoted to REGISTERED.\n", nextID);
            }
        }
    }
    saveRegistrations();
    saveHistory();
    printSuccess("Course dropped.");
}

void viewRegisteredCoursesForStudent(const char *studentID)
{
    Registration *r = registrationHead;
    int count = 0;
    printf("\n--- Registered Courses ---\n");
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0)
        {
            printf("%s | %s | %s\n", r->courseCode, r->section, r->status);
            count++;
        }
        r = r->nxt;
    }
    if (count == 0) printf("No registration records.\n");
}

void viewTotalCreditsForStudent(const char *studentID)
{
    printf("\nTotal Registered Credits: %.1f / 18\n", countRegisteredCredits(studentID));
}

void viewWaitingListStatusForStudent(const char *studentID)
{
    Registration *r = registrationHead;
    int found = 0;
    printf("\n--- Waiting List Status ---\n");
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->status, "WAITING") == 0)
        {
            printf("%s | %s | WAITING\n", r->courseCode, r->section);
            found = 1;
        }
        r = r->nxt;
    }
    if (!found) printInfo("No waiting list entries.");
}

void viewRegistrationHistoryForStudent(const char *studentID)
{
    HistoryNode *h = historyTop;
    int found = 0;
    printf("\n--- Registration History ---\n");
    while (h != NULL)
    {
        if (strcmp(h->studentID, studentID) == 0)
        {
            printf("%s | %s | %s\n", h->action, h->courseCode, h->section);
            found = 1;
        }
        h = h->nxt;
    }
    if (!found) printInfo("No history records.");
}

void viewStudentRegistrationForAdvisor(const char *advisorID)
{
    char studentID[21];
    Student *s;
    readToken("Enter Student ID: ", studentID, 20);
    s = findStudentRecord(studentID);
    if (s == NULL) { printError("Student not found."); return; }
    if (strcmp(s->advisorID, advisorID) != 0)
    {
        printError("You can only view registration for your assigned students.");
        return;
    }
    viewRegisteredCoursesForStudent(studentID);
}

void manageRegistration()
{
    int c;
    char studentID[21];
    while (1)
    {
        printf("\n--- Manage Registration ---\n");
        printf("1. View All Registrations\n2. Register Student to Course (Override)\n");
        printf("3. Drop Student from Course\n4. View Waitlists\n5. View All Registration Requests\n0. Back\n");
        c = readInt("Choice: ");
        if (c == 1)
        {
            Registration *r = registrationHead;
            if (r == NULL) printInfo("No registrations.");
            while (r != NULL)
            {
                printf("%s | %s | %s | %s\n", r->studentID, r->courseCode, r->section, r->status);
                r = r->nxt;
            }
        }
        else if (c == 2)
        {
            readToken("Enter Student ID: ", studentID, 20);
            if (!studentExists(studentID)) { printError("Student not found."); continue; }
            registerCourseForStudent(studentID);
        }
        else if (c == 3)
        {
            readToken("Enter Student ID: ", studentID, 20);
            if (!studentExists(studentID)) { printError("Student not found."); continue; }
            dropCourseForStudent(studentID);
        }
        else if (c == 4)
        {
            WaitQueue *q = waitQueueHead;
            if (q == NULL) printInfo("No waitlists.");
            while (q != NULL)
            {
                WaitNode *n = q->front;
                int pos = 1;
                printf("\n%s | %s:\n", q->courseCode, q->section);
                while (n != NULL) { printf("  %d. %s\n", pos++, n->studentID); n = n->nxt; }
                q = q->nxt;
            }
        }
        else if (c == 5) viewAllRegistrationRequests();
        else if (c == 0) return;
        else printError("Invalid choice.");
    }
}

#include "../include/registration.h"
#include "../include/types.h"
#include "../include/course.h"
#include "../include/dept_batch_section.h"
#include "../include/utils.h"

Registration *registrationHead = NULL;
WaitQueue *waitQueueHead = NULL;
HistoryNode *historyTop = NULL;

void pushHistory(const char *studentID, const char *action, const char *courseCode)
{
    HistoryNode *n = malloc(sizeof(HistoryNode));
    if (!n) return;
    strcpy(n->studentID, studentID);
    strcpy(n->action, action);
    strcpy(n->courseCode, courseCode);
    n->nxt = historyTop;
    historyTop = n;
}

WaitQueue *getOrCreateQueue(const char *courseCode)
{
    WaitQueue *q = waitQueueHead;
    while (q) { if (strcmp(q->courseCode, courseCode) == 0) return q; q = q->nxt; }
    q = malloc(sizeof(WaitQueue));
    if (!q) return NULL;
    strcpy(q->courseCode, courseCode);
    q->front = q->rear = NULL;
    q->nxt = waitQueueHead;
    waitQueueHead = q;
    return q;
}

void enqueueWait(const char *courseCode, const char *studentID)
{
    WaitQueue *q = getOrCreateQueue(courseCode);
    WaitNode *n;
    if (!q) return;
    n = malloc(sizeof(WaitNode)); if (!n) return;
    strcpy(n->studentID, studentID); strcpy(n->courseCode, courseCode); n->nxt = NULL;
    if (q->rear == NULL) q->front = q->rear = n;
    else { q->rear->nxt = n; q->rear = n; }
}

int dequeueWait(const char *courseCode, char *outStudentID)
{
    WaitQueue *q = waitQueueHead;
    WaitNode *n;
    while (q && strcmp(q->courseCode, courseCode) != 0) q = q->nxt;
    if (!q || !q->front) return 0;
    n = q->front;
    strcpy(outStudentID, n->studentID);
    q->front = n->nxt;
    if (q->front == NULL) q->rear = NULL;
    free(n);
    return 1;
}

int countCourseRegistrations(const char *courseCode)
{
    Registration *r = registrationHead; int c = 0;
    while (r) { if (strcmp(r->courseCode, courseCode) == 0 && strcmp(r->status, "REGISTERED") == 0) c++; r = r->nxt; }
    return c;
}

int isStudentRegistered(const char *studentID, const char *courseCode)
{
    Registration *r = registrationHead;
    while (r)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->courseCode, courseCode) == 0 &&
            (strcmp(r->status, "REGISTERED") == 0 || strcmp(r->status, "WAITING") == 0)) return 1;
        r = r->nxt;
    }
    return 0;
}

int countRegisteredCredits(const char *studentID)
{
    Registration *r = registrationHead; int total = 0;
    while (r)
    {
        if (strcmp(r->studentID, studentID) == 0 && strcmp(r->status, "REGISTERED") == 0)
        {
            Course *c = findCourse(r->courseCode);
            if (c) total += c->credits;
        }
        r = r->nxt;
    }
    return total;
}

void loadRegistrations()
{
    FILE *fp = fopen("data/registrations.txt", "r"); Registration *r;
    if (!fp) return;
    while (1)
    {
        r = malloc(sizeof(Registration)); if (!r) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%20[^|]|%20[^\n]\n", r->studentID, r->courseCode, r->status) != 3) { free(r); break; }
        r->nxt = NULL;
        if (!registrationHead) registrationHead = r; else { Registration *t=registrationHead; while(t->nxt) t=t->nxt; t->nxt=r; }
    }
    fclose(fp);
}

void saveRegistrations()
{
    FILE *fp = fopen("data/registrations.txt", "w"); Registration *t = registrationHead;
    if (!fp) { printf("\n[ERROR] Unable to save registrations.\n"); return; }
    while (t) { fprintf(fp, "%s|%s|%s\n", t->studentID, t->courseCode, t->status); t = t->nxt; }
    fclose(fp);
}

void loadWaitlists()
{
    FILE *fp = fopen("data/waitlists.txt", "r"); char sid[21], code[21];
    if (!fp) return;
    while (fscanf(fp, "%20[^|]|%20[^\n]\n", sid, code) == 2) enqueueWait(code, sid);
    fclose(fp);
}

void saveWaitlists()
{
    FILE *fp = fopen("data/waitlists.txt", "w"); WaitQueue *q = waitQueueHead;
    if (!fp) { printf("\n[ERROR] Unable to save waitlists.\n"); return; }
    while (q)
    {
        WaitNode *n = q->front;
        while (n) { fprintf(fp, "%s|%s\n", n->studentID, n->courseCode); n = n->nxt; }
        q = q->nxt;
    }
    fclose(fp);
}

void loadHistory()
{
    FILE *fp = fopen("data/history.txt", "r"); char sid[21], action[21], code[21];
    HistoryNode *head = NULL, *tail = NULL;
    if (!fp) return;
    while (fscanf(fp, "%20[^|]|%20[^|]|%20[^\n]\n", sid, action, code) == 3)
    {
        HistoryNode *n = malloc(sizeof(HistoryNode)); if (!n) break;
        strcpy(n->studentID, sid); strcpy(n->action, action); strcpy(n->courseCode, code); n->nxt = NULL;
        if (!head) head = tail = n; else { tail->nxt = n; tail = n; }
    }
    fclose(fp);
    while (head)
    {
        HistoryNode *next = head->nxt;
        head->nxt = historyTop; historyTop = head;
        head = next;
    }
}

void saveHistory()
{
    FILE *fp = fopen("data/history.txt", "w");
    HistoryNode *rev = NULL, *t = historyTop, *n;
    if (!fp) { printf("\n[ERROR] Unable to save history.\n"); return; }
    while (t) { n = t->nxt; t->nxt = rev; rev = t; t = n; }
    t = rev;
    while (t) { fprintf(fp, "%s|%s|%s\n", t->studentID, t->action, t->courseCode); t = t->nxt; }
    while (rev) { n = rev->nxt; rev->nxt = historyTop; historyTop = rev; rev = n; }
    fclose(fp);
}

void freeRegistrations() { Registration *t; while (registrationHead) { t=registrationHead; registrationHead=registrationHead->nxt; free(t); } }
void freeWaitlists()
{
    WaitQueue *q; while (waitQueueHead) { q=waitQueueHead; waitQueueHead=waitQueueHead->nxt; WaitNode *n; while(q->front){n=q->front;q->front=n->nxt;free(n);} free(q);} }
void freeHistory() { HistoryNode *t; while (historyTop) { t=historyTop; historyTop=historyTop->nxt; free(t); } }

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
            return count;
        }
        q = q->nxt;
    }
    return count;
}

void registerCourseForStudent(const char *studentID)
{
    char code[21];
    Course *c;
    Registration *r;
    Student *s = studentHead;
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    if (s == NULL) { printError("Student not found."); return; }
    readToken("Enter Course Code: ", code, 20);
    c = findCourse(code);
    if (c == NULL) { printError("Course not found."); return; }
    if (strcmp(c->department, s->department) != 0) { printError("Course department mismatch."); return; }
    if (isStudentRegistered(studentID, code)) { printError("Already registered or waiting for this course."); return; }
    if (!hasPrerequisitesSatisfied(studentID, code)) { printError("Prerequisites not satisfied."); return; }
    if (countRegisteredCredits(studentID) + c->credits > 18) { printError("Credit limit exceeded (max 18)."); return; }

    r = malloc(sizeof(Registration));
    if (r == NULL) { printError("Memory allocation failed."); return; }
    strcpy(r->studentID, studentID);
    strcpy(r->courseCode, code);
    if (countCourseRegistrations(code) >= c->capacity)
    {
        strcpy(r->status, "WAITING");
        enqueueWait(code, studentID);
        saveWaitlists();
        printInfo("Course full. Added to waiting list.");
        pushHistory(studentID, "WAIT", code);
    }
    else
    {
        strcpy(r->status, "REGISTERED");
        printSuccess("Course registered.");
        pushHistory(studentID, "REGISTER", code);
    }
    r->nxt = NULL;
    if (registrationHead == NULL) registrationHead = r;
    else { Registration *t = registrationHead; while (t->nxt != NULL) t = t->nxt; t->nxt = r; }
    saveRegistrations();
    saveHistory();
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
    if (r == NULL)
    {
        printError("Active registration not found.");
        return;
    }
    wasRegistered = (strcmp(r->status, "REGISTERED") == 0);
    if (strcmp(r->status, "WAITING") == 0)
    {
        WaitQueue *q = waitQueueHead;
        while (q != NULL && strcmp(q->courseCode, code) != 0) q = q->nxt;
        if (q != NULL)
        {
            WaitNode *wn = q->front;
            WaitNode *wp = NULL;
            while (wn != NULL && strcmp(wn->studentID, studentID) != 0)
            {
                wp = wn;
                wn = wn->nxt;
            }
            if (wn != NULL)
            {
                if (wp != NULL) wp->nxt = wn->nxt;
                else q->front = wn->nxt;
                if (q->rear == wn) q->rear = wp;
                free(wn);
                saveWaitlists();
            }
        }
    }
    strcpy(r->status, "DROPPED");
    saveRegistrations();
    pushHistory(studentID, "DROP", code);
    saveHistory();
    printSuccess("Course dropped.");

    if (wasRegistered)
    {
        char nextID[21];
        if (dequeueWait(code, nextID))
        {
            Registration *wr = registrationHead;
            while (wr != NULL)
            {
                if (strcmp(wr->studentID, nextID) == 0 && strcmp(wr->courseCode, code) == 0 && strcmp(wr->status, "WAITING") == 0)
                {
                    strcpy(wr->status, "REGISTERED");
                    break;
                }
                wr = wr->nxt;
            }
            saveRegistrations();
            saveWaitlists();
            pushHistory(nextID, "PROMOTED", code);
            saveHistory();
            printf("\n[INFO] Waitlisted student %s promoted to REGISTERED.\n", nextID);
        }
    }
}

void viewRegisteredCoursesForStudent(const char *studentID)
{
    Registration *r = registrationHead; int count=0;
    printf("\n--- Registered Courses ---\n");
    while (r)
    {
        if (strcmp(r->studentID, studentID)==0)
        {
            printf("%s | %s\n", r->courseCode, r->status);
            count++;
        }
        r = r->nxt;
    }
    if (count==0) printf("No registration records.\n");
}

void viewTotalCreditsForStudent(const char *studentID)
{
    printf("\nTotal Registered Credits: %d / 18\n", countRegisteredCredits(studentID));
}

void viewWaitingListStatusForStudent(const char *studentID)
{
    Registration *r = registrationHead; int found=0;
    printf("\n--- Waiting List Status ---\n");
    while (r)
    {
        if (strcmp(r->studentID,studentID)==0 && strcmp(r->status,"WAITING")==0)
        {
            printf("%s | WAITING\n", r->courseCode); found=1;
        }
        r=r->nxt;
    }
    if (!found) printInfo("No waiting list entries.");
}

void viewRegistrationHistoryForStudent(const char *studentID)
{
    HistoryNode *h = historyTop; int found=0;
    printf("\n--- Registration History ---\n");
    while (h)
    {
        if (strcmp(h->studentID, studentID)==0)
        { printf("%s | %s\n", h->action, h->courseCode); found=1; }
        h = h->nxt;
    }
    if (!found) printInfo("No history records.");
}

void viewStudentRegistrationForAdvisor(const char *advisorID)
{
    char studentID[21];
    Student *s;
    readToken("Enter Student ID: ", studentID, 20);
    s = studentHead;
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    if (s == NULL)
    {
        printError("Student not found.");
        return;
    }
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
        printf("1. View All Registrations\n2. Register Student to Course\n3. Drop Student from Course\n4. View Waitlists\n0. Back\n");
        c = readInt("Choice: ");
        if (c == 1)
        {
            Registration *r = registrationHead;
            if (r == NULL) printInfo("No registrations.");
            while (r != NULL) { printf("%s | %s | %s\n", r->studentID, r->courseCode, r->status); r = r->nxt; }
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
                printf("\nCourse %s:\n", q->courseCode);
                while (n != NULL) { printf("  %d. %s\n", pos++, n->studentID); n = n->nxt; }
                q = q->nxt;
            }
        }
        else if (c == 0) return;
        else printError("Invalid choice.");
    }
}

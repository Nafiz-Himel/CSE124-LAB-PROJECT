#include "../include/section.h"
#include "../include/types.h"
#include "../include/dept_batch_section.h"
#include "../include/registration.h"
#include "../include/utils.h"

SectionRequest *sectionRequestHead = NULL;

SectionRequest *findPendingSectionRequestByStudent(const char *studentID)
{
    SectionRequest *r = sectionRequestHead;
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0 &&
            (strcmp(r->status, "PENDING") == 0 || strcmp(r->status, "ALT_OFFERED") == 0))
        {
            return r;
        }
        r = r->nxt;
    }
    return NULL;
}

SectionRequest *findSectionRequestByID(const char *requestID)
{
    SectionRequest *r = sectionRequestHead;
    while (r != NULL)
    {
        if (strcmp(r->requestID, requestID) == 0)
        {
            return r;
        }
        r = r->nxt;
    }
    return NULL;
}

void loadSectionRequests()
{
    FILE *fp = fopen("data/section_requests.txt", "r");
    SectionRequest *r;
    if (fp == NULL) return;
    while (1)
    {
        r = malloc(sizeof(SectionRequest));
        if (r == NULL) { fclose(fp); return; }
        if (fscanf(fp, "%20[^|]|%20[^|]|%d|%20[^|]|%20[^|]|%20[^\n]\n",
                   r->requestID, r->studentID, &r->batch,
                   r->preferredSection, r->alternativeSection, r->status) != 6)
        {
            free(r);
            break;
        }
        if (strcmp(r->alternativeSection, "-") == 0) r->alternativeSection[0] = '\0';
        r->nxt = NULL;
        if (sectionRequestHead == NULL) sectionRequestHead = r;
        else
        {
            SectionRequest *t = sectionRequestHead;
            while (t->nxt != NULL) t = t->nxt;
            t->nxt = r;
        }
    }
    fclose(fp);
}

void saveSectionRequests()
{
    FILE *fp = fopen("data/section_requests.txt", "w");
    SectionRequest *t = sectionRequestHead;
    if (fp == NULL) { printError("Unable to save section requests."); return; }
    while (t != NULL)
    {
        fprintf(fp, "%s|%s|%d|%s|%s|%s\n",
                t->requestID, t->studentID, t->batch,
                t->preferredSection,
                t->alternativeSection[0] == '\0' ? "-" : t->alternativeSection,
                t->status);
        t = t->nxt;
    }
    fclose(fp);
}

void freeSectionRequests()
{
    SectionRequest *t;
    while (sectionRequestHead != NULL)
    {
        t = sectionRequestHead;
        sectionRequestHead = sectionRequestHead->nxt;
        free(t);
    }
}

void viewAvailableSectionsForStudent(const char *studentID)
{
    Student *s = studentHead;
    Section *sec;
    int found = 0;
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    if (s == NULL) { printError("Student not found."); return; }
    printf("\n--- Available Sections for Batch %d ---\n", s->batch);
    sec = sectionHead;
    while (sec != NULL)
    {
        if (sec->batchYear == s->batch)
        {
            int filled = countStudentsInSection(sec->batchYear, sec->sectionName);
            printf("Section: %s | Capacity: %d | Filled: %d | Available: %d\n",
                   sec->sectionName, sec->capacity, filled, sec->capacity - filled);
            found = 1;
        }
        sec = sec->nxt;
    }
    if (!found) printInfo("No sections found for your batch.");
}

int nextSectionRequestSeq(const char *prefix)
{
    SectionRequest *r = sectionRequestHead;
    int maxSeq = 0;
    int pLen = (int)strlen(prefix);
    while (r != NULL)
    {
        if (strncmp(r->requestID, prefix, pLen) == 0)
        {
            int n = atoi(r->requestID + pLen);
            if (n > maxSeq) maxSeq = n;
        }
        r = r->nxt;
    }
    return maxSeq + 1;
}

void submitSectionPreferences(const char *studentID)
{
    Student *s = studentHead;
    Section *sec;
    SectionRequest *r;
    char pref[21];
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    if (s == NULL) { printError("Student not found."); return; }
    if (s->section[0] != '\0')
    {
        printInfo("You already have an allocated section. Use Request Section Change.");
        return;
    }
    if (findPendingSectionRequestByStudent(studentID) != NULL)
    {
        printInfo("You already have a pending section request.");
        return;
    }
    viewAvailableSectionsForStudent(studentID);
    readToken("Enter preferred section name: ", pref, 20);
    sec = findSection(s->batch, pref);
    if (sec == NULL) { printError("Section not found for your batch."); return; }
    r = malloc(sizeof(SectionRequest));
    if (r == NULL) { printError("Memory allocation failed."); return; }
    snprintf(r->requestID, sizeof(r->requestID), "SR%04d", nextSectionRequestSeq("SR"));
    strcpy(r->studentID, studentID);
    r->batch = s->batch;
    strcpy(r->preferredSection, pref);
    r->alternativeSection[0] = '\0';
    strcpy(r->status, "PENDING");
    r->nxt = NULL;
    if (sectionRequestHead == NULL) sectionRequestHead = r;
    else
    {
        SectionRequest *t = sectionRequestHead;
        while (t->nxt != NULL) t = t->nxt;
        t->nxt = r;
    }
    saveSectionRequests();
    pushHistory(studentID, "SEC_REQ", pref);
    saveHistory();
    printSuccess("Section request submitted. Waiting for admin approval.");
}

void viewSectionRequestStatus(const char *studentID)
{
    SectionRequest *r = sectionRequestHead;
    int found = 0;
    Student *s = studentHead;
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    if (s != NULL)
    {
        printf("\nCurrent Section: %s\n", s->section[0] == '\0' ? "Not allocated" : s->section);
    }
    printf("\n--- Section Requests ---\n");
    while (r != NULL)
    {
        if (strcmp(r->studentID, studentID) == 0)
        {
            printf("RequestID: %s | Preferred: %s | Alt: %s | Status: %s\n",
                   r->requestID, r->preferredSection,
                   r->alternativeSection[0] == '\0' ? "-" : r->alternativeSection,
                   r->status);
            found = 1;
        }
        r = r->nxt;
    }
    if (!found) printInfo("No section requests found.");
}

void respondToAlternativeSection(const char *studentID)
{
    SectionRequest *r = findPendingSectionRequestByStudent(studentID);
    char choice;
    if (r == NULL || strcmp(r->status, "ALT_OFFERED") != 0)
    {
        printInfo("No alternative section offer pending.");
        return;
    }
    printf("\nAlternative section offered: %s\n", r->alternativeSection);
    if (confirmYesNo("Accept alternative section? (Y/N): "))
    {
        Student *s = studentHead;
        Section *sec = findSection(r->batch, r->alternativeSection);
        while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
        if (s == NULL || sec == NULL)
        {
            printError("Unable to process alternative section.");
            return;
        }
        if (countStudentsInSection(r->batch, r->alternativeSection) >= sec->capacity)
        {
            printError("Alternative section is now full.");
            return;
        }
        strcpy(s->section, r->alternativeSection);
        strcpy(r->status, "APPROVED");
        saveStudents();
        saveSectionRequests();
        pushHistory(studentID, "SEC_ALT_ACC", r->alternativeSection);
        saveHistory();
        printSuccess("Alternative section accepted and allocated.");
    }
    else
    {
        strcpy(r->status, "REJECTED");
        saveSectionRequests();
        pushHistory(studentID, "SEC_ALT_REJ", r->alternativeSection);
        saveHistory();
        printInfo("Alternative section rejected.");
    }
    (void)choice;
}

void requestSectionChange(const char *studentID)
{
    Student *s = studentHead;
    SectionRequest *r;
    char pref[21];
    while (s != NULL && strcmp(s->studentID, studentID) != 0) s = s->nxt;
    if (s == NULL) { printError("Student not found."); return; }
    if (s->section[0] == '\0') { printInfo("No current section allocated. Submit section preference first."); return; }
    if (findPendingSectionRequestByStudent(studentID) != NULL) { printInfo("You already have a pending request."); return; }
    viewAvailableSectionsForStudent(studentID);
    readToken("Enter requested new section: ", pref, 20);
    if (strcmp(pref, s->section) == 0) { printError("Requested section is same as current section."); return; }
    if (findSection(s->batch, pref) == NULL) { printError("Section not found."); return; }
    r = malloc(sizeof(SectionRequest));
    if (r == NULL) { printError("Memory allocation failed."); return; }
    snprintf(r->requestID, sizeof(r->requestID), "SC%04d", nextSectionRequestSeq("SC"));
    strcpy(r->studentID, studentID);
    r->batch = s->batch;
    strcpy(r->preferredSection, pref);
    r->alternativeSection[0] = '\0';
    strcpy(r->status, "PENDING");
    r->nxt = NULL;
    if (sectionRequestHead == NULL) sectionRequestHead = r;
    else { SectionRequest *t = sectionRequestHead; while (t->nxt != NULL) t = t->nxt; t->nxt = r; }
    saveSectionRequests();
    pushHistory(studentID, "SEC_CHG_REQ", pref);
    saveHistory();
    printSuccess("Section change request submitted.");
}

void viewSectionChangeStatus(const char *studentID)
{
    viewSectionRequestStatus(studentID);
}

void manageSectionRequests()
{
    int choice;
    while (1)
    {
        printf("\n--- Manage Section Requests ---\n");
        printf("1. View Pending Requests\n");
        printf("2. Approve Request\n");
        printf("3. Reject Request\n");
        printf("4. Offer Alternative Section\n");
        printf("0. Back\n");
        choice = readInt("Enter choice: ");
        if (choice == 0) return;
        if (choice == 1)
        {
            SectionRequest *r = sectionRequestHead;
            int found = 0;
            while (r != NULL)
            {
                if (strcmp(r->status, "PENDING") == 0 || strcmp(r->status, "ALT_OFFERED") == 0)
                {
                    printf("%s | %s | Batch %d | Pref:%s | Alt:%s | %s\n", r->requestID, r->studentID, r->batch,
                           r->preferredSection, r->alternativeSection[0] == '\0' ? "-" : r->alternativeSection, r->status);
                    found = 1;
                }
                r = r->nxt;
            }
            if (!found) printInfo("No pending section requests.");
        }
        else if (choice == 2)
        {
            char reqID[21];
            SectionRequest *r;
            Student *s;
            Section *sec;
            readToken("Enter Request ID: ", reqID, 20);
            r = findSectionRequestByID(reqID);
            if (r == NULL) { printError("Request not found."); continue; }
            if (strcmp(r->status, "PENDING") != 0) { printError("Request is not pending."); continue; }
            sec = findSection(r->batch, r->preferredSection);
            if (sec == NULL) { printError("Preferred section no longer exists."); continue; }
            if (countStudentsInSection(r->batch, r->preferredSection) >= sec->capacity)
            {
                printError("Preferred section is full. Offer alternative instead.");
                continue;
            }
            s = studentHead;
            while (s != NULL && strcmp(s->studentID, r->studentID) != 0) s = s->nxt;
            if (s == NULL) { printError("Student not found."); continue; }
            strcpy(s->section, r->preferredSection);
            strcpy(r->status, "APPROVED");
            saveStudents();
            saveSectionRequests();
            pushHistory(r->studentID, "SEC_APPROVE", r->preferredSection);
            saveHistory();
            printSuccess("Section request approved.");
        }
        else if (choice == 3)
        {
            char reqID[21];
            SectionRequest *r;
            readToken("Enter Request ID: ", reqID, 20);
            r = findSectionRequestByID(reqID);
            if (r == NULL) { printError("Request not found."); continue; }
            if (strcmp(r->status, "PENDING") != 0 && strcmp(r->status, "ALT_OFFERED") != 0) { printError("Request is not active."); continue; }
            strcpy(r->status, "REJECTED");
            saveSectionRequests();
            pushHistory(r->studentID, "SEC_REJECT", r->preferredSection);
            saveHistory();
            printSuccess("Section request rejected.");
        }
        else if (choice == 4)
        {
            char reqID[21], alt[21];
            SectionRequest *r;
            Section *sec;
            readToken("Enter Request ID: ", reqID, 20);
            r = findSectionRequestByID(reqID);
            if (r == NULL) { printError("Request not found."); continue; }
            if (strcmp(r->status, "PENDING") != 0) { printError("Request is not pending."); continue; }
            readToken("Enter alternative section: ", alt, 20);
            sec = findSection(r->batch, alt);
            if (sec == NULL) { printError("Alternative section not found."); continue; }
            if (countStudentsInSection(r->batch, alt) >= sec->capacity) { printError("Alternative section is full."); continue; }
            strcpy(r->alternativeSection, alt);
            strcpy(r->status, "ALT_OFFERED");
            saveSectionRequests();
            pushHistory(r->studentID, "SEC_ALT", alt);
            saveHistory();
            printSuccess("Alternative section offered.");
        }
        else
        {
            printError("Invalid choice.");
        }
    }
}

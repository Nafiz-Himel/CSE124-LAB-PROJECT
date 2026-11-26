#include "../include/reports.h"
#include "../include/types.h"
#include "../include/dept_batch_section.h"
#include "../include/course.h"
#include "../include/registration.h"
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

void printLine()
{
    printf("------------------------------------------------------------\n");
}

void exportReport(const char *name)
{
    char filename[256];
    FILE *src, *dst;
    int ch;
    snprintf(filename, sizeof(filename), "reports/%s.txt", name);
    MKDIR("reports");
    src = fopen("reports/last_report.txt", "r");
    if (src == NULL)
    {
        printf("\n[ERROR] No report generated yet.\n");
        return;
    }
    dst = fopen(filename, "w");
    if (dst == NULL)
    {
        printf("\n[ERROR] Unable to export report.\n");
        fclose(src);
        return;
    }
    while ((ch = fgetc(src)) != EOF)
        fputc(ch, dst);
    fclose(src);
    fclose(dst);
    printf("\n[SUCCESS] Report exported to %s\n", filename);
}

FILE *openReport(const char *title)
{
    MKDIR("reports");
    FILE *fp = fopen("reports/last_report.txt", "w");
    if (fp == NULL)
    {
        printf("\n[ERROR] Unable to create report file.\n");
        return NULL;
    }
    printf("\n========== %s ==========\n", title);
    fprintf(fp, "========== %s ==========\n", title);
    return fp;
}

void closeReport(FILE *fp)
{
    if (fp != NULL)
        fclose(fp);
}

void out(FILE *fp, const char *text)
{
    printf("%s", text);
    if (fp) fprintf(fp, "%s", text);
}

void reportSystemSummary()
{
    FILE *fp = openReport("SYSTEM SUMMARY REPORT");
    char buf[512];
    int adminCount = 0, studentCount = 0, advisorCount = 0;
    int assigned = 0, unassigned = 0;
    Admin *a = adminHead;
    Student *s = studentHead;
    Advisor *ad = advisorHead;
    if (!fp) return;
    while (a) { adminCount++; a = a->nxt; }
    while (s) { studentCount++; if (s->advisorID[0] == '\0') unassigned++; else assigned++; s = s->nxt; }
    while (ad) { advisorCount++; ad = ad->nxt; }

    snprintf(buf, sizeof(buf), "Total Admins: %d\n", adminCount); out(fp, buf);
    snprintf(buf, sizeof(buf), "Total Students: %d\n", studentCount); out(fp, buf);
    snprintf(buf, sizeof(buf), "Total Advisors: %d\n", advisorCount); out(fp, buf);
    snprintf(buf, sizeof(buf), "Assigned Students: %d\n", assigned); out(fp, buf);
    snprintf(buf, sizeof(buf), "Unassigned Students: %d\n", unassigned); out(fp, buf);
    if (advisorCount > 0)
    {
        snprintf(buf, sizeof(buf), "Average Students/Advisor: %.2f\n", (float)assigned / advisorCount);
        out(fp, buf);
    }

    printLine(); if (fp) fprintf(fp, "------------------------------------------------------------\n");
    out(fp, "Advisor Workload Snapshot:\n");
    ad = advisorHead;
    while (ad)
    {
        int count = 0;
        s = studentHead;
        while (s)
        {
            if (strcmp(s->advisorID, ad->advisorID) == 0) count++;
            s = s->nxt;
        }
        snprintf(buf, sizeof(buf), "%s | %s | Dept: %s | Students: %d\n", ad->advisorID, ad->name, ad->department, count);
        out(fp, buf);
        ad = ad->nxt;
    }
    closeReport(fp);
}

void reportAllStudents()
{
    FILE *fp = openReport("ALL STUDENTS REPORT");
    char buf[512];
    Student *s = studentHead;
    int count = 0;
    if (!fp) return;
    snprintf(buf, sizeof(buf), "%-12s %-20s %-8s %-6s %-8s %-10s\n", "ID", "Name", "Dept", "Batch", "Section", "Advisor");
    out(fp, buf);
    printLine(); if (fp) fprintf(fp, "------------------------------------------------------------\n");
    while (s)
    {
        snprintf(buf, sizeof(buf), "%-12s %-20.20s %-8s %-6d %-8s %-10s\n",
                 s->studentID, s->name, s->department, s->batch, s->section,
                 s->advisorID[0] == '\0' ? "-" : s->advisorID);
        out(fp, buf);
        count++;
        s = s->nxt;
    }
    snprintf(buf, sizeof(buf), "Total Students: %d\n", count); out(fp, buf);
    closeReport(fp);
}

void reportStudentsByDepartment()
{
    char dept[21];
    char buf[512];
    Student *s;
    int count = 0;
    FILE *fp;
    printf("Enter Department (or ALL): ");
    scanf("%20s", dept);
    fp = openReport("STUDENTS BY DEPARTMENT REPORT");
    if (!fp) return;
    snprintf(buf, sizeof(buf), "Filter: %s\n", dept); out(fp, buf);
    printLine(); if (fp) fprintf(fp, "------------------------------------------------------------\n");
    s = studentHead;
    while (s)
    {
        if (strcmp(dept, "ALL") == 0 || strcmp(s->department, dept) == 0)
        {
            snprintf(buf, sizeof(buf), "%-12s %-20.20s %-8s %-6d %-8s\n", s->studentID, s->name, s->department, s->batch, s->section);
            out(fp, buf);
            count++;
        }
        s = s->nxt;
    }
    snprintf(buf, sizeof(buf), "Total: %d\n", count); out(fp, buf);
    closeReport(fp);
}

void reportStudentsByBatchSection()
{
    int batch;
    char section[21];
    char buf[512];
    Student *s;
    int count = 0;
    FILE *fp;
    printf("Enter Batch (0 for ALL): ");
    scanf("%d", &batch);
    printf("Enter Section (ALL for any): ");
    scanf("%20s", section);
    fp = openReport("STUDENTS BY BATCH/SECTION REPORT");
    if (!fp) return;
    snprintf(buf, sizeof(buf), "Filter Batch: %d, Section: %s\n", batch, section); out(fp, buf);
    printLine(); if (fp) fprintf(fp, "------------------------------------------------------------\n");
    s = studentHead;
    while (s)
    {
        int batchOk = (batch == 0 || s->batch == batch);
        int sectionOk = (strcmp(section, "ALL") == 0 || strcmp(s->section, section) == 0);
        if (batchOk && sectionOk)
        {
            snprintf(buf, sizeof(buf), "%-12s %-20.20s %-8s %-6d %-8s\n", s->studentID, s->name, s->department, s->batch, s->section);
            out(fp, buf);
            count++;
        }
        s = s->nxt;
    }
    snprintf(buf, sizeof(buf), "Total: %d\n", count); out(fp, buf);
    closeReport(fp);
}

void reportAdvisorWorkload()
{
    FILE *fp = openReport("ADVISOR WORKLOAD REPORT");
    char buf[512];
    Advisor *ad = advisorHead;
    int totalAdvisors = 0, totalAssigned = 0, maxLoad = -1, minLoad = -1;
    if (!fp) return;
    while (ad)
    {
        Student *s = studentHead;
        int count = 0;
        while (s)
        {
            if (strcmp(s->advisorID, ad->advisorID) == 0) count++;
            s = s->nxt;
        }
        snprintf(buf, sizeof(buf), "%-10s %-20.20s %-8s Assigned: %d\n", ad->advisorID, ad->name, ad->department, count);
        out(fp, buf);
        if (minLoad == -1 || count < minLoad) minLoad = count;
        if (count > maxLoad) maxLoad = count;
        totalAdvisors++;
        totalAssigned += count;
        ad = ad->nxt;
    }
    if (totalAdvisors == 0) { out(fp, "No advisors found.\n"); closeReport(fp); return; }
    snprintf(buf, sizeof(buf), "Total Advisors: %d\n", totalAdvisors); out(fp, buf);
    snprintf(buf, sizeof(buf), "Total Assigned: %d\n", totalAssigned); out(fp, buf);
    snprintf(buf, sizeof(buf), "Average Load: %.2f\n", (float)totalAssigned / totalAdvisors); out(fp, buf);
    snprintf(buf, sizeof(buf), "Min Load: %d, Max Load: %d\n", minLoad, maxLoad); out(fp, buf);
    closeReport(fp);
}

void reportUnassignedStudents()
{
    FILE *fp = openReport("UNASSIGNED STUDENTS REPORT");
    char buf[512];
    Student *s = studentHead;
    int count = 0;
    if (!fp) return;
    while (s)
    {
        if (s->advisorID[0] == '\0')
        {
            snprintf(buf, sizeof(buf), "%-12s %-20.20s %-8s %-6d\n", s->studentID, s->name, s->department, s->batch);
            out(fp, buf);
            count++;
        }
        s = s->nxt;
    }
    snprintf(buf, sizeof(buf), "Total Unassigned: %d\n", count); out(fp, buf);
    closeReport(fp);
}

void reportAdvisorDetail()
{
    char advisorID[51];
    char buf[512];
    Advisor *ad = advisorHead;
    Student *s;
    int count = 0;
    FILE *fp;
    printf("Enter Advisor ID: ");
    scanf("%50s", advisorID);
    while (ad && strcmp(ad->advisorID, advisorID) != 0) ad = ad->nxt;
    fp = openReport("ADVISOR DETAIL REPORT");
    if (!fp) return;
    if (!ad) { out(fp, "Advisor not found.\n"); closeReport(fp); return; }
    snprintf(buf, sizeof(buf), "Advisor: %s (%s), Dept: %s\n", ad->advisorID, ad->name, ad->department); out(fp, buf);
    printLine(); if (fp) fprintf(fp, "------------------------------------------------------------\n");
    s = studentHead;
    while (s)
    {
        if (strcmp(s->advisorID, advisorID) == 0)
        {
            snprintf(buf, sizeof(buf), "%-12s %-20.20s %-8s %-6d %-8s\n", s->studentID, s->name, s->department, s->batch, s->section);
            out(fp, buf);
            count++;
        }
        s = s->nxt;
    }
    snprintf(buf, sizeof(buf), "Total Assigned Students: %d\n", count); out(fp, buf);
    closeReport(fp);
}

void reportDataIntegrity()
{
    FILE *fp = openReport("DATA INTEGRITY REPORT");
    char buf[512];
    Student *s = studentHead;
    int issues = 0;
    if (!fp) return;
    while (s)
    {
        if (s->advisorID[0] != '\0')
        {
            Advisor *ad = advisorHead;
            int found = 0;
            while (ad) { if (strcmp(ad->advisorID, s->advisorID) == 0) { found = 1; break; } ad = ad->nxt; }
            if (!found)
            {
                snprintf(buf, sizeof(buf), "[ISSUE] Student %s has orphan advisor ID: %s\n", s->studentID, s->advisorID);
                out(fp, buf); issues++;
            }
            else if (strcmp(ad->department, s->department) != 0)
            {
                snprintf(buf, sizeof(buf), "[ISSUE] Dept mismatch: Student %s (%s) - Advisor %s (%s)\n", s->studentID, s->department, ad->advisorID, ad->department);
                out(fp, buf); issues++;
            }
        }
        if (s->studentID[0] == '\0' || s->name[0] == '\0')
        {
            snprintf(buf, sizeof(buf), "[ISSUE] Student record missing critical fields.\n"); out(fp, buf); issues++;
        }
        s = s->nxt;
    }
    {
        Student *a = studentHead;
        while (a)
        {
            Student *b = a->nxt;
            while (b)
            {
                if (strcmp(a->studentID, b->studentID) == 0)
                {
                    snprintf(buf, sizeof(buf), "[ISSUE] Duplicate student ID: %s\n", a->studentID); out(fp, buf); issues++;
                }
                b = b->nxt;
            }
            a = a->nxt;
        }
    }
    if (issues == 0) out(fp, "No integrity issues found.\n");
    else { snprintf(buf, sizeof(buf), "Total Issues: %d\n", issues); out(fp, buf); }
    closeReport(fp);
}

void reportCourseRegistrationSummary()
{
    FILE *fp = openReport("COURSE & REGISTRATION REPORT");
    char buf[512];
    Course *c = courseHead;
    Registration *r = registrationHead;
    int courseCount = 0, regCount = 0;
    if (!fp) return;
    if (c == NULL && r == NULL)
    {
        out(fp, "Course/Registration module data not available yet or empty.\n");
        closeReport(fp);
        return;
    }
    out(fp, "Courses:\n");
    while (c)
    {
        int enrolled = 0;
        r = registrationHead;
        while (r) { if (strcmp(r->courseCode, c->code) == 0 && strcmp(r->status, "REGISTERED") == 0) enrolled++; r = r->nxt; }
        snprintf(buf, sizeof(buf), "%-10s %-20.20s Dept:%-6s Credits:%d Cap:%d Enrolled:%d\n", c->code, c->name, c->department, c->credits, c->capacity, enrolled);
        out(fp, buf);
        courseCount++; c = c->nxt;
    }
    r = registrationHead;
    while (r) { regCount++; r = r->nxt; }
    snprintf(buf, sizeof(buf), "Total Courses: %d\n", courseCount); out(fp, buf);
    snprintf(buf, sizeof(buf), "Total Registration Records: %d\n", regCount); out(fp, buf);
    closeReport(fp);
}

void viewReports()
{
    int choice;
    while (1)
    {
        printf("\n========== REPORTS MENU ==========\n");
        printf("1. System Summary\n");
        printf("2. All Students\n");
        printf("3. Students by Department\n");
        printf("4. Students by Batch/Section\n");
        printf("5. Advisor Workload\n");
        printf("6. Unassigned Students\n");
        printf("7. Advisor Detail\n");
        printf("8. Data Integrity\n");
        printf("9. Course/Registration Summary\n");
        printf("10. Export Last Report\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) { int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} continue; }

        switch (choice)
        {
        case 1: reportSystemSummary(); break;
        case 2: reportAllStudents(); break;
        case 3: reportStudentsByDepartment(); break;
        case 4: reportStudentsByBatchSection(); break;
        case 5: reportAdvisorWorkload(); break;
        case 6: reportUnassignedStudents(); break;
        case 7: reportAdvisorDetail(); break;
        case 8: reportDataIntegrity(); break;
        case 9: reportCourseRegistrationSummary(); break;
        case 10: exportReport("exported_report"); break;
        case 0: return;
        default: printf("\n[ERROR] Invalid choice!\n");
        }
    }
}

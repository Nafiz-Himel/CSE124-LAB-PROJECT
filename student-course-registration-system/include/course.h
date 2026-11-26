#ifndef COURSE_H
#define COURSE_H

typedef struct Course
{
    char code[21];
    char name[101];
    float credits;
    int capacity;
    struct Course *nxt;
} Course;

typedef struct Prerequisite
{
    char courseCode[21];
    char prereqCode[21];
    struct Prerequisite *nxt;
} Prerequisite;

extern Course *courseHead;
extern Prerequisite *prereqHead;

void loadCourses();
void loadPrerequisites();
void saveCourses();
void savePrerequisites();
void freeCourses();
void freePrerequisites();

void manageCourses();
void viewAvailableCourses();
void searchCourse();
void viewCourseDetails();
void viewCoursePrerequisites();

Course *findCourse(const char *code);
int prereqExists(const char *courseCode, const char *prereqCode);
int hasPrerequisitesSatisfied(const char *studentID, const char *courseCode);
int countCourseWaitlist(const char *courseCode);

#endif

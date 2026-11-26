#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ADMIN 1
#define ADVISOR 2
#define STUDENT 3

typedef struct Admin
{
    char adminID[51];
    char name[101];
    char email[101];
    char password[101];
    struct Admin *nxt;
} Admin;

typedef struct Student
{
    char studentID[21];
    char name[101];
    char department[21];
    int batch;
    char section[21];
    char email[101];
    char phone[21];
    char advisorID[51];
    char password[101];
    struct Student *nxt;
} Student;

typedef struct Advisor
{
    char advisorID[51];
    char name[101];
    char department[21];
    char email[101];
    char phone[21];
    char password[101];
    struct Advisor *nxt;
} Advisor;

extern Admin *adminHead;
extern Student *studentHead;
extern Advisor *advisorHead;

void saveStudents();
void saveAdvisors();

#endif

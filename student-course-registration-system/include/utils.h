#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

void clearInputBuffer();
int readInt(const char *prompt);
int readIntRange(const char *prompt, int min, int max);
void readToken(const char *prompt, char *buffer, int width);
void readLine(const char *prompt, char *buffer, int size);
int confirmYesNo(const char *prompt);
int isEmptyString(const char *s);
void printSuccess(const char *msg);
void printError(const char *msg);
void printInfo(const char *msg);
void ensureDataDir();
void ensureReportsDir();

#endif

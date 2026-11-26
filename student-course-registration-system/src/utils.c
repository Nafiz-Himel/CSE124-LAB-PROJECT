#include "../include/utils.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

void clearInputBuffer()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

int readInt(const char *prompt)
{
    int value;
    int result;
    while (1)
    {
        printf("%s", prompt);
        result = scanf("%d", &value);
        if (result == 1)
        {
            clearInputBuffer();
            return value;
        }
        clearInputBuffer();
        printf("\n[ERROR] Invalid number. Please try again.\n");
    }
}

int readIntRange(const char *prompt, int min, int max)
{
    int value;
    while (1)
    {
        value = readInt(prompt);
        if (value >= min && value <= max)
        {
            return value;
        }
        printf("\n[ERROR] Please enter a value between %d and %d.\n", min, max);
    }
}

void readToken(const char *prompt, char *buffer, int width)
{
    char format[16];
    while (1)
    {
        printf("%s", prompt);
        snprintf(format, sizeof(format), "%%%ds", width);
        if (scanf(format, buffer) == 1)
        {
            clearInputBuffer();
            if (!isEmptyString(buffer))
            {
                return;
            }
        }
        else
        {
            clearInputBuffer();
        }
        printf("\n[ERROR] Input cannot be empty.\n");
    }
}

void readLine(const char *prompt, char *buffer, int size)
{
    char format[16];
    while (1)
    {
        printf("%s", prompt);
        snprintf(format, sizeof(format), " %%%d[^\n]", size - 1);
        if (scanf(format, buffer) == 1)
        {
            clearInputBuffer();
            if (!isEmptyString(buffer))
            {
                return;
            }
        }
        else
        {
            clearInputBuffer();
        }
        printf("\n[ERROR] Input cannot be empty.\n");
    }
}

int confirmYesNo(const char *prompt)
{
    char c;
    while (1)
    {
        printf("%s", prompt);
        if (scanf(" %c", &c) == 1)
        {
            clearInputBuffer();
            if (c == 'Y' || c == 'y') return 1;
            if (c == 'N' || c == 'n') return 0;
        }
        else
        {
            clearInputBuffer();
        }
        printf("\n[ERROR] Please enter Y or N.\n");
    }
}

int isEmptyString(const char *s)
{
    while (*s)
    {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

void printSuccess(const char *msg)
{
    printf("\n[SUCCESS] %s\n", msg);
}

void printError(const char *msg)
{
    printf("\n[ERROR] %s\n", msg);
}

void printInfo(const char *msg)
{
    printf("\n[INFO] %s\n", msg);
}

void ensureDataDir()
{
    MKDIR("data");
}

void ensureReportsDir()
{
    MKDIR("reports");
}

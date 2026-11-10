#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ADMIN 1
#define ADVISOR 2
#define STUDENT 3

void login(int role);

int main()
{
    int choice;

    while (1)
    {
        printf("\n================================\n");
        printf(" DIU COURSE REGISTRATION SYSTEM\n");
        printf("================================\n\n");

        printf("1. Admin Login\n");
        printf("2. Advisor Login\n");
        printf("3. Student Login\n");
        printf("0. Exit\n\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            login(ADMIN);
            break;
        case 2:
            login(ADVISOR);
            break;
        case 3:
            login(STUDENT);
            break;
        case 0:
            printf("\nThank you for using DIU Course Registration System!\n");
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

void login(int role)
{
    char inputID[101];
    char inputPassword[101];

    char expectedID[101];
    char expectedPassword[101];
    char roleName[101];

    if (role == ADMIN)
    {
        strcpy(expectedID, "A001");
        strcpy(expectedPassword, "admin123");
        strcpy(roleName, "Admin");
    }
    else if (role == ADVISOR)
    {
        strcpy(expectedID, "ADV001");
        strcpy(expectedPassword, "advisor123");
        strcpy(roleName, "Advisor");
    }
    else if (role == STUDENT)
    {
        strcpy(expectedID, "261-15-249");
        strcpy(expectedPassword, "student123");
        strcpy(roleName, "Student");
    }
    else
    {
        printf("Invalid role!\n");
        return;
    }

    printf("\n--- %s Login ---\n", roleName);
    printf("Enter ID: ");
    scanf("%100s", inputID);
    printf("Enter Password: ");
    scanf("%100s", inputPassword);

    if (!strcmp(inputID, expectedID) && !strcmp(inputPassword, expectedPassword))
    {
        printf("\n[SUCCESS] Welcome, %s! Login successful.\n", roleName);
    }
    else
    {
        printf("\n[ERROR] Invalid ID or Password for %s login.\n", roleName);
    }
}

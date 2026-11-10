#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void adminLogin();
void advisorLogin();
void studentLogin();

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
            adminLogin();
            break;

        case 2:
            advisorLogin();
            break;

        case 3:
            studentLogin();
            break;

        case 0:
            printf("\nExiting system. Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}

void adminLogin()
{
    printf("\nWelcome Admin!!!\n");
}
void advisorLogin()
{
    printf("\nWelcome Advisor!!!\n");
}
void studentLogin()
{
    printf("\nWelcome Student!!!\n");
}
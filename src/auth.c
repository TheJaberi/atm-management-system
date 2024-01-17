#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(struct User *u)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    fgets(u->name, sizeof(u->name), stdin);
    u->name[strcspn(u->name, "\n")] = 0; // Remove newline character if present

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    fgets(u->password, sizeof(u->password), stdin);
    u->password[strcspn(u->password, "\n")] = 0; // Remove newline character if present

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
}

const char *getPassword(struct User u)
{
    FILE *fp;
    static struct User userChecker;

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    // Convert the passed username to lowercase once before the loop
    for (int i = 0; u.name[i]; i++)
    {
        u.name[i] = tolower(u.name[i]);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        // Convert the read username from the file to lowercase
        for (int i = 0; userChecker.name[i]; i++)
        {
            userChecker.name[i] = tolower(userChecker.name[i]);
        }

        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            return userChecker.password;
        }
    }

    fclose(fp);
    return "no user found";
}

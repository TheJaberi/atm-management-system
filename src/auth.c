#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(struct User *u)
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", u->name);

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
    scanf("%s", u->password);

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
    static struct User userChecker;  // Made this static to ensure the memory remains valid

    fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {

        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            return userChecker.password;
        }
    }

    fclose(fp);
    return "no user found";
}

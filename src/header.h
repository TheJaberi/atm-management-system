#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[100];
    char country[100];
    long phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(struct User *u);
void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
void checkAllAccounts(struct User u);
// my code
void registerUser();
int getNewUserId();
void updateAccountInfo(struct User u);
void checkAccountDetails(struct User u);
void appendToTransactionRecords(const char *line);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwnership(struct User u);
void flushInputBuffer();
int getUidFromUsersFile(const char *filename, const char *username);

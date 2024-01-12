#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %ld %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %ld %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

bool isValidDate(int month, int day, int year)
{
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;
    if (year < 1000 || year > 9999)
        return false; // Adjust the range as needed
    return true;
}

bool isValidCountry(char *country)
{
    for (int i = 0; i < strlen(country); i++)
    {
        if (!isalpha(country[i]) && country[i] != ' ')
        {
            return false;
        }
    }
    return true;
}

int getNewRecordId()
{
    FILE *fp = fopen(RECORDS, "r");
    if (!fp)
    {
        perror("Unable to open the file");
        return 0;
    }

    struct Record record;
    int lastId = 0;
    char userName[50];
    while (getAccountFromFile(fp, userName, &record))
    {
        lastId = record.id; // Keep updating lastId until the end of the file to get the last id
    }

    fclose(fp);
    return lastId + 1;
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

    if (!pf)
    {
        perror("Unable to open the file");
        return;
    }

    r.id = getNewRecordId();

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    // Validation for deposit date
    bool validDate = false;
    while (!validDate)
    {
        printf("\nEnter today's date(mm/dd/yyyy):");
        int scanResult = scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
        if (scanResult != 3 || !isValidDate(r.deposit.month, r.deposit.day, r.deposit.year))
        {
            printf("Invalid date format. Please enter a valid date.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
        }
        else
        {
            validDate = true;
        }
    }

    // Validation for account number
    bool validAccountNumber = false;
    while (!validAccountNumber)
    {
        printf("\nEnter the account number:");
        scanf("%d", &r.accountNbr);
        if (r.accountNbr <= 0)
        {
            printf("Account number cannot be negative or zero. Please enter a valid account number.\n");
        }
        if (r.accountNbr < 0 || r.accountNbr > 999999999999999)
        {
            printf("Please enter a valid account number with at least 7 and at most 15 digits.\n");
        }
        else
        {
            validAccountNumber = true;
            while (getAccountFromFile(pf, userName, &cr))
            {
                if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
                {
                    printf("✖ This Account already exists for this user\n\n");
                    validAccountNumber = false;
                    break;
                }
            }
        }
    }

    // Updated Validation for country
    bool validCountry = false;
    while (!validCountry)
    {
        printf("\nEnter the country:");
        scanf("%s", r.country);
        if (strlen(r.country) < 2 || !isValidCountry(r.country))
        {
            printf("Country name should be at least 2 characters long and should not contain symbols.\n");
        }
        else
        {
            validCountry = true;
        }
    }

    // Updated Validation for phone number
    bool validPhoneNumber = false;
    while (!validPhoneNumber)
    {
        printf("\nEnter the phone number:");
        if (scanf("%ld", &r.phone) != 1)
        { // Scanf should return the number of successfully scanned items
            printf("Invalid input. Please enter a valid phone number.\n");
            while (getchar() != '\n')
                ; // Clear input buffer
            continue;
        }

        if (r.phone < 1000000 || r.phone > 999999999999999)
        {
            printf("Please enter a valid phone number with at least 7 and at most 15 digits.\n");
        }
        else
        {
            validPhoneNumber = true;
        }
    }

    bool validDeposit = false;
    while (!validDeposit)
    {
        printf("\nEnter amount to deposit: $");
        scanf("%lf", &r.amount);
        if (r.amount <= 0)
        {
            printf("Deposit amount cannot be negative or zero. Please enter a valid amount.\n");
        }
        else if (r.amount > 9999999999999999.99)
        { // Adjusting according to the desired limit
            printf("Deposit amount is too large.\n");
        }
        else
        {
            validDeposit = true;
        }
    }

    // Validation for account type
    bool validAccountType = false;
    while (!validAccountType)
    {
        printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
        scanf("%s", r.accountType);
        if (strcmp(r.accountType, "saving") == 0 || strcmp(r.accountType, "current") == 0 ||
            strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 ||
            strcmp(r.accountType, "fixed03") == 0)
        {
            validAccountType = true;
        }
        else
        {
            printf("Invalid account type. Please choose a valid account type.\n");
        }
    }

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%ld \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

// My code

void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

void registerUser()
{
    struct User newUser;
    FILE *fp = fopen("./data/users.txt", "a+");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    char lowerCaseNewUser[50];
    char lowerCaseExistingUser[50];

    // Loop for username
    bool validUsername = false;
    while (!validUsername)
    {
        printf("Enter username: ");
        scanf("%s", newUser.name);
        strcpy(lowerCaseNewUser, newUser.name);
        toLowerCase(lowerCaseNewUser);

        if (strlen(lowerCaseNewUser) < 4)
        {
            printf("Username should be at least 4 characters long.\n");
            continue;
        }

        bool usernameExists = false;
        rewind(fp);
        struct User user;
        while (fscanf(fp, "%d %s %s", &user.id, user.name, user.password) != EOF)
        {
            strcpy(lowerCaseExistingUser, user.name);
            toLowerCase(lowerCaseExistingUser);

            if (strcmp(lowerCaseNewUser, lowerCaseExistingUser) == 0)
            {
                usernameExists = true;
                break;
            }
        }

        if (usernameExists)
        {
            printf("This username already exists. Choose another one.\n");
        }
        else
        {
            validUsername = true; // exit the loop if the username is valid
        }
    }

    // Loop for password
    bool validPassword = false;
    while (!validPassword)
    {
        printf("Enter password: ");
        scanf("%s", newUser.password);

        if (strlen(newUser.password) < 6)
        {
            printf("Password should be at least 6 characters long.\n");
        }
        else if (strcmp(newUser.name, newUser.password) == 0)
        {
            printf("Password should not be the same as the username.\n");
        }
        else
        {
            validPassword = true; // exit the loop if the password is valid
        }
    }

    newUser.id = getNewUserId();
    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(fp);
    printf("User registered successfully!\n");
    sleep(3);
}

int getNewUserId()
{
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file");
        return 1; // Return 1 if the file doesn't exist or can't be opened
    }

    int maxId = 0;
    struct User user;

    while (fscanf(fp, "%d %s %s", &user.id, user.name, user.password) != EOF)
    {
        if (user.id > maxId)
        {
            maxId = user.id;
        }
    }

    fclose(fp);
    return maxId + 1; // Return one more than the maximum ID found in the file
}

void updateAccountInfo(struct User u)
{
    char input[50];
    int accountId;

    FILE *fp;
    struct Record records[100]; // assuming a maximum of 100 records for simplicity
    char userName[50];
    struct Record record;
    int totalRecords = 0;
    int foundIndex = -1;

    while (foundIndex == -1)
    {
        printf("Enter the account ID you want to update or \\back to return: ");
        scanf("%s", input);

        if (strcmp(input, "\\back") == 0)
        {
            return;
        }

        accountId = atoi(input); // Convert string to int

        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        totalRecords = 0;
        while (getAccountFromFile(fp, userName, &record))
        {
            strcpy(record.name, userName); // Store the user's name in the record
            records[totalRecords] = record;

            // Convert both user names to lowercase for comparison.
            for (int i = 0; userName[i]; i++)
            {
                userName[i] = tolower(userName[i]);
            }
            char tempName[50];
            strcpy(tempName, u.name);
            for (int i = 0; tempName[i]; i++)
            {
                tempName[i] = tolower(tempName[i]);
            }

            if (record.accountNbr == accountId && strcmp(userName, tempName) == 0)
            {
                foundIndex = totalRecords;
            }
            totalRecords++;
        }
        fclose(fp);
    }

    printf("Select the field to update:\n");
    printf("1. Country\n");
    printf("2. Phone number\n");

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter new country: ");
        scanf("%s", records[foundIndex].country); // update the country in the record
    }
    else if (choice == 2)
    {
        printf("Enter new phone number: ");
        scanf("%ld", &records[foundIndex].phone); // update the phone number in the record
    }
    else
    {
        printf("Invalid choice!\n");
        return;
    }

    // Write the entire array back to the file
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, records[i].name);
        tempUser.id = records[i].userId; // Assuming you have a userId field
        // ... copy other necessary User fields ...

        saveAccountToFile(fp, tempUser, records[i]);
    }
    fclose(fp);

    printf("Account information updated successfully!\n");
}

void checkAccountDetails(struct User u)
{
    int accountId;
    printf("Enter the account ID you want to check: ");
    scanf("%d", &accountId);

    FILE *fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    char userName[50];
    struct Record record;
    int found = 0;

    while (getAccountFromFile(fp, userName, &record))
    {
        if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
        {
            found = 1;
            break;
        }
        printf("Debug: Read accountNbr=%d for user %s\n", record.accountNbr, userName);
    }
    fclose(fp);

    if (!found)
    {
        printf("Account not found!\n");
        return;
    }

    // Print account details
    printf("Account ID: %d\n", record.accountNbr);
    printf("Country: %s\n", record.country);
    printf("Phone number: %ld\n", record.phone);
    printf("Amount deposited: $%.2lf\n", record.amount);
    printf("Account type: %s\n", record.accountType);

    // Calculate interest
    double interestRate;
    if (strcmp(record.accountType, "savings") == 0)
    {
        interestRate = 0.07;
    }
    else if (strcmp(record.accountType, "fixed01") == 0)
    {
        interestRate = 0.04;
    }
    else if (strcmp(record.accountType, "fixed02") == 0)
    {
        interestRate = 0.05;
    }
    else if (strcmp(record.accountType, "fixed03") == 0)
    {
        interestRate = 0.08;
    }
    else
    {
        printf("You will not get interests because the account is of type current\n");
        return;
    }

    double monthlyInterest = record.amount * interestRate / 12;
    printf("You will get $%.2lf as interest on day %d of every month.\n", monthlyInterest, record.deposit.day);
}

void makeTransaction(struct User u)
{
    int accountId;
    char input[50]; // Buffer for account ID input
    FILE *fp;
    struct Record records[100]; // assuming a maximum of 100 records for simplicity
    char names[100][50];        // Array to store the names associated with each record
    int totalRecords = 0;
    char userName[50];
    struct Record record;
    int index = -1;

    while (1) // Loop for account ID input
    {
        printf("Enter the account ID for the transaction or \\back to return: ");
        scanf("%s", input);

        if (strcmp(input, "\\back") == 0)
        {
            return; // Exit the function if user types \back
        }

        accountId = atoi(input); // Convert input to an integer

        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        totalRecords = 0;
        index = -1;
        while (getAccountFromFile(fp, userName, &record))
        {
            strcpy(names[totalRecords], userName); // Store the user's name in the names array
            records[totalRecords] = record;
            if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
            {
                index = totalRecords;
            }
            totalRecords++;
        }
        fclose(fp);

        if (index != -1)
        {
            break; // Break the loop if the correct account ID is found
        }
        else
        {
            printf("Account not found. Please try again.\n");
        }
    }

    // Check if the account is a fixed type and transactions are not allowed
    if (strcmp(records[index].accountType, "fixed01") == 0 ||
        strcmp(records[index].accountType, "fixed02") == 0 ||
        strcmp(records[index].accountType, "fixed03") == 0)
    {
        printf("Transactions are not allowed for fixed accounts.\n");
        return;
    }

    // Process transaction
    printf("Select transaction type:\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");

    int choice;
    scanf("%d", &choice);

    if (choice != 1 && choice != 2)
    {
        printf("Invalid choice!\n");
        return;
    }

    double amount;
    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (amount <= 0)
    {
        printf("Enter a valid amount.\n");
        return;
    }

    if (choice == 2 && records[index].amount < amount)
    {
        printf("Insufficient balance!\n");
        return;
    }

    // Perform the transaction
    if (choice == 1)
    {
        records[index].amount += amount;
    }
    else if (choice == 2)
    {
        records[index].amount -= amount;
    }

    // Update the specific record in the file
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, names[i]);
        tempUser.id = records[i].userId;

        saveAccountToFile(fp, tempUser, records[i]);
    }
    fclose(fp);

    printf("Transaction successful! New balance: $%.2lf\n", records[index].amount);
}

void removeAccount(struct User u)
{
    int accountId;
    printf("Enter the account ID you want to remove: ");
    scanf("%d", &accountId);

    FILE *fp = fopen(RECORDS, "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    struct Record records[100]; // assuming a maximum of 100 records for simplicity
    char names[100][50];        // Array to store the names associated with each record
    int totalRecords = 0;
    char userName[50];
    struct Record record;
    int found = 0;

    while (getAccountFromFile(fp, userName, &record))
    {
        if (record.accountNbr == accountId && strcmp(userName, u.name) == 0)
        {
            found = 1; // Account to be removed is found
            continue;  // Skip adding this record to the array
        }
        strcpy(names[totalRecords], userName); // Store the user's name in the names array
        records[totalRecords++] = record;
    }
    fclose(fp);

    if (!found)
    {
        printf("Account not found!\n");
        return;
    }

    // Adjust the record numbers to be sequential
    for (int i = 0; i < totalRecords; i++)
    {
        records[i].id = i + 1;
    }

    // Rewrite the records.txt file without the deleted account
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, names[i]);
        tempUser.id = records[i].userId;

        saveAccountToFile(fp, tempUser, records[i]);
    }
    fclose(fp);

    printf("Account removed successfully!\n");
}

void transferOwnership(struct User u)
{
    int accountId;
    char input[50]; // Buffer for account ID input
    char newOwnerName[50];
    struct Record records[100]; // assuming a maximum of 100 records for simplicity
    char names[100][50];        // Array to store the names associated with each record
    int totalRecords = 0;
    FILE *fp;
    int found = 0;

    while (1) // Infinite loop for account ID input
    {
        printf("Enter the account ID you want to transfer or \\back to return: ");
        scanf("%s", input);

        if (strcmp(input, "\\back") == 0)
        {
            return; // Exit the function if user types \back
        }

        accountId = atoi(input); // Convert input to an integer

        fp = fopen(RECORDS, "r");
        if (fp == NULL)
        {
            printf("Error opening file!\n");
            return;
        }

        struct Record record;
        totalRecords = 0;
        found = 0;
        while (getAccountFromFile(fp, names[totalRecords], &record))
        {
            records[totalRecords] = record;
            if (record.accountNbr == accountId && strcmp(names[totalRecords], u.name) == 0)
            {
                found = 1; // Account ID found and belongs to the user
            }
            totalRecords++;
        }
        fclose(fp);

        if (found)
        {
            break; // Break the loop if the correct account ID is found
        }
        else
        {
            printf("Account not found or does not belong to you. Please try again.\n");
        }
    }

    while (1) // Infinite loop for new owner's username input
    {
        printf("Enter the username of the new owner: ");
        scanf("%s", newOwnerName);

        // Prevent transferring to oneself
        if (strcmp(newOwnerName, u.name) == 0)
        {
            printf("You cannot transfer the account to yourself. Please enter a different username.\n");
            continue; // Skip the rest of the loop and prompt again
        }

        struct User newOwner;
        FILE *userFile = fopen("./data/users.txt", "r");
        if (userFile == NULL)
        {
            printf("Error opening user file!\n");
            return;
        }

        found = 0;
        while (fscanf(userFile, "%d %s %s", &newOwner.id, newOwner.name, newOwner.password) != EOF)
        {
            if (strcmp(newOwner.name, newOwnerName) == 0)
            {
                found = 1; // New owner found
                break;
            }
        }
        fclose(userFile);

        if (found)
        {
            for (int i = 0; i < totalRecords; i++)
            {
                if (records[i].accountNbr == accountId)
                {
                    strcpy(names[i], newOwner.name); // Update the owner name in the names array
                    records[i].userId = newOwner.id; // Update the owner ID in the record
                    break;                           // Break the loop once the record is updated
                }
            }
            break; // Break the loop if the new owner is found
        }
        else
        {
            printf("New owner username not found. Please try again.\n");
        }
    }

    // Write the updated records back to the file
    fp = fopen(RECORDS, "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < totalRecords; i++)
    {
        struct User tempUser; // Temporary User to pass the correct name to saveAccountToFile
        strcpy(tempUser.name, names[i]);
        tempUser.id = records[i].userId;

        saveAccountToFile(fp, tempUser, records[i]);
    }
    fclose(fp);

    printf("Ownership transferred successfully!\n");
}

// This function is used to flush the input buffer
void flushInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Keep reading characters until a newline or end-of-file
}
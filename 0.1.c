#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100

struct clientData
{
    unsigned int acctNum;
    char lastName[20];
    char firstName[20];
    double balance;
};

unsigned int enterChoice(void);
void createTextFile(FILE *fPtr);
void updateRecord(FILE *fPtr);
void addRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void viewRecords(FILE *fPtr);
void searchRecord(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    cfPtr = fopen("credit.dat", "rb+");

    if (cfPtr == NULL)
    {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be created.\n");
            return 1;
        }

        struct clientData blankClient = {0, "", "", 0.0};

        for (int i = 0; i < SIZE; i++)
        {
            fwrite(&blankClient,
                   sizeof(struct clientData),
                   1,
                   cfPtr);
        }

        rewind(cfPtr);
    }

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            addRecord(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            deleteRecord(cfPtr);
            break;

        case 4:
            viewRecords(cfPtr);
            break;

        case 5:
            searchRecord(cfPtr);
            break;

        case 6:
            createTextFile(cfPtr);
            break;

        default:
            printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    printf("\nProgram closed successfully.\n");

    return 0;
}

unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n====================================\n");
    printf("        BANK MANAGEMENT SYSTEM\n");
    printf("====================================\n");

    printf("1. Add New Account\n");
    printf("2. Update Account\n");
    printf("3. Delete Account\n");
    printf("4. View All Accounts\n");
    printf("5. Search Account\n");
    printf("6. Export Accounts to Text File\n");
    printf("7. Exit\n");

    printf("------------------------------------\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}

void addRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("\nEnter Account Number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
    }
    else
    {
        printf("Enter First Name: ");
        scanf("%19s", client.firstName);

        printf("Enter Last Name: ");
        scanf("%19s", client.lastName);

        printf("Enter Balance: ");
        scanf("%lf", &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount created successfully.\n");
    }
}

void updateRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;
    double amount;

    printf("\nEnter Account Number: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nCurrent Details\n");
        printf("---------------------------------\n");

        printf("Account No : %u\n", client.acctNum);
        printf("Name       : %s %s\n",
               client.firstName,
               client.lastName);

        printf("Balance    : %.2f\n",
               client.balance);

        printf("---------------------------------\n");

        printf("Enter Amount (+Deposit / -Withdraw): ");
        scanf("%lf", &amount);

        client.balance += amount;

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("\nAccount updated successfully.\n");
        printf("New Balance: %.2f\n", client.balance);
    }
}

void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("\nEnter Account Number to Delete: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

void viewRecords(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n=====================================================\n");
    printf("%-10s %-15s %-15s %-10s\n",
           "Acc No",
           "First Name",
           "Last Name",
           "Balance");

    printf("=====================================================\n");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u %-15s %-15s %-10.2f\n",
                   client.acctNum,
                   client.firstName,
                   client.lastName,
                   client.balance);
        }
    }
}

void searchRecord(FILE *fPtr)
{
    struct clientData client;

    unsigned int accountNum;

    printf("\nEnter Account Number to Search: ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\n=========== ACCOUNT DETAILS ===========\n");

        printf("Account Number : %u\n",
               client.acctNum);

        printf("First Name     : %s\n",
               client.firstName);

        printf("Last Name      : %s\n",
               client.lastName);

        printf("Balance        : %.2f\n",
               client.balance);

        printf("=======================================\n");
    }
}

void createTextFile(FILE *fPtr)
{
    FILE *writePtr;

    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("Cannot create text file.\n");
        return;
    }

    rewind(fPtr);

    fprintf(writePtr,
            "%-10s %-15s %-15s %-10s\n",
            "Acc No",
            "First Name",
            "Last Name",
            "Balance");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-10u %-15s %-15s %-10.2f\n",
                    client.acctNum,
                    client.firstName,
                    client.lastName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("\naccounts.txt created successfully.\n");
}
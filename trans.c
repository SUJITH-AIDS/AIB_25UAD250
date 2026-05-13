#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 50
#define DATA_FILE "bank.dat"

struct customerData {
    unsigned int accNum;
    char name[20];
    char city[20];
    double balance;
};

// Function Prototypes
void initializeFile(FILE *fPtr);
void addAccount(FILE *fPtr);
void updateAccount(FILE *fPtr);
void deleteAccount(FILE *fPtr);
void displayAccounts(FILE *fPtr);
void searchAccount(FILE *fPtr);

int main() {
    FILE *cfPtr;
    int choice;

    // Open file
    if ((cfPtr = fopen(DATA_FILE, "rb+")) == NULL) {
        printf("Creating new file...\n");

        cfPtr = fopen(DATA_FILE, "wb+");

        if (cfPtr == NULL) {
            printf("File could not be created.\n");
            return 1;
        }

        initializeFile(cfPtr);
    }

    while (1) {

        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Add New Account\n");
        printf("2. Update Account Balance\n");
        printf("3. Delete Account\n");
        printf("4. Display All Accounts\n");
        printf("5. Search Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addAccount(cfPtr);
                break;

            case 2:
                updateAccount(cfPtr);
                break;

            case 3:
                deleteAccount(cfPtr);
                break;

            case 4:
                displayAccounts(cfPtr);
                break;

            case 5:
                searchAccount(cfPtr);
                break;

            case 6:
                fclose(cfPtr);
                printf("Program Ended.\n");
                exit(0);

            default:
                printf("Invalid Choice.\n");
        }
    }

    return 0;
}

// Initialize file with empty records
void initializeFile(FILE *fPtr) {

    struct customerData blank = {0, "", "", 0.0};

    for (int i = 0; i < MAX_RECORDS; i++) {
        fwrite(&blank, sizeof(struct customerData), 1, fPtr);
    }
}

// Add new account
void addAccount(FILE *fPtr) {

    struct customerData customer = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter Account Number (1-%d): ", MAX_RECORDS);
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX_RECORDS) {
        printf("Invalid Account Number.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct customerData), SEEK_SET);
    fread(&customer, sizeof(struct customerData), 1, fPtr);

    if (customer.accNum != 0) {
        printf("Account already exists.\n");
    }
    else {

        customer.accNum = acc;

        printf("Enter Customer Name: ");
        scanf("%19s", customer.name);

        printf("Enter City: ");
        scanf("%19s", customer.city);

        printf("Enter Balance: ");
        scanf("%lf", &customer.balance);

        fseek(fPtr, -(long)sizeof(struct customerData), SEEK_CUR);

        fwrite(&customer, sizeof(struct customerData), 1, fPtr);

        printf("Account Created Successfully.\n");
    }
}

// Update account balance
void updateAccount(FILE *fPtr) {

    struct customerData customer;
    unsigned int acc;
    double amount;

    printf("Enter Account Number: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX_RECORDS) {
        printf("Invalid Account Number.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct customerData), SEEK_SET);
    fread(&customer, sizeof(struct customerData), 1, fPtr);

    if (customer.accNum == 0) {
        printf("Account does not exist.\n");
    }
    else {

        printf("Current Balance = %.2f\n", customer.balance);

        printf("Enter Amount (+Deposit / -Withdraw): ");
        scanf("%lf", &amount);

        customer.balance += amount;

        fseek(fPtr, (acc - 1) * sizeof(struct customerData), SEEK_SET);

        fwrite(&customer, sizeof(struct customerData), 1, fPtr);

        printf("Updated Balance = %.2f\n", customer.balance);
    }
}

// Delete account
void deleteAccount(FILE *fPtr) {

    struct customerData customer;
    struct customerData blank = {0, "", "", 0.0};

    unsigned int acc;

    printf("Enter Account Number to Delete: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX_RECORDS) {
        printf("Invalid Account Number.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct customerData), SEEK_SET);

    fread(&customer, sizeof(struct customerData), 1, fPtr);

    if (customer.accNum == 0) {
        printf("Account not found.\n");
    }
    else {

        fseek(fPtr, (acc - 1) * sizeof(struct customerData), SEEK_SET);

        fwrite(&blank, sizeof(struct customerData), 1, fPtr);

        printf("Account Deleted Successfully.\n");
    }
}

// Display all accounts
void displayAccounts(FILE *fPtr) {

    struct customerData customer;
    int found = 0;

    rewind(fPtr);

    printf("\n%-10s %-20s %-20s %-10s\n",
           "Acc No", "Name", "City", "Balance");

    printf("---------------------------------------------------------\n");

    while (fread(&customer, sizeof(struct customerData), 1, fPtr)) {

        if (customer.accNum != 0) {

            printf("%-10u %-20s %-20s %-10.2f\n",
                   customer.accNum,
                   customer.name,
                   customer.city,
                   customer.balance);

            found = 1;
        }
    }

    if (!found) {
        printf("No Records Found.\n");
    }
}

// Search account
void searchAccount(FILE *fPtr) {

    struct customerData customer;
    unsigned int acc;

    printf("Enter Account Number to Search: ");
    scanf("%u", &acc);

    if (acc < 1 || acc > MAX_RECORDS) {
        printf("Invalid Account Number.\n");
        return;
    }

    fseek(fPtr, (acc - 1) * sizeof(struct customerData), SEEK_SET);

    fread(&customer, sizeof(struct customerData), 1, fPtr);

    if (customer.accNum == 0) {

        printf("Account not found.\n");
    }
    else {

        printf("\nAccount Details\n");
        printf("----------------------\n");

        printf("Account Number : %u\n", customer.accNum);
        printf("Customer Name  : %s\n", customer.name);
        printf("City           : %s\n", customer.city);
        printf("Balance        : %.2f\n", customer.balance);
    }
}
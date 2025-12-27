#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Structure for bank account */
struct Account {
    int accNo;
    char name[50];
    float balance;
};

/* Function declarations */
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void displayAllAccounts();
void deleteAccount();

int main() {
    int choice;

    do {
        printf("\n========== BANK MANAGEMENT SYSTEM ==========\n");
        printf("1. Create Account\n");
        printf("2. Deposit Money\n");
        printf("3. Withdraw Money\n");
        printf("4. Check Balance\n");
        printf("5. Display All Accounts\n");
        printf("6. Delete Account\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: displayAllAccounts(); break;
            case 6: deleteAccount(); break;
            case 7: printf("Thank you for using the Bank System.\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

/* Function to create a new account */
void createAccount() {
    struct Account acc;
    FILE *fp = fopen("bank.dat", "ab+");
    if (!fp) { printf("Error opening file!\n"); return; }

    int accNo, exists = 0;
    printf("Enter Account Number: "); 
    scanf("%d", &accNo);

    // Check for duplicate account number
    struct Account temp;
    rewind(fp);
    while (fread(&temp, sizeof(temp), 1, fp)) {
        if (temp.accNo == accNo) { exists = 1; break; }
    }
    if (exists) { printf("Account number already exists! Try again.\n"); fclose(fp); return; }

    acc.accNo = accNo;
    printf("Enter Name: "); 
    scanf(" %[^\n]", acc.name);

    do {
        printf("Enter Initial Balance (>=0): "); 
        scanf("%f", &acc.balance);
        if (acc.balance < 0) printf("Balance cannot be negative!\n");
    } while (acc.balance < 0);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

/* Function to deposit money */
void depositMoney() {
    struct Account acc;
    FILE *fp = fopen("bank.dat", "rb+");
    if (!fp) { printf("File not found!\n"); return; }

    int accNo, found = 0;
    float amount;

    printf("Enter Account Number: "); 
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            do {
                printf("Enter amount to deposit (>=0): "); 
                scanf("%f", &amount);
                if (amount < 0) printf("Amount cannot be negative!\n");
            } while (amount < 0);

            acc.balance += amount;
            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            printf("Amount deposited successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found!\n");
    fclose(fp);
}

/* Function to withdraw money */
void withdrawMoney() {
    struct Account acc;
    FILE *fp = fopen("bank.dat", "rb+");
    if (!fp) { printf("File not found!\n"); return; }

    int accNo, found = 0;
    float amount;

    printf("Enter Account Number: "); 
    scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            do {
                printf("Enter amount to withdraw (>=0 and <= balance): "); 
                scanf("%f", &amount);
                if (amount < 0) printf("Amount cannot be negative!\n");
                else if (amount > acc.balance) printf("Insufficient balance!\n");
            } while (amount < 0 || amount > acc.balance);

            acc.balance -= amount;
            fseek(fp, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, fp);
            printf("Amount withdrawn successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) printf("Account not found!\n");
    fclose(fp);
}

/* Function to check balance */
void checkBalance() {
    struct Account acc;
    FILE *fp = fopen("bank.dat", "rb");
    if (!fp) { printf("File not found!\n"); return; }

    int accNo, found = 0;
    printf("Enter Account Number: "); scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) {
            printf("\nAccount Number: %d\nName: %s\nBalance: %.2f\n", 
                    acc.accNo, acc.name, acc.balance);
            found = 1; break;
        }
    }

    if (!found) printf("Account not found!\n");
    fclose(fp);
}

/* Function to display all accounts */
void displayAllAccounts() {
    struct Account acc;
    FILE *fp = fopen("bank.dat", "rb");
    if (!fp) { printf("File not found!\n"); return; }

    printf("\n------ ALL ACCOUNTS ------\n");
    while (fread(&acc, sizeof(acc), 1, fp)) {
        printf("\nAccount No: %d\nName: %s\nBalance: %.2f\n", 
                acc.accNo, acc.name, acc.balance);
    }
    fclose(fp);
}

/* Function to delete an account */
void deleteAccount() {
    struct Account acc;
    FILE *fp = fopen("bank.dat", "rb");
    if (!fp) { printf("File not found!\n"); return; }
    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) { printf("Error opening temp file!\n"); fclose(fp); return; }

    int accNo, found = 0;
    printf("Enter Account Number to delete: "); scanf("%d", &accNo);

    while (fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.accNo == accNo) { found = 1; continue; }
        fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp); fclose(temp);
    remove("bank.dat"); rename("temp.dat", "bank.dat");

    if (found) printf("Account deleted successfully!\n");
    else printf("Account not found!\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "main.h"
#include "account.h"
#include "banking.h"
#include "queue.h"

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void handleCreateAccountFromMenu() {
    int id; int cibil_assigned; double balance; char name[50]; char phone[15]; char address[100];
    printf("\n-- CREATE ACCOUNT (M2) --\n");
    printf("Enter New Account ID: "); 
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }
    
    cibil_assigned = (rand() % 251) + 600; 
    
    clear_input_buffer();
    printf("Enter Customer Name (no spaces): "); if (scanf("%s", name) != 1) return;
    printf("Enter Initial Balance: "); if (scanf("%lf", &balance) != 1) { printf("Invalid input.\n"); return; }

    clear_input_buffer();
    printf("Enter Phone Number: "); if (fgets(phone, sizeof(phone), stdin) == NULL) return; phone[strcspn(phone, "\n")] = 0;
    printf("Enter Address (no spaces): "); if (fgets(address, sizeof(address), stdin) == NULL) return; address[strcspn(address, "\n")] = 0;
    
    createAccount(id, name, balance, cibil_assigned, phone, address); 
}

void handleUpdateAccountDetails() {
    int id; char choice;
    printf("\n-- UPDATE ACCOUNT DETAILS --\n");
    printf("Enter Account ID to modify: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }
    AccountNode *accNode = findAccountNode(id);
    if (accNode == NULL) { printf("FAILURE: Account ID %d not found.\n", id); return; }
    printf("\nAccount found for %s. What would you like to update?\n", accNode->data.name);
    printf("A. Phone Number\nB. Address\nEnter choice (A/B): ");
    clear_input_buffer();
    if (scanf(" %c", &choice) != 1) { printf("Invalid choice.\n"); return; }
    clear_input_buffer();
    if (choice == 'A' || choice == 'a') {
        printf("Enter NEW Phone Number: ");
        if (fgets(accNode->data.phoneNumber, sizeof(accNode->data.phoneNumber), stdin) != NULL) {
            accNode->data.phoneNumber[strcspn(accNode->data.phoneNumber, "\n")] = 0;
            printf("SUCCESS: Phone number updated to %s.\n", accNode->data.phoneNumber);
        }
    } else if (choice == 'B' || choice == 'b') {
        printf("Enter NEW Address: ");
        if (fgets(accNode->data.address, sizeof(accNode->data.address), stdin) != NULL) {
            accNode->data.address[strcspn(accNode->data.address, "\n")] = 0;
            printf("SUCCESS: Address updated.\n");
        }
    } else { printf("Invalid update option.\n"); }
}

void handleDisplayHistory() {
    int id;
    printf("\n-- DISPLAY TRANSACTION HISTORY (M3) --\n");
    printf("Enter Account ID: "); if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }
    AccountNode *accNode = findAccountNode(id);
    if (accNode == NULL) { printf("FAILURE: Account ID %d not found.\n", id); return; }
    
    if (accNode->data.history == NULL || accNode->data.history->top == -1) { printf("No transaction history available.\n"); return; }
    
    printf("\n--- History for ID %d (Most Recent First) ---\n", id);
    printf("Type           | Amount       | New Balance\n");
    printf("---------------|--------------|--------------\n");
    
    for (int i = accNode->data.history->top; i >= 0; i--) {
        Transaction t = accNode->data.history->items[i];
        printf("%-14s | $%-11.2lf | $%-11.2lf\n", t.type, t.amount, t.newBalance);
    }
    printf("----------------------------------------\n");
}

void displayAllAccounts() {
    printf("\n--- CUSTOMER ACCOUNT DATABASE (M2) ---\n");
    AccountNode *current = head;
    if (current == NULL) {
        printf("The database is currently empty.\n"); return;
    }
    printf("ID     | Name                    | Balance    | CIBIL\n");
    printf("-------|-------------------------|------------|-------\n");
    while (current != NULL) {
        printf("%-6d | %-23s | $%-9.2lf | %d\n",
               current->data.accountID, 
               current->data.name, 
               current->data.balance,
               current->data.cibilScore);
        current = current->next;
    }
    printf("---------------------------------------\n");
}

void printMenu() {
    printf("\n================================================\n");
    printf("BANKING SYSTEM (Modules 1, 2, 3, 4, 6)\n");
    printf("================================================\n");
    printf("1. Create New Account (M2)\n");
    printf("2. Deposit (M1)\n");
    printf("3. Withdraw (M1)\n");
    printf("4. Fund Transfer (M1)\n");
    printf("5. View Transaction History (M3)\n");
    printf("6. Submit Service Request (M4)\n");
    printf("7. Process Next Service Request (M4)\n");
    printf("8. Check Loan Eligibility (M6)\n");
    printf("9. Update Account Details (M2 Enhancement)\n");
    printf("10. Display All Accounts (M2)\n");
    printf("0. Exit Program\n");
    printf("Enter your choice: ");
}

int main() {
    int choice;
    srand(time(NULL)); 
    
    printf("System Startup: Database is empty. Use Option 1 to create an account.\n");
    
    do {
        clear_input_buffer(); 
        printMenu();
        if (scanf("%d", &choice) != 1) { choice = -1; }

        switch (choice) {
            case 1: handleCreateAccountFromMenu(); break;
            case 2: handleDeposit(); break;
            case 3: handleWithdrawal(); break;
            case 4: handleTransfer(); break;
            case 5: handleDisplayHistory(); break;
            case 6: handleNewServiceRequest(); break;
            case 7: dequeueServiceRequest(); break;
            case 8: checkLoanEligibility(); break;
            case 9: handleUpdateAccountDetails(); break;
            case 10: displayAllAccounts(); break;
            case 0: printf("\nExiting System. Cleaning up memory...\n"); break;
            default: printf("Invalid choice. Please enter a valid number.\n");
        }
    } while (choice != 0);

    AccountNode *current = head;
    AccountNode *next;
    while (current != NULL) {
        next = current->next;
        if (current->data.history != NULL) free(current->data.history);
        free(current);
        current = next;
    }
    return 0;
}

#include "banking.h"
#include "account.h"
#include "priority_queue.h" 
#include <stdio.h>
#include <string.h>

void checkLoanEligibility() {
    int id;
    double requiredMinBalance = 5000.00; 
    int requiredMinCibil = 700;           
    
    printf("\n-- LOAN ELIGIBILITY CHECK --\n");
    printf("Enter Account ID: ");
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }

    AccountNode *accNode = findAccountNode(id); 
    
    if (accNode == NULL) { printf("FAILURE: Account ID %d not found.\n", id); return; }

    int cibil = accNode->data.cibilScore;
    double balance = accNode->data.balance;
    
    printf("\n--- Eligibility Report for ID %d ---\n", id);
    printf("CIBIL Score: %d (Required: %d)\n", cibil, requiredMinCibil);
    printf("Balance: $%.2lf (Required: $%.2lf)\n", balance, requiredMinBalance);

    if (cibil >= requiredMinCibil && balance >= requiredMinBalance) {
        
        // --- LOAN PRIORITY CALCULATION (M6) ---
        // Priority Score = CIBIL Score + (Balance / 100)
        double priorityScore = (double)cibil + (balance / 100.0);

        printf("STATUS: ELIGIBLE! Priority Score calculated: %.2lf\n", priorityScore);
        enqueueApplicant(id, accNode->data.name, priorityScore);
        
    } else {
        printf("STATUS: INELIGIBLE. Criteria not met.\n");
    }
}

/**
 * @brief Processes the next loan application using the Max Heap.
 * (M6: Loan Applicant Ranking)
 */
void processNextLoanApplication() {
    dequeueHighestPriority();
}


void handleDeposit() {
    int id; double amount;
    printf("\n-- DEPOSIT --\n");
    printf("Enter Account ID: "); if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }
    AccountNode *accNode = findAccountNode(id); 
    if (accNode == NULL) { printf("FAILURE: ID %d not found.\n", id); return; }
    printf("Enter Deposit Amount: "); if (scanf("%lf", &amount) != 1) { printf("Invalid input.\n"); return; }
    if (amount <= 0) { printf("FAILURE: Must be positive.\n"); return; }
    
    accNode->data.balance += amount; 
    accNode->data.totalTransactions++;
    updateCIBIL(accNode, "Deposit", amount); 
    pushTransaction(accNode->data.history, "Deposit", amount, accNode->data.balance);
    printf("SUCCESS: New Balance: $%.2lf\n", accNode->data.balance);
}

void handleWithdrawal() {
    int id; double amount;
    printf("\n-- WITHDRAWAL --\n");
    printf("Enter Account ID: "); if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }
    AccountNode *accNode = findAccountNode(id); 
    if (accNode == NULL) { printf("FAILURE: ID %d not found.\n", id); return; }
    printf("Enter Withdrawal Amount: "); if (scanf("%lf", &amount) != 1) { printf("Invalid input.\n"); return; }
    if (amount <= 0) { printf("FAILURE: Must be positive.\n"); return; }
    if (amount > accNode->data.balance) { printf("FAILURE: Insufficient funds ($%.2lf).\n", accNode->data.balance); return; }
    
    accNode->data.balance -= amount; 
    accNode->data.totalTransactions++;
    updateCIBIL(accNode, "Withdrawal", amount); 
    pushTransaction(accNode->data.history, "Withdrawal", amount, accNode->data.balance);
    printf("SUCCESS: New Balance: $%.2lf\n", accNode->data.balance);
}

void handleTransfer() {
    int sourceID, destID; double amount;
    printf("\n-- FUND TRANSFER --\n");
    printf("Enter Source Account ID: "); if (scanf("%d", &sourceID) != 1) { printf("Invalid input.\n"); return; }
    printf("Enter Destination Account ID: "); if (scanf("%d", &destID) != 1) { printf("Invalid input.\n"); return; }
    if (sourceID == destID) { printf("FAILURE: Cannot transfer to the same account.\n"); return; }
    AccountNode *sourceAccNode = findAccountNode(sourceID);
    AccountNode *destAccNode = findAccountNode(destID);
    if (sourceAccNode == NULL || destAccNode == NULL) { printf("FAILURE: Accounts not found.\n"); return; }
    printf("Enter Transfer Amount: "); if (scanf("%lf", &amount) != 1) { printf("Invalid input.\n"); return; }
    if (amount <= 0) { printf("FAILURE: Must be positive.\n"); return; }
    if (amount > sourceAccNode->data.balance) { printf("FAILURE: Insufficient funds ($%.2lf).\n", sourceAccNode->data.balance); return; }
    
    sourceAccNode->data.balance -= amount;
    destAccNode->data.balance += amount;
    
    sourceAccNode->data.totalTransactions++;
    destAccNode->data.totalTransactions++;
    updateCIBIL(sourceAccNode, "Transfer_OUT", amount); 
    updateCIBIL(destAccNode, "Transfer_IN", amount); 
    
    pushTransaction(sourceAccNode->data.history, "Transfer_OUT", amount, sourceAccNode->data.balance);
    pushTransaction(destAccNode->data.history, "Transfer_IN", amount, destAccNode->data.balance);
    printf("SUCCESS: Transfer complete. Source New Bal: $%.2lf, Dest New Bal: $%.2lf\n", sourceAccNode->data.balance, destAccNode->data.balance);
}

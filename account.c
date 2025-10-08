#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

AccountNode *head = NULL;

TransactionStack* initTransactionStack() {
    TransactionStack *stack = (TransactionStack*)malloc(sizeof(TransactionStack));
    stack->top = -1;
    return stack;
}

void pushTransaction(TransactionStack *stack, const char *type, double amount, double newBalance) {
    if (stack == NULL) return;
    if (stack->top == MAX_HISTORY - 1) {
        for (int i = 0; i < MAX_HISTORY - 1; i++) { stack->items[i] = stack->items[i+1]; }
        stack->top = MAX_HISTORY - 2; 
    }
    stack->top++;
    strncpy(stack->items[stack->top].type, type, sizeof(stack->items[stack->top].type) - 1);
    stack->items[stack->top].type[sizeof(stack->items[stack->top].type) - 1] = '\0';
    stack->items[stack->top].amount = amount;
    stack->items[stack->top].newBalance = newBalance;
}

void updateCIBIL(AccountNode *accNode, const char *type, double amount) {
    int impact = 0;
    
    if (strcmp(type, "Withdrawal") == 0 || strcmp(type, "Transfer_OUT") == 0) {
        if (amount > accNode->data.balance * 0.5) { impact -= 5; } else { impact += 1; }
    } else if (strcmp(type, "Deposit") == 0 || strcmp(type, "Transfer_IN") == 0) {
        if (amount > 0) { impact += 3; }
    }

    double utilization = accNode->data.balance / accNode->data.availableCredit;
    if (utilization > 0.8) { impact -= 7; } else if (utilization < 0.3) { impact += 4; }

    accNode->data.cibilScore += impact;
    if (accNode->data.cibilScore > 900) { accNode->data.cibilScore = 900; } 
    else if (accNode->data.cibilScore < 300) { accNode->data.cibilScore = 300; }
    
    printf("--- CIBIL Status: %s. New CIBIL: %d (Impact: %d) ---\n", type, accNode->data.cibilScore, impact);
}

AccountNode* findAccountNode(int id) {
    AccountNode *current = head;
    while (current != NULL) {
        if (current->data.accountID == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool createAccount(int id, const char *name, double initialBalance, int cibil, const char *phone, const char *address) {
    if (findAccountNode(id) != NULL) { printf("FAILURE: Account with ID %d already exists.\n", id); return false; }
    
    AccountNode *newNode = (AccountNode *)malloc(sizeof(AccountNode));
    if (newNode == NULL) { perror("Memory allocation failed"); return false; }

    newNode->data.accountID = id;
    strncpy(newNode->data.name, name, sizeof(newNode->data.name) - 1);
    newNode->data.name[sizeof(newNode->data.name) - 1] = '\0';
    newNode->data.balance = initialBalance;
    newNode->data.cibilScore = cibil; 
    
    strncpy(newNode->data.phoneNumber, phone, sizeof(newNode->data.phoneNumber) - 1);
    strncpy(newNode->data.address, address, sizeof(newNode->data.address) - 1);
    
    newNode->data.totalTransactions = 0;
    newNode->data.availableCredit = 10000.00; 
    
    newNode->data.history = initTransactionStack();
    newNode->next = head;
    head = newNode;
    
    printf("SUCCESS: Account created for %s (ID: %d). Balance: $%.2lf. **CIBIL: %d**\n", name, id, initialBalance, cibil);
    return true;
}

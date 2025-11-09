#include "account.h"
#include "bst.h" // Include BST header for tree operations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Note: AccountNode *head = NULL; is removed.
// The global BST root is defined and managed in bst.c.

TransactionStack *initTransactionStack() {
    TransactionStack *stack = (TransactionStack *)malloc(sizeof(TransactionStack));
    stack->top = -1;
    return stack;
}

void pushTransaction(TransactionStack *stack, const char *type, double amount, double newBalance) {
    if (stack == NULL)
        return;
    if (stack->top == MAX_HISTORY - 1) {
        // Shift elements down to implement fixed-size history (sliding window)
        for (int i = 0; i < MAX_HISTORY - 1; i++) {
            stack->items[i] = stack->items[i + 1];
        }
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
        if (accNode->data.balance <= 0) { // Check for near zero balance on withdrawal
            impact -= 15;
        } else if (amount > accNode->data.balance * 0.5) {
            impact -= 5;
        } else {
            impact += 1;
        }
    } else if (strcmp(type, "Deposit") == 0 || strcmp(type, "Transfer_IN") == 0) {
        if (amount > 0) {
            impact += 3;
        }
    }

    // Recalculate utilization *after* the balance change
    double utilization = accNode->data.balance / accNode->data.availableCredit;
    if (utilization > 0.9) {
        impact -= 7;
    } else if (utilization < 0.2) {
        impact += 4;
    }

    accNode->data.cibilScore += impact;
    if (accNode->data.cibilScore > 900) {
        accNode->data.cibilScore = 900;
    } else if (accNode->data.cibilScore < 300) {
        accNode->data.cibilScore = 300;
    }

    printf("--- CIBIL Status: %s. New CIBIL: %d (Impact: %d) ---\n", type, accNode->data.cibilScore, impact);
}

/**
 * @brief Finds an account node using the BST search function.
 * @param id The account ID to find.
 * @return AccountNode* The found node or NULL.
 */
AccountNode* findAccountNode(int id) {
    // Access the global root defined in bst.c
    extern AccountNode *root; 
    // Uses the BST search, which is much faster (O(log n))
    return searchAccountBST(root, id);
}

/**
 * @brief Creates a new account, allocates a node, and inserts it into the BST.
 * @return bool True on success, false on failure.
 */
bool createAccount(int id, const char *name, double initialBalance, int cibil, const char *phone, const char *address) {
    // Access the global root defined in bst.c
    extern AccountNode *root; 
    
    if (findAccountNode(id) != NULL) {
        printf("FAILURE: Account with ID %d already exists.\n", id);
        return false;
    }

    AccountNode *newNode = (AccountNode *)malloc(sizeof(AccountNode));
    if (newNode == NULL) {
        perror("Memory allocation failed");
        return false;
    }

    // Initialize all AccountData fields
    newNode->data.accountID = id;
    strncpy(newNode->data.name, name, sizeof(newNode->data.name) - 1);
    newNode->data.name[sizeof(newNode->data.name) - 1] = '\0';
    newNode->data.balance = initialBalance;
    newNode->data.cibilScore = cibil;

    strncpy(newNode->data.phoneNumber, phone, sizeof(newNode->data.phoneNumber) - 1);
    strncpy(newNode->data.address, address, sizeof(newNode->data.address) - 1);
    
    newNode->data.phoneNumber[sizeof(newNode->data.phoneNumber) - 1] = '\0';
    newNode->data.address[sizeof(newNode->data.address) - 1] = '\0';

    newNode->data.totalTransactions = 0;
    newNode->data.availableCredit = 10000.00; // Default credit

    newNode->data.history = initTransactionStack();
    
    // Initialize BST pointers (left/right are used instead of next)
    newNode->left = NULL;
    newNode->right = NULL;

    // Insert into the BST
    root = insertAccountBST(root, newNode);

    printf("SUCCESS: Account created for %s (ID: %d). Balance: $%.2lf. **CIBIL: %d**\n", name, id, initialBalance, cibil);
    return true;
}

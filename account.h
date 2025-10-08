#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>

#define MAX_HISTORY 5

typedef struct Transaction { 
    char type[15]; 
    double amount; 
    double newBalance; 
} Transaction;

typedef struct TransactionStack { 
    Transaction items[MAX_HISTORY]; 
    int top; 
} TransactionStack;

typedef struct AccountData {
    int accountID; 
    char name[50]; 
    double balance; 
    int cibilScore;
    char phoneNumber[15]; 
    char address[100];
    TransactionStack *history; 
    
    int totalTransactions; 
    double availableCredit; 
} AccountData;

typedef struct AccountNode {
    AccountData data;
    struct AccountNode *next;
} AccountNode;

extern AccountNode *head;

AccountNode* findAccountNode(int id);
bool createAccount(int id, const char *name, double initialBalance, int cibil, const char *phone, const char *address);

TransactionStack* initTransactionStack();
void pushTransaction(TransactionStack *stack, const char *type, double amount, double newBalance);

void updateCIBIL(AccountNode *accNode, const char *type, double amount);

#endif

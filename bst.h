#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include "account.h"

// External declaration for the BST root
extern AccountNode *root;

// Function to initialize the BST root (set to NULL)
void initBST();

// Function to insert a new account node into the BST
AccountNode* insertAccountBST(AccountNode* node, AccountNode *newNode);

// Function to search for an account node in the BST
AccountNode* searchAccountBST(AccountNode* node, int id);

// Utility function to traverse the BST and perform an action (e.g., saving data)
void inorderTraversal(AccountNode* node, void (*process)(AccountNode*));

// Utility function to clean up the memory used by the BST
void freeBST(AccountNode* node);

#endif

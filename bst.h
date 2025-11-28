#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include "account.h"


extern AccountNode *root;

void initBST();

AccountNode* insertAccountBST(AccountNode* node, AccountNode *newNode);

AccountNode* searchAccountBST(AccountNode* node, int id);

void inorderTraversal(AccountNode* node, void (*process)(AccountNode*));

void freeBST(AccountNode* node);

#endif

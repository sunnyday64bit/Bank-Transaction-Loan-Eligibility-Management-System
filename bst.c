#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

AccountNode *root = NULL;

void initBST() {
    root = NULL;
}

AccountNode* insertAccountBST(AccountNode* node, AccountNode *newNode) {
    if (node == NULL) {
       
        return newNode;
    }

    if (newNode->data.accountID < node->data.accountID) {
        node->left = insertAccountBST(node->left, newNode);
    } else if (newNode->data.accountID > node->data.accountID) {
        node->right = insertAccountBST(node->right, newNode);
    }

    return node;
}

AccountNode* searchAccountBST(AccountNode* node, int id) {
    if (node == NULL || node->data.accountID == id) {
        return node;
    }

    if (id < node->data.accountID) {
        return searchAccountBST(node->left, id);
    } else {
        return searchAccountBST(node->right, id);
    }
}

void inorderTraversal(AccountNode* node, void (*process)(AccountNode*)) {
    if (node != NULL) {
        inorderTraversal(node->left, process);
        process(node);
        inorderTraversal(node->right, process);
    }
}


void freeBST(AccountNode* node) {
    if (node != NULL) {
        freeBST(node->left);
        freeBST(node->right);
   
        if (node->data.history != NULL) {
            free(node->data.history);
        }
        free(node);
    }
}

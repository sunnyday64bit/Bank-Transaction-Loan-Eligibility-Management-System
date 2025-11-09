#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

// Global root pointer for the BST structure
AccountNode *root = NULL;

/**
 * @brief Initializes the BST by setting the global root to NULL.
 */
void initBST() {
    root = NULL;
}

/**
 * @brief Inserts a new account node into the Binary Search Tree.
 * The BST is ordered by accountID.
 *
 * @param node The current node being examined (starts with root).
 * @param newNode The new node to insert.
 * @return AccountNode* The updated root or sub-root of the tree.
 */
AccountNode* insertAccountBST(AccountNode* node, AccountNode *newNode) {
    if (node == NULL) {
        // This is the correct insertion point
        return newNode;
    }

    if (newNode->data.accountID < node->data.accountID) {
        node->left = insertAccountBST(node->left, newNode);
    } else if (newNode->data.accountID > node->data.accountID) {
        node->right = insertAccountBST(node->right, newNode);
    }
    // If IDs are equal, we do nothing (should be handled before calling insert)

    return node;
}

/**
 * @brief Searches for an account node in the BST based on accountID.
 *
 * @param node The current node being examined (starts with root).
 * @param id The account ID to search for.
 * @return AccountNode* The node with the matching ID, or NULL if not found.
 */
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

/**
 * @brief Performs an in-order traversal of the BST, executing a function
 * for each node. Useful for printing or saving data in sorted order.
 *
 * @param node The current node.
 * @param process Function pointer to the action to perform on each node.
 */
void inorderTraversal(AccountNode* node, void (*process)(AccountNode*)) {
    if (node != NULL) {
        inorderTraversal(node->left, process);
        process(node);
        inorderTraversal(node->right, process);
    }
}

/**
 * @brief Recursively frees the memory of all nodes in the BST.
 *
 * @param node The current node.
 */
void freeBST(AccountNode* node) {
    if (node != NULL) {
        freeBST(node->left);
        freeBST(node->right);
        // Free the transaction history stack
        if (node->data.history != NULL) {
            free(node->data.history);
        }
        free(node);
    }
}

#include "file_handler.h"
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Forward declaration for the utility function used in inorderTraversal
static void writeNodeToFile(AccountNode* node);

/**
 * @brief Utility function to write a single AccountNode's data to the global file pointer.
 * This function is designed to be passed to inorderTraversal.
 * * NOTE: File pointer management is handled in the caller function (saveAllAccounts).
 * We write the AccountData structure, which includes the fixed-size TransactionStack,
 * but NOT the dynamic history pointer itself.
 * * @param node The account node to write.
 */
static FILE* dataFilePtr = NULL; // Internal pointer for file writing

static void writeNodeToFile(AccountNode* node) {
    if (node == NULL || dataFilePtr == NULL) return;

    // Create a temporary structure to hold the data we want to save
    AccountData saveData = node->data;

    // Reset history pointer to NULL before saving to prevent writing garbage pointer values
    // to the file, but first, copy the actual stack data.
    TransactionStack tempHistory = *(node->data.history);
    saveData.history = NULL; 

    // Write the main data structure
    fwrite(&saveData, sizeof(AccountData), 1, dataFilePtr);

    // Write the transaction stack data immediately following the main data
    fwrite(&tempHistory, sizeof(TransactionStack), 1, dataFilePtr);
}

/**
 * @brief Saves all account data from the BST to a binary file.
 * * @return bool True on success, false on failure.
 */
bool saveAllAccounts() {
    if (root == NULL) {
        printf("WARNING: No accounts to save. Skipping file write.\n");
        return true;
    }

    dataFilePtr = fopen(ACCOUNT_DATA_FILE, "wb");
    if (dataFilePtr == NULL) {
        perror("ERROR: Could not open file for saving");
        return false;
    }

    printf("Saving all account data to '%s'...\n", ACCOUNT_DATA_FILE);
    inorderTraversal(root, writeNodeToFile);

    fclose(dataFilePtr);
    dataFilePtr = NULL;
    printf("SUCCESS: Account data saved.\n");
    return true;
}

/**
 * @brief Loads all account data from a binary file and inserts them into the BST.
 * * @return bool True on success, false on failure.
 */
bool loadAllAccounts() {
    FILE *file = fopen(ACCOUNT_DATA_FILE, "rb");
    if (file == NULL) {
        printf("INFO: Data file '%s' not found. Starting with an empty database.\n", ACCOUNT_DATA_FILE);
        return true; // Not an error, just no saved data
    }

    printf("Loading account data from '%s'...\n", ACCOUNT_DATA_FILE);
    AccountData data;
    TransactionStack history;
    size_t data_read, history_read;

    while (1) {
        // Read the main account data
        data_read = fread(&data, sizeof(AccountData), 1, file);
        if (data_read != 1) break; 

        // Read the transaction stack data that immediately follows
        history_read = fread(&history, sizeof(TransactionStack), 1, file);
        if (history_read != 1) {
            printf("ERROR: Corrupt file structure (missing history for ID %d). Stopping load.\n", data.accountID);
            break;
        }

        // 1. Allocate memory for the new node and its history stack
        AccountNode *newNode = (AccountNode *)malloc(sizeof(AccountNode));
        if (newNode == NULL) { perror("Memory allocation failed during load"); break; }
        
        TransactionStack *newHistory = (TransactionStack*)malloc(sizeof(TransactionStack));
        if (newHistory == NULL) { perror("Memory allocation failed for history"); free(newNode); break; }

        // 2. Copy the loaded data
        newNode->data = data; // Copies all fields except the pointer
        memcpy(newHistory, &history, sizeof(TransactionStack));
        
        // 3. Re-link the history pointer
        newNode->data.history = newHistory;
        
        // 4. Initialize BST pointers
        newNode->left = NULL;
        newNode->right = NULL;

        // 5. Insert the new node into the BST
        root = insertAccountBST(root, newNode);
        printf("Loaded account ID %d: %s\n", data.accountID, data.name);
    }

    if (ferror(file)) {
        perror("ERROR: Error reading file during load");
        fclose(file);
        return false;
    }

    fclose(file);
    printf("SUCCESS: Account data loading complete.\n");
    return true;
}

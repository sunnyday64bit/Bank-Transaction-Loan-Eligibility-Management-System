#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdbool.h>
#include "account.h"

// Define the file name for persistent storage
#define ACCOUNT_DATA_FILE "bank_accounts.dat"

/**
 * @brief Saves all account data from the BST to a binary file.
 * The BST is traversed in-order to write nodes sequentially.
 * * @return bool True on success, false on failure.
 */
bool saveAllAccounts();

/**
 * @brief Loads all account data from a binary file and inserts them into the BST.
 * This is called at the beginning of the program.
 * * @return bool True on success, false on failure.
 */
bool loadAllAccounts();

#endif

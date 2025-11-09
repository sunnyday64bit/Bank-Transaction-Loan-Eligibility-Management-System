#include "report.h"
#include "account.h"
#include "queue.h"
#include "priority_queue.h"
#include <stdio.h>

// Global accumulator structure for the report
typedef struct BankMetrics {
    int totalAccounts;
    double totalBalance;
    long long totalCibilScore;
    int totalTransactions;
} BankMetrics;

// External access to global queue/heap variables from queue.c and priority_queue.c
extern ServiceQueue serviceQ;
extern int heapSize;
extern LoanApplicant heap[]; // Need to access the heap structure for top applicant

// Global root of the BST
extern AccountNode *root; 

/**
 * @brief Recursive traversal function to visit all nodes in the BST and collect metrics.
 */
void collectMetricsTraversal(AccountNode *node, BankMetrics *metrics) {
    if (node == NULL) {
        return;
    }
    
    // Visit left subtree
    collectMetricsTraversal(node->left, metrics);

    // Process current node (data collection)
    metrics->totalAccounts++;
    metrics->totalBalance += node->data.balance;
    metrics->totalCibilScore += node->data.cibilScore;
    metrics->totalTransactions += node->data.totalTransactions;

    // Visit right subtree
    collectMetricsTraversal(node->right, metrics);
}

/**
 * @brief Generates and prints the final bank system report.
 */
void generateSummaryReport() {
    BankMetrics metrics = {0, 0.0, 0, 0};

    printf("\n================================================\n");
    printf("         BANK SYSTEM EXECUTIVE REPORT\n");
    printf("================================================\n");

    // 1. Account Metrics (from BST Traversal)
    collectMetricsTraversal(root, &metrics);

    double avgCibil = (metrics.totalAccounts > 0) ? (double)metrics.totalCibilScore / metrics.totalAccounts : 0.0;
    
    printf("--- 1. Financial & Customer Summary (BST) ---\n");
    printf("Total Active Customer Accounts: %d\n", metrics.totalAccounts);
    printf("Total Bank Assets (Combined Balance): $%.2lf\n", metrics.totalBalance);
    printf("Average Customer CIBIL Score: %.2lf\n", avgCibil);
    printf("System-Wide Total Transactions Processed: %d\n", metrics.totalTransactions);
    printf("-------------------------------------------\n");

    // 2. Service Queue Status (M4 Queue)
    printf("--- 2. Service Requests (Queue Status) ---\n");
    printf("Current Pending Service Requests: %d (Capacity: %d)\n", serviceQ.count, MAX_QUEUE);
    if (serviceQ.count > 0) {
        int nextIndex = serviceQ.front;
        printf("Next Request (FIFO): ID %d | Details: %.20s...\n", 
               serviceQ.items[nextIndex].accountID, 
               serviceQ.items[nextIndex].details);
    }
    printf("-------------------------------------------\n");

    // 3. Loan Priority Queue Status (M6 PQ)
    printf("--- 3. Loan Processing (Priority Queue) ---\n");
    printf("Pending Loan Applications: %d (Max Capacity: %d)\n", heapSize, MAX_PQ_SIZE);
    if (heapSize > 0) {
        // Highest priority is at index 1 in the Max Heap
        printf("Highest Priority Applicant:\n");
        printf("  - ID: %d, Name: %s\n", heap[1].accountID, heap[1].name);
        printf("  - Priority Score: %.2lf\n", heap[1].priorityScore);
    } else {
        printf("Loan queue is currently empty.\n");
    }
    printf("================================================\n");
}

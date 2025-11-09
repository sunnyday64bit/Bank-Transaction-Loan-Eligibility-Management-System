#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Array to store the Max Heap
LoanApplicant heap[MAX_PQ_SIZE + 1]; // Index 0 is unused, start from 1 for easier math
int heapSize = 0;

void initPriorityQueue() {
    heapSize = 0;
    // Optional: Clear the heap array structure
    memset(heap, 0, sizeof(heap));
}

// Utility to swap two applicants in the heap array
void swap(int i, int j) {
    LoanApplicant temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

/**
 * @brief Restores the Max Heap property by moving the node up (bubble up).
 * @param index The index of the newly inserted element.
 */
void heapifyUp(int index) {
    // Current node must be greater than its parent (Max Heap property)
    while (index > 1 && heap[index].priorityScore > heap[index / 2].priorityScore) {
        swap(index, index / 2);
        index = index / 2;
    }
}

/**
 * @brief Restores the Max Heap property by moving the node down (bubble down).
 * @param index The index of the node to start heapifying from.
 */
void heapifyDown(int index) {
    int leftChild = 2 * index;
    int rightChild = 2 * index + 1;
    int largest = index;

    // Check if left child exists and is larger than current largest
    if (leftChild <= heapSize && heap[leftChild].priorityScore > heap[largest].priorityScore) {
        largest = leftChild;
    }

    // Check if right child exists and is larger than current largest
    if (rightChild <= heapSize && heap[rightChild].priorityScore > heap[largest].priorityScore) {
        largest = rightChild;
    }

    // If largest is not the current index, swap and continue heapifying down
    if (largest != index) {
        swap(index, largest);
        heapifyDown(largest);
    }
}

/**
 * @brief Adds a new loan applicant to the Priority Queue.
 */
void enqueueApplicant(int id, const char *name, double priority) {
    if (heapSize >= MAX_PQ_SIZE) {
        printf("FAILURE: Loan priority queue is full (Max %d applicants).\n", MAX_PQ_SIZE);
        return;
    }

    heapSize++;
    int index = heapSize;
    
    // Insert new applicant at the end
    heap[index].accountID = id;
    strncpy(heap[index].name, name, sizeof(heap[index].name) - 1);
    heap[index].name[sizeof(heap[index].name) - 1] = '\0';
    heap[index].priorityScore = priority;

    // Restore Max Heap property
    heapifyUp(index);
    printf("SUCCESS: Applicant %s (ID %d) added to Loan Queue (Priority: %.2lf).\n", name, id, priority);
}

/**
 * @brief Removes and processes the applicant with the highest priority.
 */
void dequeueHighestPriority() {
    if (heapSize == 0) {
        printf("Loan priority queue is empty. No applications to process.\n");
        return;
    }

    LoanApplicant topApplicant = heap[1]; // Highest priority is always at index 1

    printf("\n--- PROCESSING HIGHEST PRIORITY LOAN APPLICATION ---\n");
    printf("Applicant Name: %s (ID: %d)\n", topApplicant.name, topApplicant.accountID);
    printf("Priority Score: %.2lf\n", topApplicant.priorityScore);
    printf("STATUS: Loan application approved for processing based on ranking.\n");
    
    // Replace the root (index 1) with the last element
    heap[1] = heap[heapSize];
    heapSize--;

    // Restore Max Heap property
    heapifyDown(1);
}

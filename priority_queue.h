#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#define MAX_PQ_SIZE 10 // Max number of loan applicants in the queue

typedef struct LoanApplicant {
    int accountID;
    char name[50];
    // Priority Score: Higher score = Higher priority (Max Heap)
    double priorityScore; 
} LoanApplicant;

// Function prototypes
void initPriorityQueue();
void enqueueApplicant(int id, const char *name, double priority);
void dequeueHighestPriority();

#endif

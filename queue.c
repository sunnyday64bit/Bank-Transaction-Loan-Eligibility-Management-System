#include "queue.h"
#include "account.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct ServiceQueue { 
    ServiceRequest items[MAX_QUEUE]; 
    int front; 
    int rear; 
    int count; 
} ServiceQueue;

ServiceQueue serviceQ = { .front = 0, .rear = -1, .count = 0 };

void clear_input_buffer(); 

void handleNewServiceRequest() {
    int id; char details[100];
    
    printf("\n-- SUBMIT SERVICE REQUEST --\n");
    printf("Enter Account ID: "); 
    if (scanf("%d", &id) != 1) { printf("Invalid input.\n"); return; }
    
    clear_input_buffer(); 
    
    printf("Enter Request Details (max 99 chars): "); 
    if (fgets(details, sizeof(details), stdin) == NULL) return;
    details[strcspn(details, "\n")] = 0; 
    
    if (findAccountNode(id) == NULL) { printf("FAILURE: Account ID %d not found.\n", id); return; }
    if (serviceQ.count == MAX_QUEUE) { printf("FAILURE: Queue full.\n"); return; }
    
    serviceQ.rear = (serviceQ.rear + 1) % MAX_QUEUE;
    serviceQ.items[serviceQ.rear].accountID = id;
    strncpy(serviceQ.items[serviceQ.rear].details, details, sizeof(serviceQ.items[serviceQ.rear].details) - 1);
    serviceQ.count++;
    printf("SUCCESS: Request submitted for ID %d.\n", id);
}

void dequeueServiceRequest() {
    if (serviceQ.count == 0) { printf("Service queue is empty.\n"); return; }
    ServiceRequest req = serviceQ.items[serviceQ.front];
    serviceQ.front = (serviceQ.front + 1) % MAX_QUEUE;
    serviceQ.count--;
    printf("\n--- Processing Request (FIFO) ---\n");
    printf("Processed request for ID: %d | Details: %s\n", req.accountID, req.details);
}

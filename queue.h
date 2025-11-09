#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE 10

typedef struct ServiceRequest { 
    int accountID; 
    char details[100]; 
} ServiceRequest;

typedef struct ServiceQueue { 
    ServiceRequest items[MAX_QUEUE]; 
    int front; 
    int rear; 
    int count; 
} ServiceQueue;

// Export the global queue structure for status checks (used in report.c)
extern ServiceQueue serviceQ;

void handleNewServiceRequest();
void dequeueServiceRequest();

#endif

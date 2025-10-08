#ifndef QUEUE_H
#define QUEUE_H

#define MAX_QUEUE 10

typedef struct ServiceRequest { 
    int accountID; 
    char details[100]; 
} ServiceRequest;

void handleNewServiceRequest();
void dequeueServiceRequest();

#endif

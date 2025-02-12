#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Author: Joshua Shu
//Priority Round Robin algorithm
//Picks the job with the highest priority first, performing round robin scheduling if there are multiple jobs with the same priority
//Uses a quantum of 10 units

#define QUANTUM 10
#define MAX_PRIORITY 10

struct ProcessNode {
    char name[5];
    int priority;
    int burst;
    int remainingBurst;
    struct ProcessNode* nextNode;
};

struct ProcessNode* newProcess(char* name, int priority, int burst) {
    struct ProcessNode* newNode = (struct ProcessNode*)malloc(sizeof(struct ProcessNode));
    strcpy(newNode->name, name);
    newNode->priority = priority;
    newNode->burst = burst;
    newNode->remainingBurst = burst;
    newNode->nextNode = NULL;
    return newNode;
}

void addProcess(struct ProcessNode** priorityQueue, char* name, int priority, int burst) {
    struct ProcessNode* newNode = newProcess(name, priority, burst);

    if (priorityQueue[priority] == NULL) {
        priorityQueue[priority] = newNode;
        newNode->nextNode = newNode;
    } else {
        struct ProcessNode* tempNode = priorityQueue[priority];
        while (tempNode->nextNode != priorityQueue[priority]) {
            tempNode = tempNode->nextNode;
        }
        tempNode->nextNode = newNode;
        newNode->nextNode = priorityQueue[priority];
    }
}

void roundRobinPriority(struct ProcessNode** priorityQueue) {
    int currTime = 0;

    printf("Process Name\tPriority\tBurst\tTime in Processor\tRemaining Time\n");

    for (int i = MAX_PRIORITY; i >= 0; i--) {
        struct ProcessNode* headNode = priorityQueue[i];

        if (headNode == NULL) {
            continue;
        }

        int processesRemaining = 1;

        while (processesRemaining) {
            processesRemaining = 0;

            struct ProcessNode* currNode = headNode;
            struct ProcessNode* prevNode = NULL;

            do {
                int timeProcessed = (currNode->remainingBurst > QUANTUM) ? QUANTUM : currNode->remainingBurst;

                printf("%s\t\t%d\t\t%d\t\t%d\t\t%d\n", currNode->name, currNode->priority, currNode->burst, timeProcessed, currNode->remainingBurst - timeProcessed);

                currTime += timeProcessed;
                currNode->remainingBurst -= timeProcessed;

                struct ProcessNode* nextNode = currNode->nextNode;

                if (currNode->remainingBurst == 0) {
                    printf("Process %s has been completed.\n", currNode->name);

                    if (currNode == headNode) {
                        if (currNode->nextNode == currNode) {
                            priorityQueue[i] = NULL;
                            break;
                        } else {
                            headNode = currNode->nextNode;
                            priorityQueue[i] = headNode;
                        }
                    }

                    if (prevNode == NULL) {
                        prevNode = currNode;
                        while (prevNode->nextNode != currNode) {
                            prevNode = prevNode->nextNode;
                        }
                    }

                    prevNode->nextNode = currNode->nextNode;

                    processesRemaining = (priorityQueue[i] != NULL);
                } else {
                    processesRemaining = 1;
                    prevNode = currNode;
                }

                currNode = nextNode;

            } while (priorityQueue[i] != NULL && currNode != headNode);
        }
    }
}

void readFile(char* filename, struct ProcessNode** priorityQueue) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("ERROR: Could not open file.");
        exit(1);
    }

    char name[5];
    int priority;
    int burst;

    while (fscanf(file, "%[^,], %d, %d\n", name, &priority, &burst) != EOF) {
        addProcess(priorityQueue, name, priority, burst);
    }

    fclose(file);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("ERROR: Invalid number of arguments.\n");
        return 1;
    }

    char* filename = argv[1];
    struct ProcessNode* priorityQueue[MAX_PRIORITY + 1] = {NULL};

    readFile(filename, priorityQueue);
    printf("~~~~~ Priority Round Robin Scheduler ~~~~~\n");
	printf("Each line represents a quantum of 10 time units.\n");
	printf("Jobs with the highest priority are picked first with round robin scheduling being performed if there are multiple jobs with the same priority.\n\n");
    roundRobinPriority(priorityQueue);

    return 0;
}
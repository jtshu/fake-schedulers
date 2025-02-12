#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Author: Joshua Shu
//Shortest Job First algorithm
//Picks the jobs with the shortest burst time to complete first
//Uses priority as a tie-breaker if there are multiple jobs with the same burst

struct ProcessNode {
	char name[5];
	int priority;
	int burst;
	struct ProcessNode* nextNode;
};

struct ProcessNode* newProcess(char* name, int priority, int burst) {
	struct ProcessNode* newNode = (struct ProcessNode*)malloc(sizeof(struct ProcessNode));
	strcpy(newNode->name, name);
	newNode->priority = priority;
	newNode->burst = burst;
	newNode->nextNode = NULL;
	return newNode;
}

void addProcess(struct ProcessNode** headNode, char* name, int priority, int burst) {
	struct ProcessNode* newNode = newProcess(name, priority, burst);

	if (*headNode == NULL || (*headNode)->burst > burst || 
        ((*headNode)->burst == burst && (*headNode)->priority < priority)) {
		newNode->nextNode = *headNode;
		*headNode = newNode;
	}
	else {
		struct ProcessNode* currNode = *headNode;
		while (currNode->nextNode != NULL && 
              (currNode->nextNode->burst < burst || 
              (currNode->nextNode->burst == burst && currNode->nextNode->priority >= priority))) {
			currNode = currNode->nextNode;
		}

		newNode->nextNode = currNode->nextNode;
		currNode->nextNode = newNode;
	}
}

void schedulerOutput(struct ProcessNode* headNode) {
	printf("Process Name\tPriority\tBurst\tTime in Processor\n");

	struct ProcessNode* tempNode = headNode;
	while (tempNode != NULL) {
		printf("%s\t\t%d\t\t%d\t\t%d\n", tempNode->name, tempNode->priority, tempNode->burst, tempNode->burst);
		tempNode = tempNode->nextNode;
	}
}

void readFile(char* filename, struct ProcessNode** headNode) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		perror("ERROR: Could not open file.\n");
		exit(1);
	}

	char name[5];
	int priority;
	int burst;

	while (fscanf(file, "%[^,], %d, %d\n", name, &priority, &burst) != EOF) {
		addProcess(headNode, name, priority, burst);
	}

	fclose(file);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("ERROR: Invalid number of arguments.\n");
		return 1;
	}

	char* filename = argv[1];
	struct ProcessNode* headNode = NULL;

	readFile(filename, &headNode);
	printf("~~~~~ Shortest Job First Scheduler ~~~~~\n");
	printf("The jobs are listed in execution order with the first executed job at the top.\n");
	printf("Jobs with the shortest burst time are picked first with priority being used a tie-breaker.\n\n");
	schedulerOutput(headNode);

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "holes.h"
#include <string.h>

//Takes two command line args "testfile memStrategy"
//Mem strats = first, best, next, worst
int main(int argc, char * argv[]) {

    char line[20];
    printf("Max memory = %d\n", MEMMAX);
    int memStrat = inputChecker(argc, argv);
    printf("Chosen mem strat = %d\n\n", memStrat);
    int numLines = 0;
    FILE * inFile = fopen(argv[1], "r");

    if (inFile == NULL) {
        printf("Error, could not open file\n");
        return 0;
    }
    //Gather number of lines for allocation
    while (fgets(line, 100, inFile) != NULL) {
        numLines++;
    }
    printf("Num Lines = %d\n", numLines);
    fclose(inFile);

    proc_list * processes = malloc(sizeof(struct proc_list)); //Initial list that holds all procs
    sim * ms = (sim *)malloc(sizeof(struct sim)); //Memory sim struct
    ms->space_rem = MEMMAX; //Addressing 0-1023
    processes->proc_arr = malloc(sizeof(struct process) * numLines);
    processes->numProcs = numLines;
    
    printf("Here?\n");
    const char s[2] = " ";
    char * token;
    int cnt = 0;
    inFile = fopen(argv[1], "r");
    while (fgets(line, 100, inFile) != NULL) {
        //parse line and store data
        //pidnum <space> memSize
        process p;
        token = strtok(line, s);//pid
        p.pid = atoi(token);
        p.currentQ = p.pid; //Set current q to its process number then deal as time goes on
        token = strtok(NULL, s);//memchunk
        p.memchunk = atoi(token);
        processes->proc_arr[cnt] = p;
        cnt++;
    }
    fclose(inFile);
    printf("\n");

    for (int i = 0; i < processes->numProcs; i++) {
        printf("Process pid %d needs %d memory\n", processes->proc_arr[i].pid, processes->proc_arr[i].memchunk);
    }

    Heap * q = CreateHeap(processes->numProcs, 0);
    
    for(int i = 0; i < processes->numProcs; i++) {//Add all processes to q to start
        insert(q, processes->proc_arr[i]);
    }

    print(q);
    
    //0-1024
    //Start making algorithm for first
    if (memStrat == 1) {
        printf("Allocate by first\n");
        printf("Current mem left for allocation = %d\n", ms->space_rem);
        process node = PopMin(q);
        insertNode(node, ms);
        
        printf("sim pointer pointer points to pid %d and has memchunk %d\n", ms->head->pid, ms->head->memchunk);
        printf("Node popped currentQ = %d\n", node.currentQ);
        node = PopMin(q);
        printf("Node popped currentQ = %d\n", node.currentQ);

    }

    free(q->arr);
    free(q);
    free(processes->proc_arr);
    free(processes);
    free(ms);
}

void insertNode(process node, sim * ms) {
    printf("Insert node function\n");
    process * track = NULL;
    if (ms->head == NULL) {
        printf("The LL is empty\n");
        ms->head = &node;
    } else {
        //LL is not empty
        track = ms->head;
        while (track->next != NULL) {
            track = track->next;
        } //Gonna have to modify this to suit the different allocation algorithms
        //Track got to the end of the LL
        track->next = &node;
        track->next->next = NULL;
    }
    
}
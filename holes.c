#include <stdio.h>
#include <stdlib.h>
#include "holes.h"
#include <string.h>

//Takes two command line args "testfile memStrategy"
//Mem strats = first, best, next, worst

void insertNode(process * node, sim * ms);

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

    sim * ms = (sim *)malloc(sizeof(struct sim)); //Memory sim struct
    ms->space_rem = MEMMAX; //Addressing 0-1023
    ms->head = NULL;
    ms->nodeCnt = 0;
    Heap * q = CreateHeap(numLines, 0);
    
    printf("Here?\n");
    const char s[2] = " ";
    char * token;
    int cnt = 0;
    inFile = fopen(argv[1], "r");
    while (fgets(line, 100, inFile) != NULL) {
        //parse line and store data
        //pidnum <space> memSize
        process * p = (process *)malloc(sizeof(process));
        token = strtok(line, s);//pid
        p->pid = atoi(token);
        p->currentQ = p->pid; //Set current q to its process number then deal as time goes on
        token = strtok(NULL, s);//memchunk
        p->memchunk = atoi(token);
        insert(q, p);    
        // processes->proc_arr[cnt] = p;
        cnt++;
    }
    fclose(inFile);
    printf("\n");

    // for (int i = 0; i < processes->numProcs; i++) {
    //     printf("Process pid %d needs %d memory\n", processes->proc_arr[i].pid, processes->proc_arr[i].memchunk);
    // }

    
    
    // for(int i = 0; i < processes->numProcs; i++) {//Add all processes to q to start
    //     insert(q, processes->proc_arr[i]);
    // }

    print(q);
    
    //0-1024
    //Start making algorithm for first
    if (memStrat == 1) {
        printf("Allocate by first\n");
        printf("Current mem left for allocation = %d\n", ms->space_rem);
        process * node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms);
        printf("Current head memchunk %d\n", ms->head->memchunk);
        // printf("sim pointer points to pid %d and has memchunk %d\n", ms->head->pid, ms->head->memchunk);
        // printf("Node popped currentQ = %d\n", node.currentQ);
        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms);
        // printf("Node popped currentQ = %d\n", node.currentQ);

    }

    free(q->arr);
    free(q);
    free(ms);
}

void insertNode(process * node, sim * ms) {
    process * track = NULL;

    if (ms->head == NULL) {
        printf("The LL is empty\n");
        ms->head = node;
        ms->head->next = NULL;
        printf("ms->head->memchunk %d\n", ms->head->memchunk);
        ms->head->start = 0;
        ms->head->end = node->memchunk;
        printf("Process start %d and end %d\n", ms->head->start, ms->head->end);
        ms->space_rem -= node->memchunk;
        printf("%d Memory added, Space remaining %d\n", node->memchunk, ms->space_rem);
    } else if (ms->head != NULL) {
        printf("LL is not empty\n");
        track = ms->head;
        printf("track->memchunk = %d\n", ms->head->memchunk);
        while (track->next != NULL) {
            int spaceAfter = track->next->start - track->end;
            if (spaceAfter >= node->memchunk) {//The space between processes is big enough to accomadate
                node->next = track->next;
                track->next = node;
                node->start = track->end;
                node->end = node->start + node->memchunk;
                printf("process start %d and process end %d", node->start, node->end);
            } else {//Need to do some swapping
                printf("Need to do some swapping to make room\n");
            }
            track = track->next;
        } //Gonna have to modify this to suit the different allocation algorithms
        int spaceAfter = MEMMAX - track->end;//The space remaining after the last process in the list
        printf("Space after last node in memory %d\n", spaceAfter);
        if (track->next == NULL && spaceAfter >= node->memchunk) {//Track got to the end of the LL
            track->next = node;
            track->next->next = NULL;
            ms->space_rem -= node->memchunk;
            node->start = track->end;
            node->end = node->start + node->memchunk;
            printf("%d Memory added, Space remaining %d\n", node->memchunk, ms->space_rem);
            printf("process start %d and process end %d\n", node->start, node->end);
        } else {
            printf("Need to do some swapping to make room\n");
        }
        
    }
    track = NULL;
}
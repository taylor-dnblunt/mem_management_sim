#include <stdio.h>
#include <stdlib.h>
#include "hole.h"
#include <string.h>

//Takes two command line args "testfile memStrategy"
//Mem strats = first, best, next, worst

int main(int argc, char * argv[]) {

    char line[20];
    int memStrat = inputChecker(argc, argv);
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
    fclose(inFile);

    sim * ms = (sim *)malloc(sizeof(struct sim)); //Memory sim struct
    ms->space_rem = MEMMAX; //Addressing 0-1023
    ms->head = NULL;
    ms->nodeCnt = 0;
    ms->curNumPIDLoads = 0;
    ms->cumPercTotal = 0;
    ms->mostRecentPid = 0;
    ms->lowestTime = 0;
    ms->numprocs = numLines;
    Heap * q = CreateHeap(numLines, 0);
    process * node = NULL;
    process ** pp = (process**) malloc(sizeof(process *) * numLines);
    
    process * p = NULL;
    const char s[2] = " ";
    char * token;
    int cnt = 0;
    inFile = fopen(argv[1], "r");
    while (fgets(line, 100, inFile) != NULL) {
        p = (process *)malloc(sizeof(process));
        pp[cnt] = p;
        
        token = strtok(line, s);//pid
        p->pid = atoi(token);
        p->timeStamp = p->pid; //Set current q to its process number then deal as time goes on
        token = strtok(NULL, s);//memchunk
        p->memchunk = atoi(token);
        p->numSwaps = 0;
        insert(q, p);    
        cnt++;
    }
    fclose(inFile);

    // print(q);
    if (memStrat == 1) {
        printf("Allocate by first\n");
        printf("\n");
        if (ms->head != NULL) {
            printf("Current head node has %d memchunk\n", ms->head->memchunk);
            if (ms->head->next != NULL) {
                printf("Head node ends at %d and next node starts at %d\n", ms->head->end, ms->head->next->start);
                printf("Following node %d memchunk\n", ms->head->next->memchunk);
            }
        }
        while (q->count != 0) {//While the q is not empty swap in and out processes
            node = PopMin(q);   
            insertNode(node, ms, q);
            // printMem(ms);
        }
    } else if (memStrat == 2) {//best fit
        printf("Allocate by best fit\n");
        printf("\n");
        while (q->count != 0) {//While the q is not empty swap in and out processes
            // printMem(ms);
            node = PopMin(q);   
            insertNodeBest(node, ms, q);
        }
    } else if (memStrat == 3) {//next fit
        printf("Allocate by next fit\n");
        printf("\n");
        while (q->count != 0) {//While the q is not empty swap in and out processes
            // printMem(ms);
            node = PopMin(q);   
            insertNodeNext(node, ms, q);
        }
    } else if (memStrat == 4) {//worst fit
        printf("Allocate by worst fit\n");
        printf("\n");
        while (q->count != 0) {//While the q is not empty swap in and out processes
            // printMem(ms);
            node = PopMin(q);   
            insertNodeWorst(node, ms, q);
        }
    }

    printSummary(ms);
    
    free(q->arr);
    free(q);
    for (int i = 0; i < numLines; i++) {
        free(pp[i]);
    }
    free(pp);
    free(ms);
}
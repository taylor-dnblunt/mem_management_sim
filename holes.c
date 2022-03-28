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

    sim * ms = (sim *)malloc(sizeof(struct sim)); //Memory sim struct
    ms->cumPercMem = 0;
    ms->space_rem = MEMMAX; //Addressing 0-1023
    ms->head = NULL;
    ms->nodeCnt = 0;
    ms->numprocs = numLines;
    Heap * q = CreateHeap(numLines, 0);
    
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
        p->timeStamp = p->pid; //Set current q to its process number then deal as time goes on
        token = strtok(NULL, s);//memchunk
        p->memchunk = atoi(token);
        p->numSwaps = 0;
        printf("Process %d with memory %d\n", p->pid, p->memchunk);
        insert(q, p);    
        // processes->proc_arr[cnt] = p;
        cnt++;
    }
    fclose(inFile);
    printf("\n");

    print(q);
    
    //0-1024
    //Start making algorithm for first
    if (memStrat == 1) {
        printf("Allocate by first\n");
        printf("Current mem left for allocation = %d\n", ms->space_rem);
        process * node;
        // node = PopMin(q);
        // insertNode(node, ms, q);
        // // printf("Current head memchunk %d\n", ms->head->memchunk);
        // // printf("sim pointer points to pid %d and has memchunk %d\n", ms->head->pid, ms->head->memchunk);

        // node = PopMin(q);
        // insertNode(node, ms, q);
        // // printf("Node popped currentQ = %d\n", node.currentQ);
        // node = PopMin(q);
        // insertNode(node, ms, q);

        // node = PopMin(q);
        // insertNode(node, ms, q); //4th insert node

        // node = PopMin(q);
        // insertNode(node, ms, q); //5th insert node

        // node = PopMin(q);
        // insertNode(node, ms, q); //6th insert node

        // node = PopMin(q);
        // insertNode(node, ms, q); //7th insert node

        // node = PopMin(q);
        // insertNode(node, ms, q); //8th insert node

        // node = PopMin(q);
        // insertNode(node, ms, q); //9th insert node back to 130

        // node = PopMin(q);
        // insertNode(node, ms, q); //10th insert node back to 99

        // node = PopMin(q);
        // insertNode(node, ms, q); //11th insert node back to 200

        // node = PopMin(q);
        // insertNode(node, ms, q); //12th insert node back to 512

        // node = PopMin(q);
        // insertNode(node, ms, q); //13th insert node back to 440

        // node = PopMin(q);
        // insertNode(node, ms, q); //14th insert node back to 320

        if (ms->head != NULL) {
            printf("Current head node has %d memchunk\n", ms->head->memchunk);
            if (ms->head->next != NULL) {
                printf("Head node ends at %d and next node starts at %d\n", ms->head->end, ms->head->next->start);
                printf("Following node %d memchunk\n", ms->head->next->memchunk);
            }
        }
        while (q->count != 0) {//While the q is not empty swap in and out processes
            node = PopMin(q);   
            insertNode(node, ms, q); //12th insert node back to 512
        }

    }

    printMem(ms);
    print(q);
    // for (int i = 0; i < numLines; i++) {
    //     free(q->arr[i]);
    // } //Going to have to free all the processes
    free(q->arr);
    free(q);
    free(ms);
}
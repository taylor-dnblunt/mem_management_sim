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

    process * pptr;//Allocate an array of 20 processes
    process proc_list;
    proc_list.proc_arr = malloc(sizeof(struct process) * numLines);
    pptr = &proc_list;
    pptr->numProcs = numLines;
    

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
        token = strtok(NULL, s);//memchunk
        p.memchunk = atoi(token);
        pptr->proc_arr[cnt] = p;
        cnt++;
    }
    fclose(inFile);
    printf("\n");

    for (int i = 0; i < pptr->numProcs; i++) {
        printf("Process pid %d needs %d memory\n", pptr->proc_arr[i].pid, pptr->proc_arr[i].memchunk);
    }

    Heap * q = CreateHeap(pptr->numProcs, 0);
    
    for(int i = 0; i < pptr->numProcs; i++) {
        insert(q, pptr->proc_arr[i]);
    }

    print(q);
    
    free(pptr->proc_arr);
}
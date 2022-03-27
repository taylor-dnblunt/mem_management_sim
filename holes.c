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
    ms->space_rem = MEMMAX; //Addressing 0-1023
    ms->head = NULL;
    ms->nodeCnt = 0;
    ms->numprocs = numLines;
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
        process * node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q);
        printf("Current head memchunk %d\n", ms->head->memchunk);
        // printf("sim pointer points to pid %d and has memchunk %d\n", ms->head->pid, ms->head->memchunk);
        // printf("Node popped currentQ = %d\n", node.currentQ);
        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q);
        // printf("Node popped currentQ = %d\n", node.currentQ);
        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q);

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //4th insert node

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //5th insert node

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //6th insert node

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //7th insert node

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //8th insert node

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //9th insert node back to 130

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //10th insert node back to 99

        node = PopMin(q);
        printf("\nInsert node function\n");
        insertNode(node, ms, q); //11th insert node back to 200

        if (ms->head != NULL) {
            printf("Current head node has %d memchunk\n", ms->head->memchunk);
            printf("Head node ends at %d and next node starts at %d\n", ms->head->end, ms->head->next->start);
            printf("Following node %d memchunk\n", ms->head->next->memchunk);
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

void insertNode(process * node, sim * ms, Heap * q) {
    process * track = NULL;

    if (ms->head == NULL) {//If the list is empty insert at head
        printf("\nThe LL is empty\n");
        ms->head = node;
        ms->head->next = NULL;
        printf("ms->head->memchunk %d\n", ms->head->memchunk);
        ms->head->start = 0;
        ms->head->end = node->memchunk;
        printf("Process start %d and end %d\n", ms->head->start, ms->head->end);
        ms->space_rem -= node->memchunk;
        ms->nodeCnt++;
        printf("%d Memory added, Space remaining %d\n", node->memchunk, ms->space_rem);

    } else if (ms->head != NULL) {
        printf("\nLL is not empty\n");
        track = ms->head;
        if (ms->head->start - 0 >= node->memchunk) {//There is space at the start of the memory allocation list
            printf("Insert before the first node\n");
            node->next = ms->head;//place node at the start of the list
            ms->head = node;
            ms->head->start = 0;
            ms->head->end = ms->head->memchunk;
            ms->space_rem -= ms->head->memchunk;
            ms->nodeCnt++;
            printf("process start %d and process end %d\n", node->start, node->end);
            return;
        }
        while (track->next != NULL) {
            if (ms->nodeCnt >= 2) {//Enter node into the middle of two if there is space
                printf("Greater than 2 nodes\n");
                int spaceAfter = track->next->start - track->end;
                if (spaceAfter >= node->memchunk) {//The space between processes is big enough to accomadate
                    node->next = track->next;
                    track->next = node;
                    ms->space_rem -= node->memchunk;
                    ms->nodeCnt++;
                    node->start = track->end + 1;
                    node->end = node->start + node->memchunk;
                    printf("process start %d and process end %d\n", node->start, node->end);
                    return;//Leave function if space is found between two processes
                } 
            }
            
            track = track->next;
        } //Gonna have to modify this to suit the different allocation algorithms
        //There is not enough space between two nodes is there at the end?
        int spaceAfter = MEMMAX - track->end;//The space remaining after the last process in the list
        printf("Space after last node in memory %d\n", spaceAfter);
        if (track->next == NULL && spaceAfter >= node->memchunk) {//Track got to the end of the LL
            track->next = node;
            track->next->next = NULL;
            ms->space_rem -= node->memchunk;
            ms->nodeCnt++;
            node->start = track->end + 1;
            node->end = node->start + node->memchunk;

            printf("%d Memory added, Space remaining %d\n", node->memchunk, ms->space_rem);
            printf("process start %d and process end %d\n", node->start, node->end);
        } else {
            printf("Need to do some swapping to make room\n");
            swapOut(ms, node, q);
        }
        
    }
    track = NULL;
}

void swapOut(sim * ms, process * node, Heap * q) {
    printf("\nSwap out function\n");
    printf("Need %d contiguous memory\n", node->memchunk);

    int largestSpace = spaceChecker(ms);
    printf("The largest space currently is %d and the memchunk is %d\n", largestSpace, node->memchunk);

    process * traverse = NULL;

    int spaceFound = 0;
    while (largestSpace < node->memchunk) {//Keep swapping out processes til there is enough room
        
        printf("\nThe largest space before the while loop body %d and the memchunk is %d\n", largestSpace, node->memchunk);
        timeStampCheck(ms);
        traverse = ms->head;
        printf("The lowest time stamp is %d\n", ms->lowestTime);

        if (traverse->timeStamp == ms->lowestTime) {//The head of the list has the lowest timestamp
            ms->head->numSwaps++;
            ms->space_rem += ms->head->memchunk;
            ms->head->timeStamp += ms->numprocs;
            printf("Remove process with memchunk %d\n", traverse->memchunk);
            if (ms->head->numSwaps == 3) {
                printf("This process has been swapped out 3 times and is complete\n"); //Prob free node now
                process * temp = ms->head;
                ms->head = ms->head->next;
                temp->next = NULL;
                free(temp);
            } else {
                printf("This process has %d swaps\n", ms->head->numSwaps);
                insert(q, ms->head);
            }

            if (ms->head->next == NULL) {
                printf("Last node in list\n");
                ms->head = NULL;
                printf("No memory is currently allocated, list is empty\n");
                largestSpace = MEMMAX;
            } else {
                ms->head = ms->head->next; //The new head is the next head
                printf("New head of the list has memchunk %d and starts at %d\n", ms->head->memchunk, ms->head->start); 
                if (ms->head->next != NULL) {
                    printf("and next in line is %d\n",ms->head->next->memchunk);
                }
            }
            
            printf("Space reminaing after swapping out %d\n", ms->space_rem);

        } else {//Not the head of the list
            while (traverse->next != NULL) {//Traverse list and find lowest timestamp
                if (traverse->next->timeStamp == ms->lowestTime) {
                    //Remove node and add it back to priority queue
                    process * temp = NULL;
                    temp = traverse->next; //The node to be removed
                    temp->numSwaps++;
                    temp->timeStamp += ms->numprocs;
                    printf("The node to be removed has timestamp %d\n", temp->timeStamp);
                    if (temp->numSwaps == 3) {
                        printf("This process has been swapped out 3 times and is complete\n");
                        free(temp);
                    } else {
                        insert(q, temp);
                    }
                    traverse->next = traverse->next->next;
                    temp = NULL;
                }
            traverse = traverse->next;
            }
        }
        largestSpace = spaceChecker(ms);
        printf("The largest space at the end of the while loop body %d\n", largestSpace);
        printf("The memchunk the largest space needs to be over %d\n", node->memchunk);
        //After here a process has been removed. Now check for enough memory
    }
    //There is enough space for a process
    printf("Before the insert node in swapout function\n");
    insertNode(node, ms, q);
    
}

int spaceChecker(sim  * ms) {
    int largestSpace = 0;
    int space = 0;
    process * traverse = NULL;
    traverse = ms->head;
    int i = 1;
    //Check space
    if (ms->head == NULL) {
        printf("The list is empty\n");
        return ms->space_rem;
    } else {
        while (traverse->next != NULL) {//Traverse list and find lowest timestamp
            if (traverse->start != 0 && i == 1) {
                space = traverse->start - 0;
                if (space > largestSpace) {
                    largestSpace = space;
                    printf("Largest space at the beginning\n");
                }
            } else if (traverse->next->start - traverse->end > 1) {//There is a hole between processes
                space = traverse->next->start - traverse->end;
                if (space > largestSpace) {
                    largestSpace = space;
                    printf("Space between processes\n");
                }
            } 
            traverse = traverse->next;
            i++;
        }
        if (traverse->next == NULL) {
            space = MEMMAX - traverse->end;
            if (space > largestSpace) {
                largestSpace = space;
                printf("Largest space at the end\n");
            }
        }
        return largestSpace;
    }
    
}

void timeStampCheck(sim * ms) {
    process * traverse = NULL;
    traverse = ms->head;
    ms->lowestTime = ms->head->timeStamp;
    //Take oldest process out and add back to priority queue
    while (traverse->next != NULL) {//Traverse list and find lowest timestamp
        if (traverse->timeStamp < ms->lowestTime) {
            ms->lowestTime = traverse->timeStamp;
        }
        traverse = traverse->next;
    }
}

void printMem(sim * ms) {
    if (ms->head != NULL) {
        process * temp = ms->head;
        while (temp->next != NULL) {
            printf("Current node starts at %d and ends at %d with memory %d\n", temp->start, temp->end, temp->memchunk);
            temp = temp->next;
        }   
    }
    
}

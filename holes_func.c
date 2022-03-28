#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "holes.h"


int inputChecker(int argc, char * argv[]) {

    if (argc < 3) {
        printf("Not enough command line arguments\n");
        return -1;
    }

    char * inFile = argv[1];
    char * strat = argv[2];
    int memStrat = 0;

    printf("Input file name = %s\n", inFile);
    printf("Memory strategy = %s\n", strat);

    if (strcmp(strat, "first") == 0) {
        return memStrat = 1;
    } else if (strcmp(strat, "best") == 0) {
        return memStrat = 2;
    } else if (strcmp(strat, "next") == 0) {
        return memStrat = 3;
    } else if (strcmp(strat, "worst") == 0) {
        return memStrat = 4;
    } else {//Second command line argument is incorrect
        printf("Please use first, best, next, or worst\n");
        return 0;
    }
}

void insertNode(process * node, sim * ms, Heap * q) {
    process * track = NULL;
    int once = 0;
    printf("\n***INSERT***\n");
    if (ms->head == NULL) {//If the list is empty insert at head
        //printf("The LL is empty\n");
        ms->head = node;
        ms->head->next = NULL;
        // printf("ms->head->memchunk %d\n", ms->head->memchunk);
        ms->head->start = 0;
        ms->head->end = node->memchunk;
        // printf("Process start %d and end %d\n", ms->head->start, ms->head->end);
        ms->space_rem -= node->memchunk;
        printAlloInfo(ms);
        ms->nodeCnt++;
        // printf("%d Memory added, Space remaining %d\n", node->memchunk, ms->space_rem);

    } else if (ms->head != NULL) {
        //printf("LL is not empty\n");
        track = ms->head;
        if (ms->head->start - 0 >= node->memchunk) {//There is space at the start of the memory allocation list
            //printf("Insert before the first node\n");
            node->next = ms->head;//place node at the start of the list
            ms->head = node;
            ms->head->start = 0;
            ms->head->end = ms->head->memchunk;
            ms->space_rem -= ms->head->memchunk;
            printAlloInfo(ms);
            ms->nodeCnt++;
            // printf("process start %d and process end %d\n", node->start, node->end);
            return;
        }
        while (track->next != NULL) {
            if (ms->nodeCnt >= 2) {//Enter node into the middle of two if there is space
                //printf("Greater than 2 nodes\n");
                int spaceAfter = track->next->start - track->end;
                if (spaceAfter >= node->memchunk) {//The space between processes is big enough to accomadate
                    node->next = track->next;
                    track->next = node;
                    ms->space_rem -= node->memchunk;
                    ms->nodeCnt++;
                    node->start = track->end + 1;
                    node->end = node->start + node->memchunk;
                    printAlloInfo(ms);
                    // printf("process start %d and process end %d\n", node->start, node->end);
                    return;//Leave function if space is found between two processes
                } 
            }
            
            track = track->next;
        } //Gonna have to modify this to suit the different allocation algorithms
        //There is not enough space between two nodes is there at the end?
        // printf("The final node starts at %d and has memory %d\n", track->start, track->memchunk);

        //printMem(ms);

        int spaceAfter = MEMMAX - track->end;//The space remaining after the last process in the list
        // printf("Space after last node in memory %d\n", spaceAfter);
        if (track->next == NULL && spaceAfter >= node->memchunk) {//Track got to the end of the LL
            track->next = node;
            track->next->next = NULL;
            ms->space_rem -= node->memchunk;
            ms->nodeCnt++;
            node->start = track->end + 1;
            node->end = node->start + node->memchunk;
            printAlloInfo(ms);
            // printf("%d Memory added, Space remaining %d\n", node->memchunk, ms->space_rem);
            // printf("process start %d and process end %d\n", node->start, node->end);
        } else {
            printf("Need to do some swapping to make room\n");
            swapOut(ms, node, q);
        }
        
    }
    
    track = NULL;
}

void swapOut(sim * ms, process * node, Heap * q) {
    printf("\n***SWAP FUNCTION***\n");
    //printf("Need %d contiguous memory\n", node->memchunk);

    int largestSpace = spaceChecker(ms);
    //printf("The largest space currently is %d and the memchunk is %d\n", largestSpace, node->memchunk);

    process * temp = NULL;

    int spaceFound = 0;
    while (largestSpace < node->memchunk) {//Keep swapping out processes til there is enough room
        
        //printf("\nThe largest space before the while loop body %d and the memchunk is %d\n", largestSpace, node->memchunk);
        timeStampCheck(ms);
        temp = ms->head;
        printf("The lowest time stamp is %d\n", ms->lowestTime);

        if (temp->timeStamp == ms->lowestTime) {//The head of the list has the lowest timestamp
            headSwap(ms, temp, largestSpace, q);

        } else {//Not the head of the list
            nonHeadSwap(ms, temp, q);
        }
        largestSpace = spaceChecker(ms);
        //printf("The largest space after the while loop body %d\n", largestSpace);
        //printf("The memchunk the largest space needs to be over %d\n", node->memchunk);
        //After here a process has been removed. Now check for enough memory
    }
    //There is enough space for a process
    //printf("Before the insert node in swapout function\n");
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
            //printf("Space = %d and largest space = %d\n", space, largestSpace);
            if (traverse->start != 0 && i == 1) {
                space = traverse->start - 0;
                if (space > largestSpace) {
                    largestSpace = space;
                    //printf("Largest space at the beginning\n");
                }
            } else if (traverse->next->start - traverse->end > 1) {//There is a hole between processes
                space = traverse->next->start - traverse->end;
                if (space > largestSpace) {
                    largestSpace = space;
                    //printf("Space between processes\n");
                }
            } 
            traverse = traverse->next;
            i++;
            //printf("ms->head->start %d and i = %d\n", ms->head->start, i);
        }
        if (traverse->next == NULL) {
            //printf("Possibly the only node\n");
            if (traverse->start != 0 && i == 1) {
                space = traverse->start - 0;
                if (space > largestSpace) {
                    largestSpace = space;
                    //printf("Largest space before only node\n");
                }
            }
            space = MEMMAX - traverse->end;
            if (space > largestSpace) {
                largestSpace = space;
                //printf("Largest space at the end\n");
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
    if (traverse->timeStamp < ms->lowestTime) {//Check the very last node
        ms->lowestTime = traverse->timeStamp;
    }
}

void printMem(sim * ms) {
    if (ms->head != NULL) {
        process * temp = ms->head;
        printf("\n****************Memory Summary****************\n");
        while (temp->next != NULL) {
            printf("Current node timestamp %d:\n", temp->timeStamp);
            printf("node starts at %d and ends at %d with memory %d\n\n", temp->start, temp->end, temp->memchunk);
            temp = temp->next;
        }
        printf("Final node timestamp %d:\n", temp->timeStamp);  
        printf("final node starts at %d and ends at %d with memory %d\n", temp->start, temp->end, temp->memchunk);
    }
    printf("Total memory space remaining: %d\n", ms->space_rem);
    
}

void headSwap(sim * ms, process * temp, int largestSpace, Heap * q) {
    ms->head->numSwaps++;
    ms->space_rem += ms->head->memchunk;
    ms->head->timeStamp += ms->numprocs;
    //printf("Remove process with memchunk %d\n", temp->memchunk);
    if (ms->head->numSwaps == 3) {
        printf("This process has been swapped out 3 times and is complete\n"); //Prob free node now
        //printf("Head Node\n");
        process * temp = ms->head;
        ms->head = ms->head->next;
        temp->next = NULL;
        //free(temp);
    } else {
        //printf("This process has %d swaps\n", ms->head->numSwaps);
        insert(q, ms->head);
    }

    if (ms->head->next == NULL) {
        printf("Last node in list\n");
        ms->head = NULL;
        printf("No memory is currently allocated, list is empty\n");
        largestSpace = MEMMAX;
        ms->space_rem = MEMMAX;
    } else {
        //printf("Remove front node and next node becomes head\n");
        temp = ms->head;
        ms->head = ms->head->next; //The new head is the next head
        temp->next = NULL;
        printf("New head of the list has memchunk %d and starts at %d\n", ms->head->memchunk, ms->head->start); 
        // if (ms->head->next != NULL) {
        //     printf("and next in line is %d\n",ms->head->next->memchunk);
        // }
    }
    
    //printf("Space reminaing after swapping out %d\n", ms->space_rem);
}

void nonHeadSwap(sim * ms, process * temp, Heap * q) {
    process * prev = ms->head;
    while (temp->next != NULL) {//The node to be removed is in the middle and not the end of the list
    //printf("Previous node %d and current node %d and next node %d\n", prev->timeStamp, temp->timeStamp, temp->next->timeStamp);
        if (temp->next->timeStamp == ms->lowestTime) {//The next pointer of temp points to the node to be
        //removed

            ms->space_rem += temp->next->memchunk;
            //Remove node and add it back to priority queue
            process * delNode = temp->next; //The node to be removed
            temp->next = delNode->next;
            delNode->next = NULL;
            delNode->numSwaps++;
            printf("Deleting node in middle of list\n");
            //printf("This node now has %d numSwaps\n", delNode->numSwaps);
            delNode->timeStamp += ms->numprocs;
            //printf("The node to be removed has new timestamp %d\n", delNode->timeStamp);
            if (delNode->numSwaps == 3) {
                printf("This process has been swapped out 3 times and is complete\n");
                //free(delNode);
            } else {
                insert(q, delNode);
                printf("SUCCESSFULLY INSERTED BACK INTO QUEUE\n");
            }
        } else {
            prev = temp;
            temp = temp->next;
        }   
    }
}

void printAlloInfo(sim * ms) {
    
    process * temp;
    int numProcs = 1;
    int numHoles = 0;
    float percMem = 0;
    temp = ms->head;
    while (temp->next != NULL) {
        numProcs++;
        if ((temp->next->start - temp->end) > 1) {
            numHoles++;
        }
        temp = temp->next;
    }
    if (MEMMAX - temp->end > 1) {//temp next == NULL but distance between temp and end?
        numHoles++;
    }
    percMem = 100 - (float)ms->space_rem / MEMMAX * 100;
    ms->curNumPIDLoads++;
    if (ms->cumPercMem == 0) {
        ms->cumPercTotal = percMem;
        ms->cumPercMem = percMem;
    } else {
        ms->cumPercTotal = ms->cumPercTotal + percMem;
        printf("cumPercTotal = %.1f\n", ms->cumPercTotal);
        ms->cumPercMem = ms->cumPercTotal / ms->curNumPIDLoads;
    }
    printf("ms->space rem = %d\n", ms->space_rem);
    printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.1f, cumulative %%mem = %.1f\n", numProcs, numHoles, percMem, ms->cumPercMem);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "holes.h"

int numFrees = 0;

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
    process * temp = NULL;
    void (*inserts)(process * node, sim * ms, Heap * q);
    inserts = &insertNode;
    printf("\n***INSERT NODE***\n");
    if (ms->head == NULL) {//LL is empty, insert at head
        insertHead(ms, node);

    } else if (ms->head != NULL) {//The LL is not empty
        temp = ms->head;
        if (ms->head->start - 0 >= node->memchunk) {//Insert before first node
            insertStart(ms, node);
            return;
        }
        while (temp->next != NULL) {
            if (ms->nodeCnt >= 2) {//Enter node into the middle of two if there is space
                int spaceAfter = temp->next->start - temp->end;
                if (spaceAfter >= node->memchunk) {//The space between processes is big enough to accomadate
                    insertMiddle(ms, node, temp);
                    return;//Leave function if space is found between two processes
                } 
            }
            
            temp = temp->next;
        } //Gonna have to modify this to suit the different allocation algorithms

        //printMem(ms);

        int spaceAfter = MEMMAX - temp->end;//The space remaining after the last process in the list
        if (temp->next == NULL && spaceAfter >= node->memchunk) {
            insertEnd(ms, node, temp);
        } else {
            printf("Need to do some swapping to make room\n");
            swapOut(ms, node, q, (*inserts));
        }
    }
    temp = NULL;
}

void insertNodeNext(process * node, sim * ms, Heap * q) {//Node gives the node to be inserted
    process * temp = NULL;
    int space = 0;
    int i = 1;
    void (*inserts)(process * node, sim * ms, Heap * q);
    inserts = &insertNodeNext;
    printf("\n***INSERT NODE NEXT***\n");
    //LL IS EMPTY, INSERT AT HEAD
    if (ms->head == NULL) {
        temp = node;
        ms->mostRecentPid = temp->timeStamp;
        ms->mostRecent = temp;
        printf("Empty list, insert new node with pid %d and memchunk %d\n", temp->timeStamp, temp->memchunk);
        insertHead(ms, node);
        return;
    //LL NOT EMPTY
    } else {
        printf("ms->mostRecent->memchunk = %d\n", ms->mostRecent->memchunk); 
        temp = ms->mostRecent;
        while (temp->next != NULL) {
            space = temp->next->start - temp->end;
            if (space >= node->memchunk) {
                ms->mostRecentPid = node->timeStamp;
                ms->mostRecent = node;
                insertMiddle(ms, node, temp);
                return;
            }
            temp = temp->next;
        }
        if (temp->next == NULL) {//End node or only node
            space = MEMMAX - temp->end;
            if (space >= node->memchunk) {
                ms->mostRecentPid = node->timeStamp;
                ms->mostRecent = node;
                insertEnd(ms, node, temp);
                return;
            }
        }//At this point checked from most recent to end now check from beginning to most recent
        printf("Space not found in middle or end, check before most recent\n");
        if (ms->head->start != 0 && i == 1) {
            printf("Head does not start at 0\n");
            space = ms->head->start - 0;
            if (space >= node->memchunk) {
                ms->mostRecentPid = node->timeStamp;
                ms->mostRecent = node;
                insertStart(ms, node);
                return;
            }
            i = 0;
        }
        if (ms->head->timeStamp != ms->mostRecent->timeStamp) {//Go until most recent and check spaces
            temp = ms->head;
            while (temp->next->timeStamp != ms->mostRecent->timeStamp) {
                space = temp->next->start - temp->end;
                if (space >= node->memchunk) {
                    ms->mostRecentPid = node->timeStamp;
                    ms->mostRecent = node;
                    insertMiddle(ms, node, temp);
                    return;
                }
                temp = temp->next;
            }
            //Check space in between node and most recent
            space = temp->next->start - temp->end;
            if (space >= node->memchunk) {
                ms->mostRecentPid = node->timeStamp;
                ms->mostRecent = node;
                insertMiddle(ms, node, temp);
                return;
            }
        }
    }
    printf("No space found\n");
    swapOut(ms, node, q, (*inserts));
}

void insertNodeBest(process * node, sim * ms, Heap * q) {
    //Allocate the spot that most closely resembles memory to be allocated
    process * temp = NULL;
    int bestSpace = spaceChecker(ms);//Set a little higher then check if theres actually enough space later
    process * befBestSpace = NULL;
    int space = 0;
    int i = 1;
    void (*inserts)(process * node, sim * ms, Heap * q);
    inserts = &insertNodeBest;
    printf("\n***INSERT NODE BEST***\n");
    //LL IS EMPTY, INSERT AT HEAD
    printf("Node to be inserted has memchunk %d\n", node->memchunk);
    if (ms->head == NULL) {
        temp = node;
        ms->mostRecentPid = temp->timeStamp;
        ms->mostRecent = temp;
        printf("Empty list, insert new node with pid %d and memchunk %d\n", temp->timeStamp, temp->memchunk);
        insertHead(ms, node);
        return;
    //LL NOT EMPTY
    } else {//ms->head !=NULL
        printf("Scan LL\n");
        temp = ms->head;
        while (temp->next != NULL) {//Check middle
            space = temp->next->start - temp->end;
            if (space <= bestSpace && space > 1 && space >= node->memchunk) {
                bestSpace = space;
                printf("Best space is in the middle= %d\n", bestSpace);
                befBestSpace = temp;//Set the pointer to the node before best space
                printf("befBestSpace has memchunk %d\n", temp->memchunk);
            }
            temp = temp->next;
        }
        if (temp->next == NULL) {//Check end
            space = MEMMAX - temp->end;
            if (space <= bestSpace && space > 1 && space >= node->memchunk) {
                bestSpace = space;
                printf("Best space is at the end = %d\n", bestSpace);
                befBestSpace = temp;
            }
        }
        printf("Before any inserts\n");
        printf("Best space = %d\n", bestSpace);
        printf("ms->head->start = %d and i = %d\n", ms->head->start, i);
        if (ms->head->start != 0 && i == 1) {//Check before head node
            space = ms->head->start - 0;
            printf("Space = %d and best space = %d\n", space, bestSpace);
            if (space <= bestSpace && space >= node->memchunk) {
                printf("Insert before head node\n");
                insertStart(ms, node); //Checked all other cases so if this is best place there
                return;
            }
            i = 0;
        }
        printf("Space is big enough\n");
        if (bestSpace >= node->memchunk) {//Space is big enough
            printf("bestSpace %d >= node->memchunk %d\n", bestSpace, node->memchunk);
            printf("befBestSpace has memchunk %d\n", befBestSpace->memchunk);
            if (befBestSpace->next == NULL) {//Placement is at the end
                printf("Insert at the end\n");
                insertEnd(ms, node, befBestSpace);
                return;
            } else {
                printf("Insert somewhere in the middle\n");
                insertMiddle(ms, node, befBestSpace);
                return;
            }
            printf("End of if statement\n");
        } 
        printf("No space found\n");
        swapOut(ms, node, q, (*inserts));

    }
}

void insertNodeWorst(process * node, sim * ms, Heap * q) {
    //Worst allocation, largest free block of memory is used
    process * temp = NULL;
    int worstSpace = 0;
    int space = 0;
    process * befWorstSpace = NULL;
    int i = 1;
    void (*inserts)(process * node, sim * ms, Heap * q);
    inserts = &insertNodeWorst;
    printf("\n***INSERT NODE WORST***\n");
    //LL IS EMPTY, INSERT AT HEAD
    if (ms->head == NULL) {
        temp = node;
        printf("Empty list, insert new node with pid %d and memchunk %d\n", temp->timeStamp, temp->memchunk);
        insertHead(ms, node);
        return;
    //LL NOT EMPTY
    } else {
        printf("Scan LL\n");
        temp = ms->head;
        while (temp->next != NULL) {//Check middle
            space = temp->next->start - temp->end;
            if (space > worstSpace && space > 1 && space >= node->memchunk) {
                worstSpace = space;
                befWorstSpace = temp;//Set the pointer to the node before best space
            }
            temp = temp->next;
        }
        if (temp->next == NULL) {//Check end
            space = MEMMAX - temp->end;
            if (space >= worstSpace && space > 1 && space >= node->memchunk) {
                worstSpace = space;
                befWorstSpace = temp;
            }
        }
        printf("Before any inserts\n");
        printf("Worst space = %d\n", worstSpace);
        printf("ms->head->start = %d and i = %d\n", ms->head->start, i);
        if (ms->head->start != 0 && i == 1) {//Check before head node
            space = ms->head->start - 0;
            printf("Space = %d and worst space = %d\n", space, worstSpace);
            if (space >= worstSpace && space >= node->memchunk) {
                printf("Insert before head node\n");
                insertStart(ms, node); //Checked all other cases so if this is best place there
                return;
            }
            i = 0;
        }
        if (worstSpace >= node->memchunk) {//Space is big enough
            if (befWorstSpace->next == NULL) {//Placement is at the end
                printf("Insert at the end\n");
                insertEnd(ms, node, befWorstSpace);
                return;
            } else {
                printf("Insert somewhere in the middle\n");
                insertMiddle(ms, node, befWorstSpace);
                return;
            }
        } 
        printf("No space found\n");
        swapOut(ms, node, q, (*inserts));

    }
}

void swapOut(sim * ms, process * node, Heap * q, void (*inserts)(process * node, sim * ms, Heap * q)) {
    printf("\n***SWAP FUNCTION***\n");

    int largestSpace = spaceChecker(ms);
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
    // insertNode(node, ms, q);
    inserts(node, ms, q);
    
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

void headSwap(sim * ms, process * temp, int largestSpace, Heap * q) {
    ms->head->numSwaps++;
    ms->space_rem += ms->head->memchunk;
    ms->head->timeStamp += ms->numprocs;
    //printf("Remove process with memchunk %d\n", temp->memchunk);
    if (ms->head->numSwaps == 3) {
        printf("This process has been swapped out 3 times and is complete\n"); //Prob free node now
        //printf("Head Node\n");
        process * temp = ms->head;
        if (ms->head->next == NULL) {//Head node
            ms->head = NULL;
            // free(temp);
            numFrees++;
            printf("NODE FREED\n");
            printf("numFrees = %d\n", numFrees);

        } else {
            ms->head = ms->head->next;
            temp->next = NULL;
            temp = NULL;
            // free(temp);
            numFrees++;
            printf("NODE FREED\n");
            printf("numFrees = %d\n", numFrees);
        }
        
    } else {
        insert(q, ms->head);
        if (ms->head->next == NULL) {
            printf("Last node in list\n");
            ms->head = NULL;
            printf("No memory is currently allocated, list is empty\n");
            largestSpace = MEMMAX;
            ms->space_rem = MEMMAX;
        } else {
            printf("Remove front node and next node becomes head\n");
            temp = ms->head;
            ms->head = ms->head->next; //The new head is the next head
            temp->next = NULL;
            printf("New head of the list has memchunk %d and starts at %d\n", ms->head->memchunk, ms->head->start); 
        }
    }
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
                delNode = NULL;
                // free(delNode);
                printf("NODE FREED\n");
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
    float cumPercMem = 0;
    temp = ms->head;
    printf("Node with memchunk %d\n", temp->memchunk);
    while (temp->next != NULL) {
        
        numProcs++;
        if ((temp->next->start - temp->end) > 1) {
            numHoles++;
        }
        temp = temp->next;
    }
    printf("Final node numchunk = %d\n", temp->memchunk);
    if (MEMMAX - temp->end > 1) {//temp next == NULL but distance between temp and end?
        numHoles++;
    }
    percMem = 100 - (float)ms->space_rem / MEMMAX * 100;
    ms->curNumPIDLoads++;
    ms->cumPercTotal = ms->cumPercTotal + percMem;
    printf("cumPercTotal = %.1f\n", ms->cumPercTotal);
    cumPercMem = ms->cumPercTotal / (float)ms->curNumPIDLoads;
    ms->avgProcsTot = ms->avgProcsTot + numProcs;
    ms->avgHolesTot = ms->avgHolesTot + numHoles;
    printf("ms->space rem = %d\n", ms->space_rem);
    printf("pid loaded, #processes = %d, holes = %d, %%memusage = %.1f, cumulative %%mem = %.1f\n", numProcs, numHoles, percMem, cumPercMem);
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

void printSummary(sim * ms) {
    float avgProcs = 0;
    float avgHoles = 0;
    avgProcs = ms->avgProcsTot / ms->curNumPIDLoads;
    avgHoles = ms->avgHolesTot / ms->curNumPIDLoads;
    printf("Total loads = %d\n", ms->curNumPIDLoads);
    printf(", average processes = %.1f, average #holes = %.1f, ", avgProcs, avgHoles);
    printf("cumulative %%mem = %.1f\n", ms->cumPercTotal / ms->curNumPIDLoads);
}

void insertHead(sim * ms, process * node) {
    ms->head = node;
    ms->head->next = NULL;
    ms->head->start = 0;
    ms->head->end = node->memchunk;
    ms->space_rem -= node->memchunk;
    printAlloInfo(ms);
    ms->nodeCnt++;
}

void insertStart(sim * ms, process * node) {
    node->next = ms->head;//place node at start of list
    ms->head = node;
    ms->head->start = 0;
    ms->head->end = ms->head->memchunk;
    ms->space_rem -= ms->head->memchunk;
    printAlloInfo(ms);
    ms->nodeCnt++;
}

void insertMiddle(sim * ms, process * node, process * temp) {
    node->next = temp->next;
    temp->next = node;
    ms->space_rem -= node->memchunk;
    ms->nodeCnt++;
    node->start = temp->end + 1;
    node->end = node->start + node->memchunk;
    printAlloInfo(ms);
}

void insertEnd(sim * ms, process * node, process * temp) {
    temp->next = node;
    temp->next->next = NULL;
    ms->space_rem -= node->memchunk;
    ms->nodeCnt++;
    node->start = temp->end + 1;
    node->end = node->start + node->memchunk;
    printAlloInfo(ms);
}
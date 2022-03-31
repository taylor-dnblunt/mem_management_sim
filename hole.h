//Taylor Blunt 0793684

#define MEMMAX 1024

typedef struct process {
    int pid;
    int timeStamp;
    int memchunk;
    int start;
    int end;
    int numSwaps;
    struct process * next;
} process;

typedef struct proc_list {
    int numProcs;
    struct process * proc_arr;
} proc_list;

struct Heap{
    process **arr;
    int count;
    int capacity;
    int heap_type; // for min heap , 1 for max heap
};
typedef struct Heap Heap;

typedef struct sim {
    int space_rem; //Remaining space for allocation
    int nodeCnt;
    int lowestTime;//For swapping out
    int numprocs;
    float avgProcsTot;
    float avgHolesTot;
    float cumPercTotal;
    int curNumPIDLoads;
    int mostRecentPid;
    process * mostRecent;
    process * head;
} sim;

int inputChecker(int argc, char * argv[]);
void insertNode(process * node, sim * ms, Heap * q);
void swapOut(sim * ms, process * node, Heap * q, void (*inserts)(process * node, sim * ms, Heap * q));
int spaceChecker(sim * ms);
void timeStampCheck(sim * ms);
void printMem(sim * ms);
void headSwap(sim * ms, process * temp, int largestSpace, Heap * q);
void nonHeadSwap(sim * ms, process * temp, Heap * q);
void printAlloInfo(sim * ms);
void printSummary(sim * ms);
void insertHead(sim * ms, process * node);
void insertStart(sim * ms, process * node);
void insertMiddle(sim * ms, process * node, process * temp);
void insertEnd(sim * ms, process * node, process * temp);
void insertNodeNext(process * node, sim * ms, Heap * q);
int cycleCheck(sim* ms, process * node, process * temp, int space);
void insertNodeBest(process * node, sim * ms, Heap * q);
void insertNodeWorst(process * node, sim * ms, Heap * q);

Heap *CreateHeap(int capacity,int heap_type);
void insert(Heap *h, process * thread);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
process * PopMin(Heap *h);
void print(Heap *h);
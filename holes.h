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
    int lowestTime;
    process * head;
} sim;

int inputChecker(int argc, char * argv[]);
void insertNode(process * node, sim * ms, Heap * q);
void swapOut(sim * ms, process * node, Heap * q);
int spaceChecker(sim * ms);
void timeStampCheck(sim * ms);

Heap *CreateHeap(int capacity,int heap_type);
void insert(Heap *h, process * thread);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
process * PopMin(Heap *h);
void print(Heap *h);
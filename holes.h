//Taylor Blunt 0793684

#define MEMMAX 1024

typedef struct process {
    int pid;
    int memchunk;
    int numProcs;
    struct process * proc_arr;
} process;

struct Heap{
    process *arr;
    int count;
    int capacity;
    int heap_type; // for min heap , 1 for max heap
};
typedef struct Heap Heap;

int inputChecker(int argc, char * argv[]);

Heap *CreateHeap(int capacity,int heap_type);
void insert(Heap *h, process thread);
void heapify_bottom_top(Heap *h,int index);
void heapify_top_bottom(Heap *h, int parent_node);
process PopMin(Heap *h);
void print(Heap *h);
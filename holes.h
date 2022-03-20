//Taylor Blunt 0793684

#define MEMMAX 1024

typedef struct process {
    int pid;
    int memchunk;
    int numProcs;
    struct process * proc_arr;
} process;

int inputChecker(int argc, char * argv[]);
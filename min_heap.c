#include<stdio.h>
#include<stdlib.h>
#include"holes.h"


//heap_type = 0 for min heap and 1 for max heap
Heap *CreateHeap(int capacity,int heap_type){
    Heap *h = (Heap * ) malloc(sizeof(Heap)); //one is number of heap

    //check if memory allocation is fails
    /*if(h == NULL){
        printf("Memory Error!");
        return;
    }*/
    h->heap_type = heap_type;
    h->count=0;
    h->capacity = capacity;
    //make the array an array of threads to be sorted by arrival time
    h->arr = (process **) malloc(capacity*sizeof(process *)); //size in bytes

    //check if allocation succeed
    /*if ( h->arr == NULL){
        printf("Memory Error!");
        return;
    }*/
    return h;
}

//Want to insert the thread
void insert(Heap *h, process * thread){
    if( h->count < h->capacity){
        h->arr[h->count] = thread;
        heapify_bottom_top(h, h->count);
        h->count++;
    }
}

void heapify_bottom_top(Heap *h,int index){//What is index in this case?
    process * temp;
    int parent_node = (index-1)/2;

    if(h->arr[parent_node]->timeStamp > h->arr[index]->timeStamp){
        //swap and recursive call
        temp = h->arr[parent_node];
        h->arr[parent_node] = h->arr[index];
        h->arr[index] = temp;
        heapify_bottom_top(h,parent_node);
    }
}

void heapify_top_bottom(Heap *h, int parent_node){
    int left = parent_node*2+1;
    int right = parent_node*2+2;
    int min;
    process * temp;

    if(left >= h->count || left <0) {
        left = -1;
    }
        
    if(right >= h->count || right <0){
        right = -1;
    }
    if(left != -1 && h->arr[left]->timeStamp < h->arr[parent_node]->timeStamp) {
        min=left;
    } else {
        min =parent_node;
    }
        
    if(right != -1 && h->arr[right]->timeStamp < h->arr[min]->timeStamp) {
        min = right;
    }
        

    if(min != parent_node){
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        // recursive  call
        heapify_top_bottom(h, min);
    }
}

process * PopMin(Heap *h){
    process * pop;
    if(h->count==0){
        printf("\n__Heap is Empty__\n");
        
    } else {
    // replace first node by last and delete last
        pop = h->arr[0];
        h->arr[0] = h->arr[h->count-1];
        h->count--;
        heapify_top_bottom(h, 0);
        return pop;
    }
    
}
void print(Heap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->count;i++){
        printf("-> %d ",h->arr[i]->pid);
    }
    printf("->__/\\__\n");
}
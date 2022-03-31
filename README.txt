//Taylor Blunt 0793684
//Assignment 3 Memory Management

The main files for the assignment are hole.c, hole_func.c, hole.h, min_heap.c, README.txt, and the makefile

The data structure used for the assignment is a singly-linked list. The struct sim is used to look after summary
info and contains the head of the list

The data for the processes are contained in the struct process. Timestamp is used to keep track of the priorities
when swapping processes in and out of "memory".

hole.c contains the main function and deals with taking the input file, setting the right flags for which
memory allocation strategy is to be used, setting up the sim and then running it based on the flags, and finally
freeing up memory at the end.

hole_func.c contains all the helper functions used in holes.c. Each strategy has its own insert function:
insertNode(first), insertNodeNext(next), insertNodeBest, and insertNodeWorst. Timestamp checker checks 
the timestamps of each process in memory to determine which one has been in the longest for swapping. headSwap
and nonHeadSwap do the swapping of the processes out of memory and continues until the space checker function
says there is enough room for the next process. printAlloInfo prints the info when a process is inserted and
printSummary prints the final summary.

hole.h contains the structs as well as the function prototypes.

min_heap.c contains the code for a min heap found on the internet that was modified to work with this program.
https://gist.github.com/sudhanshuptl/d86da25da46aa3d060e7be876bbdb343

All memory allocation strategies run and there are no memory leaks.
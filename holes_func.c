#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

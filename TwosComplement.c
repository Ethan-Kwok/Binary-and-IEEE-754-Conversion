#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void toBinary (int* binaryArr, long decimal, int numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        if (pow(2, i) <= decimal) {
            binaryArr[i] = 1;
            decimal -= pow(2, i);
        }
    }
    //printBinaryArr(binaryArr, numBits);
}


void invert(int* binaryArr, int numBits) {
    for (int i = 0; i < numBits; i++) {
        if (binaryArr[i] == 0) binaryArr[i] = 1;
        else if (binaryArr[i] == 1) binaryArr[i] = 0;
    }
}


void addOne(int* binaryArr, int numBits) {
    int i = 0;
    while (i < numBits && binaryArr[i] == 1) {
        binaryArr[i] = 0;
        i++;
    }
    if (i < numBits) {
        binaryArr[i] = 1;
    }
}


void printBinaryArr(int* binaryArr, int numBits) {
    for (int i = numBits - 1; i >= 0; i--) {
        printf("%d",binaryArr[i]);
    }
    printf("\n");
}


int main(int argc, char** argv) {
    
    FILE *fp;
    fp = fopen(argv[1], "r");

    if(fp == NULL) {
        printf("error\n");
        exit(0);
        return 0;
    }

    long decimal = 0;
    int numBits = 0;

    while(fscanf(fp, "%ld %d", &decimal, &numBits) != EOF) {
        int* binaryArr;
        binaryArr = malloc(numBits * sizeof(int));
        for (int i = 0; i < numBits; i++) {
            binaryArr[i] = 0;
        }

        if (decimal > pow(2, numBits - 1) - 1) {
            //binary print pow(2, numBits) - 1
            toBinary(binaryArr, (pow(2, numBits - 1) - 1), numBits);
            printBinaryArr(binaryArr, numBits);
        }
        else if (decimal < -1 * (pow(2, numBits - 1))) {
            // binary print -1 * pow(2, numBits)
            toBinary(binaryArr, -1 * (-1 * pow(2, numBits - 1)), numBits);
            invert(binaryArr, numBits);
            addOne(binaryArr, numBits);
            printBinaryArr(binaryArr, numBits);
        }
        else if (decimal >= 0) {
            toBinary(binaryArr, decimal, numBits);
            printBinaryArr(binaryArr, numBits);
        }
        else if (decimal < 0) {
            toBinary(binaryArr, -1 * decimal, numBits);
            invert(binaryArr, numBits);
            addOne(binaryArr, numBits);
            printBinaryArr(binaryArr, numBits);
        }

        free(binaryArr);
    }

    return EXIT_SUCCESS;

}
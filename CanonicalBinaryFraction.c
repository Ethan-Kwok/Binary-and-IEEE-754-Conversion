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
}


void toBinaryFraction (int* binaryArr, double fraction, int numBits) {
    for (int i = 0; i < numBits; i++) {
        int exponent = -1 * (i + 1);
        if (pow(2, exponent) <= fraction) {
            binaryArr[i] = 1;
            fraction -= pow(2, exponent);
        }
    }
}


void printArr(int* wholeArr, int wholeBits, int* fractionArr, int fractionBits, int shift) {
    if (wholeBits != 0) {
        printf("%d.", wholeArr[wholeBits - 1]);
        for (int i = wholeBits - 2; i >= 0; i--) {
            printf("%d", wholeArr[i]);
        }
        for (int i = 0; i < fractionBits; i++) {
            printf("%d", fractionArr[i]);
        }
        printf(" %d\n", wholeBits - 1 + shift);
    }
    else {
        printf("%d.", fractionArr[0]);
        for (int i = 1; i < fractionBits; i++) {
            printf("%d", fractionArr[i]);
        }
        printf(" %d\n", -1 * (shift + 1));
    }
}


int main(int argc, char** argv) {
    
    FILE *fp;
    fp = fopen(argv[1], "r");

    if(fp == NULL) {
        printf("error\n");
        exit(0);
        return 0;
    }

    double decimal = 0;
    int numDig = 0;
    double whole = 0;
    double fraction = 0;
    int wholeBits = 0;
    int fractionBits = 0;
    int shift = 0;


    while(fscanf(fp, "%lf %d", &decimal, &numDig) != EOF) {
        shift = 0;
        whole = floor(decimal);
        for (wholeBits = 0; pow(2, wholeBits) - 1 < whole; wholeBits++);
        if (wholeBits > numDig) {
            while (wholeBits > numDig + 1) {
                whole /= 2;
                wholeBits--;
                shift++;
            }
        }
        
        int* wholeArr;
        wholeArr = malloc((wholeBits) * sizeof(int));
        for (int i = 0; i < wholeBits; i++) {
            wholeArr[i] = 0;
        }
        toBinary(wholeArr, whole, wholeBits);


        fraction = decimal - whole;
        fractionBits = numDig - (wholeBits - 1);
        if (wholeBits == 0) {
            while (fraction < 0.5) {
                fraction *= 2;
                shift++;
            }
            fractionBits = numDig + 1;
        }
        
        int* fractionArr;
        fractionArr = malloc((fractionBits) * sizeof(int));
        for (int i = 0; i < fractionBits; i++) {
            fractionArr[i] = 0;
        }
        toBinaryFraction(fractionArr, fraction, fractionBits);


        printArr(wholeArr, wholeBits, fractionArr, fractionBits, shift);
        free(wholeArr);
        free(fractionArr);
    }

    return EXIT_SUCCESS;

}
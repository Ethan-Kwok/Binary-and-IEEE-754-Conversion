#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

void toBinary (int* binaryArr, unsigned int decimal, int numBits) {
    //truncate
    if (decimal > pow(2, numBits) - 1) {
        int max = 0;
        while (decimal > pow(2, max)) max++;
        for (int i = max; i >= numBits; i--) {
            if (decimal <= pow(2, numBits) - 1) break;
            if (decimal - pow(2, i) > 0) {
                decimal -= pow(2, i);
            }
        }
    }


    //actually convert
    for (int i = numBits - 1; i >= 0; i--) {
        if (pow(2, i) <= decimal) {
            binaryArr[numBits - i - 1] = 1;
            decimal -= pow(2, i);
        }
    }
    if (decimal > 0) {
        for (int i = 0; i < numBits; i++) {
            binaryArr[i] = 0;
        }
        toBinary(binaryArr, decimal, numBits);
    }
}


int toDecimal (int* binaryArr, int expBits) {
    int decimal = 0;
    int j = 0;
    for (int i = expBits; i > 0; i--) {
        if (binaryArr[i] == 1) {
            decimal += pow(2, j);
        }
        j++;
    }
    return decimal;
}


double mantisaToDecimal (int* mantisaArr, int mantisaBits) {
    double decimal = 0;
    int j = -1;
    for (int i = 0; i < mantisaBits; i++) {
        if (mantisaArr[i] == 1) {
            decimal += pow(2, j);
        }
        j--;
    }
    
    return decimal;
}


int main(int argc, char** argv) {
    
    FILE *fp;
    fp = fopen(argv[1], "r");

    if(fp == NULL) {
        printf("error\n");
        exit(0);
        return 0;
    }

    unsigned int decimal = 0;
    int numDig = 0;
    int shift = 0;
    int expBits = 0;
    int mantisaBits = 0;
    int bias = 0;
    int precisionBits = 0;
    int expDecimal = 0;
    double result;


    while(fscanf(fp, "%d %d %d %x %d", &numDig, &expBits, &mantisaBits, &decimal, &precisionBits) != EOF) {
        
        //convert hex to IEEE-FP
        int* fpBinary;
        fpBinary = malloc((numDig) * sizeof(int));
        for (int i = 0; i < numDig; i++) {
            fpBinary[i] = 0;
        }
        toBinary(fpBinary, decimal, numDig);


        //find exponent 2^E from exponent part
        expDecimal = toDecimal(fpBinary, expBits);
        bias = pow(2, (expBits - 1)) - 1;
        shift = expDecimal - bias;


        //convert mantisa to decimal and multiply by 2^E
        int* mantisaBinary;
        mantisaBinary = malloc(mantisaBits * sizeof(int));
        for (int i = 0; i < mantisaBits; i++) {
            mantisaBinary[i] = fpBinary[expBits + 1 + i];
        }
        double mantisaDecimal = mantisaToDecimal(mantisaBinary, mantisaBits);
        //if normalized (exponent part != 0), add 1 to value.
        if (expDecimal != 0) {
            mantisaDecimal += 1;
            result = mantisaDecimal * pow(2, shift);
        }
        else { //if denormalized, shift + 1 because the assumed 1 is not there
            result = mantisaDecimal * pow(2, shift + 1);
        }
        if (fpBinary[0] == 1) result *= -1;


        printf("%.*f\n", precisionBits, result);
        free(fpBinary);
        free(mantisaBinary);

    }

    return EXIT_SUCCESS;

}
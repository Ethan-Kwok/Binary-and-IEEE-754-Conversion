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


/*void printArr(int* wholeArr, int wholeBits, int* fractionArr, int fractionBits, int shift) {
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
}*/


void printExponent(int* expArr, int expBits) {
    if (expBits != 0) {
        for (int i = expBits - 1; i >= 0; i--) {
            printf("%d", expArr[i]);
        }
    }
}


void printMantisa(int* mantisaArr, int mantisaBits) {
    if (mantisaBits != 0) {
        for (int i = 0; i < mantisaBits; i++) {
            printf("%d", mantisaArr[i]);
        }
    }
}


void setMantisaArr(int* wholeArr, int wholeBits, int* fractionArr, int fractionBits, int exponent, int* mantisaArr, int mantisaArrLen) {
    if (wholeBits == 0 && exponent == 0) {
        int j = 0;
        for (int i = 0; i < fractionBits - 1; i++) {
            mantisaArr[j] = fractionArr[i];
            j++;
        }
    } 
    if (wholeBits == 0 && exponent != 0) {
        int j = 0;
        for (int i = 1; i < fractionBits; i++) {
            mantisaArr[j] = fractionArr[i];
          j++;
        }
    } 
    else if (wholeBits != 0) {
        int j = 0;
        for (int i = wholeBits - 2; i >= 0; i--) {
            mantisaArr[j] = wholeArr[i];
            j++;
        }
        for (int i = 0; i < fractionBits; i++) {
            mantisaArr[j] = fractionArr[i];
            j++;
        }
    }
}


void printMantisaArr(int* mantisaArr, int mantisaArrLen) {
    for (int i = 0; i < mantisaArrLen; i++) {
        printf("%d", mantisaArr[i]);
    }
}


void addToExp(int* expArr, int expBits) {
    for (int i = expBits - 1; i >= 0; i--) {
        if (expArr[i] == 0) {
            expArr[i] = 1;
            break;
        }
        else {
            expArr[i] = 0;
        }
    }
}


void roundArr(int* mantisaArr, int mantisaArrLen, int roundTo, int* expArr, int expBits) {
    int isAddToExp = 0; //bool for edge case that requires the exponent to be added by 1 bruh
    //note expArr and expBits are both for this edge case only bruh

    int* roundDown;
    roundDown = malloc(mantisaArrLen * sizeof(int));
    for (int i = 0; i < mantisaArrLen; i++) {
        roundDown[i] = 0;
    }
    for (int i = 0; i < roundTo; i++) {
        roundDown[i] = mantisaArr[i];
    }

    int* roundUp;
    roundUp = malloc(mantisaArrLen * sizeof(int));
    for (int i = 0; i < mantisaArrLen; i++) {
        roundUp[i] = roundDown[i];
    }
    //add one at index roundTo-1 to roundDown for roundUp
    for (int i = roundTo - 1; i >= 0; i--) {
        if (roundUp[i] == 0) {
            roundUp[i] = 1;
            break;
        }
        else {
            if (i == 0) isAddToExp = 1;
            roundUp[i] = 0;
        }
    }

    //choose which to round to
    if (mantisaArr[roundTo] == 0) {
        for (int i = 0; i < mantisaArrLen; i++) {
            mantisaArr[i] = roundDown[i];
        }
    }
    else {
        int i = roundTo + 1; //I dont think roundTo + 1 can be greater than mantisaArrLen
        while (i < mantisaArrLen) { //if at any point after mantisaArr[roundTo] there's a 1, round up (no longer tie)
            if (mantisaArr[i] == 1) {
                for (int i = 0; i < mantisaArrLen; i++) {
                    mantisaArr[i] = roundUp[i];
                }
                if (isAddToExp == 1) addToExp(expArr, expBits);
                break;
            }

            else if (i == mantisaArrLen - 1) { //if there's no 1 after mantisaArr[roundTo], the following does the tie rule
                if (roundDown[roundTo - 1] == 0) {
                    for (int i = 0; i < mantisaArrLen; i++) {
                        mantisaArr[i] = roundDown[i];
                    }
                }
                else {
                    for (int i = 0; i < mantisaArrLen; i++) {
                        mantisaArr[i] = roundUp[i];
                        if (isAddToExp == 1) addToExp(expArr, expBits);
                    }
                }
            }

            i++;
        }
    }

    free(roundUp);
    free(roundDown);
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
    int expBits = 0;
    int exponent = 0;
    int mantisaBits = 0;
    int sign = 0;
    int bias = 0;


    while(fscanf(fp, "%lf %d %d %d", &decimal, &numDig, &expBits, &mantisaBits) != EOF) {
        shift = 0;
        sign = 0;
        if (decimal < 0) {
            sign = 1;
            decimal *= -1;
        }

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


        int* expArr;
        expArr = malloc((expBits) * sizeof(int));
        for (int i = 0; i < expBits; i++) {
            expArr[i] = 0;
        }
        bias = pow(2, (expBits - 1)) - 1;
        if (wholeBits != 0) exponent = wholeBits - 1 + shift + bias;
        else exponent = -1 * (shift + 1) + bias;
        toBinary(expArr, exponent, expBits);


        int* mantisaArr;
        mantisaArr = malloc((numDig) * sizeof(int));
        for (int i = 0; i < numDig; i++) {
            mantisaArr[i] = 0;
        }
        bias = pow(2, (expBits - 1)) - 1;
        if (wholeBits != 0) exponent = wholeBits - 1 + shift + bias;
        else exponent = -1 * (shift + 1) + bias;
        setMantisaArr(wholeArr, wholeBits, fractionArr, fractionBits, exponent, mantisaArr, numDig);
        roundArr(mantisaArr, numDig, mantisaBits, expArr, expBits);

        /*printf("\ntest case: %lf", decimal);
        printf("\nsignBit: %d", sign);
        printf("\nexponentArr: ");
        printExponent(expArr, expBits);
        printf("\nprintArr: ");
        printArr(wholeArr, wholeBits, fractionArr, fractionBits, shift);
        printf("mantisaArr: ");
        printMantisaArr(mantisaArr, numDig);
        printf("\nroundedMantisaArr: ");
        printMantisaArr(mantisaArr, numDig);

        printf("\nfinal: ");*/

        printf("%d", sign);
        printExponent(expArr, expBits);
        printMantisa(mantisaArr, mantisaBits);   
        printf("\n");
        
        free(wholeArr);
        free(fractionArr);
        free(expArr);
        free(mantisaArr);
    }

    return EXIT_SUCCESS;

}
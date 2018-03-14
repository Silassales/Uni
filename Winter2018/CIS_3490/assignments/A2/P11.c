/*
Silas Sales
0922973
2/12/2018
A2
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

/* fills array from file*/
void fillArrayFromFile(FILE * input_file, int * array) {
    int i = 0, return_val;

    if(input_file == NULL) {
        return;
    }

    while(i < 50000 && (return_val = fscanf(input_file, "%d", &array[i++])) == 1);


    if(return_val == 0) {
        printf("The file contained something other than integers.\n");
        return;
    }
}

/* determines inversion through brute force*/
int bruteForceInversions(int * array) {
    int num_inversions = 0;
    int i,j;

    for (i = 0; i  < 50000;i++){
        for(j = i; j < 50000;j++){
            if(array[i] > array[j]) {
                num_inversions++;
            }
        }
    }

    return num_inversions;
}

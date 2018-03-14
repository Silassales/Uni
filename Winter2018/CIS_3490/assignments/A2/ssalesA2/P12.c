/*
Silas Sales
0922973
2/12/2018
A2
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

/*merges the left-right arrays and counts inversions*/
int merge(int * array, int * temp_array, int left, int mid, int right) {
    int i = left, j = mid, h = left, inversions = 0;

    while((i < mid) && (j <= right)) {
        if (array[i] <= array[j]) {
            temp_array[h] = array[i];
            h++; i++;
        } else {
            temp_array[h] = array[j];
            h++;
            j++;

            inversions += mid - i;
        }
    }

    // copy the remaining values to the temp_array
    for(i = i; i <= mid - 1; i++){
        temp_array[h] = array[i];
        h++;
    }

    // copy the remaining values to the temp_array
    for(j = j; j <= right; j++){
        temp_array[h] = array[j];
        h++;
    }

    // update the array
    for(i = left; i <= right; i++){
        array[i] = temp_array[i];
    }

    return inversions;
}

// recursively sorts the input array 
// returns the number of inversions in the array
// inversions are calculated by left + right + while merging
// uses merge() to count inversions that happen during merging
int mergeSort(int * array, int * temp_array, int left, int right) {
    int mid, inversions;
    inversions = 0;

    if(left < right) {
        mid = (left + right) / 2;
    
        // inversions of the left portion
        inversions += mergeSort(array, temp_array, left, mid);
        // inversions of the right portion
        inversions += mergeSort(array, temp_array, mid+1, right);
        // inversions of the merging of the left and right portion
        inversions += merge(array, temp_array, left, mid+1, right);  
    }

    return inversions;
}
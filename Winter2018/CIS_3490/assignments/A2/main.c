/*
Silas Sales
0922973
2/12/2018
A2
*/

#include "P11.c"
#include "P12.c"
#include "P21.c"
#include "P22.c"
#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>


int main() {
    FILE * input;
    int array[50000], temp_array[50000];
    int num_inversions = 0;
    struct timeb t_start, t_current;
    int t_dif;

    input = fopen("data_1.txt", "r");

    if(input == NULL) {
        printf("Error reading file.\n");
        return 1;
    }

    fillArrayFromFile(input, array);

    ftime(&t_start);
    num_inversions = bruteForceInversions(array);
    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Number of inversions: %d\n", num_inversions);
    printf("Brute Force execution time (milliseconds): %d\n", t_dif);

    fillArrayFromFile(input, array);

    ftime(&t_start);
    num_inversions = mergeSort(array, temp_array, 0, 49999);
    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Number of inversions: %d\n", num_inversions);
    printf("Merge sort execution time (milliseconds): %d\n", t_dif);

    double x_values[INPUT_SIZE], y_values[INPUT_SIZE], convex_hull_x[INPUT_SIZE], convex_hull_y[INPUT_SIZE];

    input = fopen("data_2.txt", "r");

    if(input == NULL) {
        printf("Error reading file.\n");
        return 1;
    }

    fillArraysFromFile2(input, x_values, y_values, convex_hull_x, convex_hull_y);

    ftime(&t_start);

    bruteForceConvexHull(x_values, y_values, convex_hull_x, convex_hull_y);

    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printHull(stdout, convex_hull_x, convex_hull_y);

    printf("Time Dif: %d\n", t_dif);

    fillArraysFromFile(input, x_values, y_values, convex_hull_x, convex_hull_y);

    ftime(&t_start);

    int size_of_hull = hull(INPUT_SIZE, x_values, y_values, convex_hull_x, convex_hull_y);

    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Time Dif: %d\n", t_dif);

    printHull2(stdout, size_of_hull, convex_hull_x, convex_hull_y);

    fclose(input);
    return 0;
}
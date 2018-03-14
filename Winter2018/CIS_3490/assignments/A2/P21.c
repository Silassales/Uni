/*
Silas Sales
0922973
2/12/2018
A2
*/
#define INPUT_SIZE 30000

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>

/*displays the hull to the stream provided*/
void printHull(FILE * fstream, double * convex_hull_x, double * convex_hull_y) {
    int i = 0;
    printf("Convex hull line segements:\n");
    while(i < INPUT_SIZE && (fabs(convex_hull_x[i] + 1.0) > 0.0000001)) {
        printf("Point %d: (%lf,%lf)\n", i+1, convex_hull_x[i], convex_hull_y[i]);
        i++;
    }
}

/*fills arrays from file*/
void fillArraysFromFile2(FILE * input_file, double * x_values, double * y_values, double * convex_hull_x, double *convex_hull_y) {
    int i = 0, return_val = 1;

    if(input_file == NULL) {
        return;
    }

    while(i < INPUT_SIZE && return_val == 1) {
        return_val = fscanf(input_file, "%lf", &x_values[i]);
        return_val = fscanf(input_file, "%lf", &y_values[i]);
        i++;
    }

    for(i = 0; i < INPUT_SIZE;i++) {
        convex_hull_x[i] = -1.0;
        convex_hull_y[i] = -1.0;
    }
}

/*determines hull from brute force*/
void bruteForceConvexHull(double * x_values, double * y_values, double * convex_hull_x, double *convex_hull_y) {
    int i, j, k, m = 0, s;
    double a,b,c;
    int previous_direction = 0;
    int direction = 0;
    int part_of_hull = 0, one_in_hull = 0, two_in_hull = 0;

    for (i = 0; i < INPUT_SIZE;i++){
        for(j = i;j < INPUT_SIZE;j++){
            if(i==j) continue;
            a = y_values[j] - y_values[i];
            b = x_values[i] - x_values[j];
            c = (x_values[i]*y_values[j] - x_values[j]*y_values[i]);
            
            part_of_hull = 1;
            k = 0;
            previous_direction = 0;
            direction = 0;
            while(part_of_hull && k < INPUT_SIZE) {
                if (k==j || k ==i){
                    k++;
                    continue;
                }

                if ((a*x_values[k] + b*y_values[k]) > c) { // above
                    previous_direction = direction;
                    direction = 1;
                } else if ((a*x_values[k] + b*y_values[k]) < c) { // below
                    previous_direction = direction;
                    direction = -1;
                }   else { // on the line
                    previous_direction = direction;
                }

                if (((direction == -1) && (previous_direction == 1)) || ((direction == 1) && (previous_direction == -1))) { // not part of the convex hull
                    part_of_hull = 0;
                }   

                k++;
            }

            if(part_of_hull) {

                s=0;
                one_in_hull = 0;
                two_in_hull = 0;
                while(s < INPUT_SIZE && (fabs(convex_hull_x[s] + 1.0) > 0.0000001)) {
                    if(convex_hull_x[s] == x_values[i] && convex_hull_y[s] == y_values[i]){
                        one_in_hull = 1;
                    } else if(convex_hull_x[s] == x_values[j] && convex_hull_y[s] == y_values[j]) {
                        two_in_hull = 1;
                    }
                    s++;
                }

                if(!one_in_hull){
                    convex_hull_x[m] = x_values[i];
                    convex_hull_y[m] = y_values[i];
                    m++;
                } else if(!two_in_hull) {
                    convex_hull_x[m] = x_values[j];
                    convex_hull_y[m] = y_values[j];
                    m++;
                }
            }
        }
    }
}
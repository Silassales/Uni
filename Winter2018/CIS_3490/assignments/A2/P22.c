#define INPUT_SIZE 30000 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/timeb.h>

typedef struct {
    double x;
    double y;
} point;

/*prints the hull to the stream provided*/
void printHull2(FILE * fstream, int size, double * convex_hull_x, double * convex_hull_y) {
    int i = 0;
    printf("Convex hull line segements:\n");
    while(i < size && ((int)convex_hull_x[i] != 0)) {
        printf("Point %d: (%lf,%lf)\n", i+1, convex_hull_x[i], convex_hull_y[i]);
        i++;
    }
}

/*fills arrays from file*/
void fillArraysFromFile(FILE * input_file, double * x_values, double * y_values, double * convex_hull_x, double *convex_hull_y) {
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
        convex_hull_x[i] = 0;
        convex_hull_y[i] = 0;
    }
}

/*comp for clockwise sorting*/
int comp (const void * f, const void * s) {
    point * a = (point*)f;
    point * b = (point*)s;
    
    if (a->x >= 0 && b->x < 0) return 0;
    if(a->x < 0 && b->x >= 0) return 1;
    if((int)a->x == 0 && (int)b->x == 0) {
        if(a->y >= 0 || b->y >= 0) return a->y > b->y;
        return b->y < a->y;
    }

    int det = (a->x) * (b->y) - (b->x) * (a->y);
    if(det < 0) return 0;
    if(det > 0) return 1;

    int d1 = (a->x) * (a->x) + (a->y) * (a->y);
    int d2 = (b->x) * (b->x) + (b->y) * (b->y);
    return d1 < d2;
}

/*determines hull with brute force, used for sets < 5*/
void bruteForceConvexHull2(int size, double * x_values, double * y_values, double * convex_hull_x, double *convex_hull_y) {
    int i, j, k, m = 0, s;
    double a,b,c;
    int previous_direction = 0;
    int direction = 0;
    int part_of_hull = 0, one_in_hull = 0, two_in_hull = 0;

    for (i = 0; i < size;i++){
        for(j = i;j < size;j++){
            if(i==j) continue;
            a = y_values[j] - y_values[i];
            b = x_values[i] - x_values[j];
            c = (x_values[i]*y_values[j] - x_values[j]*y_values[i]);
            
            part_of_hull = 1;
            k = 0;
            previous_direction = 0;
            direction = 0;
            while(part_of_hull && k < size) {
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
                while(s < size && ((int)convex_hull_x[s] != 0)) {
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

    // sort counter-clockwise
    point points[m];
    for(i = 0; i < m;i++){
        points[i].x = convex_hull_x[i];
        points[i].y = convex_hull_y[i];
    }

    qsort(points, m, sizeof(point), comp);

    for(i = 0; i < m;i++){
        convex_hull_x[i] = points[i].x;
        convex_hull_y[i] = points[i].y;
    }
}

// counts elements up till a 0 or up until INPUT_SIZE
int getSize(double * array) {
    int i = 0;
    int size = sizeof(array) / sizeof(array[0]);

    while(i < size && ((int)array[i] != 0)) i++;
    
    return i;
}

/*determines if the points pass through the hull or not*/
int orientation(double a_x, double a_y, double b_x, double b_y, double c_x, double c_y) {
    int result = (b_y-a_y)*(c_x-b_x) - (c_y-b_y)*(b_x-a_x);

    if(result == 0) return 0;
    if(result > 0) return 1;
    return -1;
}

// returns size of merged hull
// merges the left and right hull using upper and lower tangents*/
int mergeHull (double * left_x, double * left_y, double * right_x, double * right_y, double * hull_x, double * hull_y) {
    int left_size = getSize(left_x);
    int right_size = getSize(right_x);
    
    int i, i_left = 0, i_right = 0;

    for(i = 1; i < left_size;i++){
        if(left_x[i] > left_x[i_left]){
            i_left = i;
        }
    }
    for(i = 1; i < right_size;i++){
        if(right_x[i] > right_x[i_right]){
            i_right = i;
        }
    }

    int i_nd_left = i_left, i_nd_right = i_right;
    int finished = 0;

    while(!finished) {
        finished = 1;
        while(orientation(right_x[i_nd_right], right_y[i_nd_right], left_x[i_nd_left], left_y[i_nd_left], left_x[(i_nd_left + 1) % left_size], left_y[(i_nd_left + 1) % left_size]) >= 0){
            i_nd_left=(i_nd_left + 1) % left_size;
        }

        while(orientation(left_x[i_nd_left], left_y[i_nd_left], right_x[i_nd_right], right_y[i_nd_right], right_x[(right_size + i_nd_right - 1) % right_size], right_y[(right_size + i_nd_right - 1) % right_size]) <= 0){
            i_nd_right = (right_size + i_nd_right - 1) % right_size;
            finished = 0;
        }
    }

    int upper_left = i_nd_left, upper_right = i_nd_right;
    i_nd_left = i_left, i_nd_right = i_right;
    finished = 0;

    while(!finished) {
        finished = 1;
        while(orientation(left_x[i_nd_left], left_y[i_nd_left], right_x[i_nd_right], right_y[i_nd_right], right_x[(i_nd_right + 1) % right_size], right_y[(i_nd_right + 1) % right_size]) >= 0){
            i_nd_right=(i_nd_right+1) % right_size;
        }

        while(orientation(right_x[i_nd_right], right_y[i_nd_right], left_x[i_nd_left], left_y[i_nd_left], left_x[(left_size + i_nd_left - 1) % left_size], left_y[(left_size + i_nd_left - 1) % left_size]) <= 0){
            i_nd_left = (left_size + i_nd_left - 1) % left_size;
            finished = 0;
        }
    }

    int lower_left = i_nd_left, lower_right = i_nd_right;
    int hull_i = 0;
    int ind = upper_left;
    hull_x[hull_i] = left_x[upper_left];
    hull_y[hull_i++] = left_y[upper_left];

    while (ind != lower_left) {
        ind = (ind+1) % left_size;
        hull_x[hull_i] = left_x[ind];
        hull_y[hull_i++] = left_y[ind];
    }

    ind = lower_right;
    hull_x[hull_i] = right_x[lower_right];
    hull_y[hull_i++] = right_y[lower_right];

    while (ind != upper_right) {
        ind = (ind+1) % right_size;
        hull_x[hull_i] = right_x[ind];
        hull_y[hull_i++] = right_y[ind];
    }

    return hull_i + 1;
}

// recusive function for determining the convex hull of the set of points
int hull (int size, double * x_values, double * y_values, double * hull_x, double * hull_y) {
    int i,j;

    if (size <= 5) {
        bruteForceConvexHull2(size, x_values, y_values, hull_x, hull_y);
        return getSize(hull_x);
    }

    //create the left and right arrays
    double * left_x = malloc((size/2) * sizeof(double));
    double * right_x = malloc((size/2) * sizeof(double));
    double * left_y = malloc((size/2) * sizeof(double));
    double * right_y = malloc((size/2) * sizeof(double));

    for(i = 0; i < size/2; i++){
        left_x[i] = x_values[i];
        left_y[i] = y_values[i];
    }
    j = 0;
    for(i = size/2; i < size-1;i++){
        right_x[j] = x_values[i];
        right_y[j++] = y_values[i];
    }

    double * left_hull_x = malloc((size/2) * sizeof(double));
    double * right_hull_x = malloc((size/2) * sizeof(double));
    double * left_hull_y = malloc((size/2) * sizeof(double));
    double * right_hull_y = malloc((size/2) * sizeof(double));

    for(i = 0; i < size/2; i++){
        left_hull_x[i] = 0;
        right_hull_x[i] = 0;
        left_hull_y[i] = 0;
        right_hull_y[i] = 0;
    }

    // recurse
    hull(size/2, left_x, left_y, left_hull_x, left_hull_y);
    hull(size/2, right_x, right_y, right_hull_x, right_hull_y);

    int size_of_hull = mergeHull(left_hull_x, left_hull_y, right_hull_x, right_hull_y, hull_x, hull_y);

    free(left_x);
    free(right_x);
    free(left_y);
    free(right_y);

    free(left_hull_x);
    free(right_hull_x);
    free(left_hull_y);
    free(right_hull_y);

    return size_of_hull;
}
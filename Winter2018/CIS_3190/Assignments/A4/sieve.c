#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main () {
    char input[255];
    long int upper_lim = 0;
    int correct_input = 0;
    struct timespec tsi, tsf; 

    while(correct_input != 1) {
        printf("Enter the upper limit of primes to find: ");
        
        fgets(input, 255, stdin);

        upper_lim = strtol(input, NULL, 10);
        if(upper_lim == 0 || upper_lim <= 2) {
            printf("Please enter a valid integer greater than 2 and not much greater than 1000000.\n");
        } else {
            correct_input = 1;
        }
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsi);

    int int_array[upper_lim];
    int prime_array[upper_lim];

    int int_counter = 0;
    int i, j;

    for(i = 0; i < upper_lim; i++, int_counter++){
        int_array[int_counter] = i;
        prime_array[i] = -1;
    }

    for (i=2;i*i<upper_lim;i++){ // i*i because and number which is divisible by a number greater than the square root of i*i is also divisible by a number less than the sqaure root of i*i
        if (int_array[i]!=-1){
            for (j= i*2;j<upper_lim;j+=i) {
                int_array[j]=-1;
            }            
        }
    }

    int prime_counter = 0; 

    for(i = 2; i < upper_lim;i++){
        if(int_array[i] != -1) prime_array[prime_counter++] = int_array[i];
    }

    printf("Number: %d\n", prime_counter);
    //write to output file
    FILE * fp = fopen("output.txt", "w");
    for(i = 0; i < upper_lim; i++){
        if(prime_array[i] != -1) fprintf(fp, "%d,", prime_array[i]);
    }

    fclose(fp);

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tsf);

    double elaps_s = difftime(tsf.tv_sec, tsi.tv_sec);
    long elaps_ns = tsf.tv_nsec - tsi.tv_nsec;

    printf("Runtime: %lf sec\n", (elaps_s + (double)elaps_ns) / 1.0e9);

    return 1;
}
/* Silas Sales  
0922973
A3 - CIS_3490 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

int bruteForceSearch(char * pattern) {

    /* get file contents */
    char *file_contents;
    long input_file_size;
    FILE *input_file = fopen("data_5.txt", "rb");
    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);
    file_contents = malloc(input_file_size * (sizeof(char)));
    fread(file_contents, sizeof(char), input_file_size, input_file);
    fclose(input_file);

    int i,j;
    int n = strlen(file_contents);
    int m = strlen(pattern) - 1;
    int number_of_matches = 0;
    int number_of_pattern_shifts = 0;
    for(i = 0; i < n - m; i++){
        j = 0;
        while(j < m && pattern[j] == file_contents[i + j]) {
            j++;
        }
        if(j == m) {
            number_of_matches++;
        }
        number_of_pattern_shifts++;
    }

    printf("Number of pattern shifts: %d\n", number_of_pattern_shifts);
    free(file_contents);
    return number_of_matches;
}

int main() {

    struct timeb t_start, t_current;
    int t_dif;
    int i;
    int number_of_matches;
    /*get pattern from user*/
    printf("Please enter the pattern you want to search for: ");
    char pattern[255];
    fgets(pattern, 255, stdin);

    ftime(&t_start);
    number_of_matches = bruteForceSearch(pattern);
    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Number of matches: %d\n", number_of_matches);
    printf("Brute force search execution time (milliseconds): %d\n", t_dif);

    return 1;
}
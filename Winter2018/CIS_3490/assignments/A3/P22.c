/* Silas Sales  
0922973
A3 - CIS_3490 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#define SIZE_OF_ALPHABET 255


void shiftTable(int * table, char * pattern){
    int m = strlen(pattern) - 1;
    int i,j;

    for(i = 0; i < SIZE_OF_ALPHABET; i++){
        table[i] = m;
    }
    for (j = 0; j < m - 1;j++){
        table[pattern[j]] = m - 1 - j;
    }
}

int horspool(char * pattern) {

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

    int i,j,k;
    int number_of_matches = 0;
    int number_of_pattern_shifts = 0;

    int shift_table[SIZE_OF_ALPHABET];

    shiftTable(shift_table, pattern);

    int m = strlen(pattern)-1;
    int n = strlen(file_contents);

    i = m-1;
    while (i <= n-1) {
        k = 0;
        while(k <= m-1 && pattern[m - 1 - k] == file_contents[i - k]){
            k++;
        }
        if(k == m){
            number_of_matches++;
        }
        if(file_contents[i] >= 0 && file_contents[i] < SIZE_OF_ALPHABET) { // to avoid weird characters
            i += shift_table[file_contents[i]];
        } else {
            i += m;
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
    number_of_matches = horspool(pattern);

    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Number of matches: %d\n", number_of_matches);
    printf("Horspool search execution time (milliseconds): %d\n", t_dif);

    return 1;
}
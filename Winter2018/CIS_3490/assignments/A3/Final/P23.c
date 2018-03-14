/* Silas Sales  
0922973
A3 - CIS_3490 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#define SIZE_OF_ALPHABET 255

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

void badCharacterTable(int * table, char * pattern){
    int m = strlen(pattern) - 1;
    int i,j;

    for(i = 0; i < SIZE_OF_ALPHABET; i++){
        table[i] = m;
    }
    for (j = 0; j < m - 1;j++){
        table[pattern[j]] = m - 1 - j;
    }
}

void goodSuffixTable(int * table, int * border_pos, char * pattern) {
    int m = strlen(pattern) - 1;
    int i = m;
    int j = m+1;
    border_pos[i] = j;
    while(i > 0) {
        while(j <= m && pattern[i-1] != pattern[j -1]){
            if(table[j] == 0){
                table[j] = j-i;
            }
            j = border_pos[j];
        }
        i--;j--;
        border_pos[i] = j;
    }
}

void case2Table(int * table, int * border_pos, char * pattern) {
    int m = strlen(pattern) - 1;

    int i,j;
    j = border_pos[0];
    for(i = 0; i <= m;i++){
        if (table[i] == 0){
            table[i] = j;
        }

        if(i == j){
            j = border_pos[j];
        }
    }
}

int BoyerMoore(char * pattern) {

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
    int m = strlen(pattern) - 1;
    int n = strlen(file_contents);

    int bad_char_table[SIZE_OF_ALPHABET];
    int good_suffix_table[m+1];
    int border_pos[m+1];

    for(i = 0; i < m+1; i++){
        good_suffix_table[i] = 0; 
    }

    badCharacterTable(bad_char_table, pattern);
    goodSuffixTable(good_suffix_table, border_pos, pattern);
    case2Table(good_suffix_table, border_pos, pattern);

    j = 0;

    while(j <= n - m) {
        for(i = m - 1; i >= 0 && pattern[i] == file_contents[i + j]; --i);
        if (i < 0) {
            number_of_matches++;
            j += good_suffix_table[0];
        } else {
            if(file_contents[i + j] >= 0 && file_contents[i + j] < SIZE_OF_ALPHABET) {
                j += max(good_suffix_table[i+1], bad_char_table[file_contents[i+j]]);
            } else {
                j += m;
            }
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
    number_of_matches = BoyerMoore(pattern);

    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Number of matches: %d\n", number_of_matches);
    printf("Boyer-Moore search execution time (milliseconds): %d\n", t_dif);

    return 1;
}
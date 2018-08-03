/* Silas Sales  
0922973
A3 - CIS_3490 */

/* Brute force algo design
 read the file and build an array of strings within the file
 read input and get the string to find anagrams of
 search the file words word by word
 compare the inputted word char by char for each word within the file
 if the second word contains the char, remove if from the second word and continue with the next char from the first
 if we get to the end of the first word, the second is an anagram
 if we dont find a letter from the first in the second, it is not an anagram

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

void findAnagrams(char * word, char ** anagram_array){
    int i = 0, j = 0, k = 0;
    int size_of_word = strlen(word);  
    int is_anagram = 0;  
    int found_letter = 0;
    int num_of_anagrams = 0;
    char * org_word;
    
    for(i = 0; i < 30000; i++){
        int size_of_file_word = strlen(anagram_array[i]);
        if (size_of_word != size_of_file_word) continue; // have to be the same size to be an anagram

        // make a copy of the word so that we can print it out
        org_word = (char*)malloc(size_of_file_word * sizeof(char));
        strcpy(org_word, anagram_array[i]);
                
        is_anagram = 1;
        for(j = 0; j < size_of_word;j++) {
            found_letter = 0;
            for(k = 0; k < size_of_file_word;k++){
                if(word[j] == anagram_array[i][k]) {
                    anagram_array[i][k] = ' '; // 'remove' by setting the char value to a space
                    found_letter = 1;
                    break; // go to next letter of input word
                } else if(anagram_array[i][k] == ' ') {
                    continue; // 'removed' char
                }
            }
            if(!found_letter) {
                is_anagram = 0;
                break;
            }
        }
        if(is_anagram){
            num_of_anagrams++;
            printf("Anagram #%d of \'%s\': \'%s\'\n", num_of_anagrams, word, org_word);
        }

        free(org_word);
    }

    if(num_of_anagrams == 0){
        printf("Could not find any anagrams of \'%s\'\n", word);
    }
}

void readFile(char ** array_of_strings) {
    FILE * fp = fopen("data_4.txt", "r");

    int int_string = 0;
    char * string;
    int i = 0;
    int nDigits = 0;
    fscanf (fp, "%d", &int_string);    
    while (i < 30000)
        {  
        nDigits = floor(log10(abs(int_string))) + 1;
        string = (char *) malloc(nDigits * sizeof(char));
        sprintf(string, "%d", int_string);
        array_of_strings[i] = string;

        fscanf (fp, "%d", &int_string);   
        i++;   
        }
    fclose (fp);     
}

void freeArray(char ** array_of_strings) {
    int i = 0;
    for (i = 0; i < 30000; i++){
        free(array_of_strings[i]);
    }
}

int main() {
    char * array_of_strings[30000];
    char input_word[50];
    struct timeb t_start, t_current;
    int t_dif;
    int i;

    printf("Enter a string: ");
    scanf("%s", input_word);
    printf("\nFinding anagrams...\n");

    ftime(&t_start);

    readFile(array_of_strings);
    findAnagrams(input_word, array_of_strings);
    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("Brute Force execution time (milliseconds): %d\n", t_dif);

    freeArray(array_of_strings);

}


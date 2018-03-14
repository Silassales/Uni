/* Silas Sales  
0922973
A3 - CIS_3490 */

/* presort algo
 for each string in the file, first sort the string
 then sort the input string, search through all the sorted strings and find the ones that match, those are anagrams

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

void swap(char * a, char * b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

int partition (char * string, int low, int high)
{
    int pivot = string[high];
    int i = (low - 1);
    int j;
 
    for (j = low; j <= high- 1; j++)
    {
        if (string[j] <= pivot)
        {
            i++;
            swap(&string[i], &string[j]);
        }
    }
    swap(&string[i + 1], &string[high]);
    return (i + 1);
}
 
void quickSort(char * string, int low, int high)
{
    if (low < high)
    {
        int partiion = partition(string, low, high);
 
        quickSort(string, low, partiion - 1);
        quickSort(string, partiion + 1, high);
    }
}

void findAnagrams(char * input_word, char ** sorted_array, char ** array_of_strings) {
    int i = 0;
    int num_anagrams = 0;
    int found_anagram = 0;
    char * org_word;

    org_word = (char*)malloc(strlen(input_word) * sizeof(char));
    strcpy(org_word, input_word);
    quickSort(input_word, 0, strlen(input_word) - 1);

    for(i = 0; i < 30000; i++){
        if(strcmp(input_word, sorted_array[i]) == 0){
            found_anagram = 1;
            num_anagrams++;
            printf("Anagram #%d of \'%s\': \'%s\'\n", num_anagrams, org_word, array_of_strings[i]);
        }
    }

    if(!found_anagram) printf("Could not find any anagrams of \'%s\'\n", input_word);
}

// creates a sorted verion of the original array
void sortArray(char ** sorted_array, char ** array_of_strings) {
    int i = 0;
    char * string;

    for(i = 0; i < 30000; i++){
        string = (char*) malloc(strlen(array_of_strings[i]) * sizeof(char));
        strcpy(string, array_of_strings[i]);
        quickSort(string, 0, strlen(string) - 1);
        sorted_array[i] = string;
    }
}

// reads in the file, sets up the array 
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
    char * sorted_array[30000];
    char input_word[50];
    struct timeb t_start, t_current;
    int t_dif;
    int i;

    printf("Enter a string: ");
    scanf("%s", input_word);
    printf("\nFinding anagrams...\n");

    ftime(&t_start);
    readFile(array_of_strings);
    sortArray(sorted_array, array_of_strings);
    findAnagrams(input_word, sorted_array, array_of_strings);
    ftime(&t_current);
    t_dif = (int) (1000*(t_current.time - t_start.time) + (t_current.millitm - t_current.millitm)); 

    printf("PreSort execution time (milliseconds): %d\n", t_dif);

    freeArray(array_of_strings);
    freeArray(sorted_array);

}


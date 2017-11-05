#include "ClassStorage.h"

/* readInputFile
    Reads in the first original C++ file, and outputs tokenized file
    into "tmp.txt"
*/
void readInputFile(char * the_file);

/* storeInputFile
    Reads in the parsed tmp.txt file created by the above functions and
    stores the values into linked lists, also will print correct output to 
    given file
*/
void storeInputFile (char * the_file, char * out_file);

void replaceNewLineAndSemiColon(char * string);
void replaceNewLine(char * string);

/* enterNewLine 
    Checks if we should in face enter a new line based on that last character that was parsed
    */
int enterNewLine (int last_char);

void storeClass(Class_Struct * the_class, FILE * the_file, Class_Struct *);

void assignBody(Methods * the_method, char * temp_string);
 
void getAndStoreParamList(Methods * the_method, FILE * the_file);
void getAndStoreFunctionBody(Methods * the_method, FILE * the_file, Class_Struct *);

/* The following are helper functions for 
the storeInputFile function, the name will be
pretty selfexpanitory, the will return 1 on true
0 on false */
int isInclude(char * the_string);

int isComment(char * the_string);

int isOpenBrace(char * the_string);

int isClass(char * the_string);

char * instanceInClass(char * the_name, Class_Struct * the_classes);

char * getClassName(char * the_name, Class_Struct * the_classes);

char * getIdentifierOfClass (char * the_string, char ** class_names, int num_filled);

int isClassVar(Class_Struct * the_class, char * the_string);

int isParam(Local_Vars * the_params, char * the_string);

int isLocalVar(char ** local_vars, int num_filled, char * temp_string);

int isCurrentClass(char * the_string, char ** class_names, int);

int addBracket(char * the_string);

int isReturnType(char * the_string);
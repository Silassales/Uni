#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parsing.h"

int main (int argc, char * argv[]) {
    if(argc != 2) {
        puts("Usage: ./a1 fileToCovert.cc");
        exit(1);
    }
    readInputFile(argv[1]);
    storeInputFile("tmp.txt", "output.c");
    return 1;
}

void readInputFile(char * the_file) {
    FILE * input_file;
    FILE * output_file;
    int temp_char; //int because fgetc returns an int (its all the same)
    int last_char = 0;

    if((input_file = fopen(the_file, "r")) == NULL){
        printf("Error opening input file.\n");
        exit(1);
    }

    if((output_file = fopen("tmp.txt", "w")) == NULL) {
        printf("Error opening output file.\n");
        exit(1);
    }

    while((temp_char = fgetc(input_file)) != EOF){
        switch (temp_char) {
            // whitespace
            case ' ':
            case '\t':
            case '\n':
            case '\v':
            case '\f':
            case '\r':
                if (enterNewLine(last_char)) fprintf(output_file, "\n");
                break;
           
            case '(':
                if (isspace(last_char)) { // act normal
                    fprintf(output_file, "%c\n", temp_char);
                    last_char = -1;
                } else { // print out extra newline
                    fprintf(output_file, "\n%c\n", temp_char);
                    last_char = -1; 
                }
                break;
            case '!':
            case '=':
                if (last_char == '=' || last_char == '!') {fprintf(output_file, "%c\n", temp_char);}
                else {fprintf(output_file, "%c", temp_char);}
                break;
            case ',':
            case '.':
            case ';':
            case ')':
            case '{':
            case '}':
                if (isspace(last_char)) { // act normal
                    fprintf(output_file, "%c\n", temp_char);
                    last_char = -1;
                } else { // print out extra newline
                    fprintf(output_file, "\n%c\n", temp_char);
                    last_char = -1; 
                }
                break;
            // comments
            case '/':
                if (last_char == '/') {
                    do {
                        fprintf(output_file, "%c", temp_char);
                        last_char = temp_char;
                    } while((temp_char = fgetc(input_file)) != '\n');
                    fprintf(output_file, "\n");
                    last_char = -1;
                } else {
                    fprintf(output_file, "%c", temp_char);
                }
                break;
            case '*':
                if (last_char == '/') {
                    while((temp_char != '/') || (last_char != '*')) {
                        fprintf(output_file, "%c", temp_char);
                        last_char = temp_char;
                        temp_char = fgetc(input_file);
                    }
                    fprintf(output_file, "%c", temp_char);
                } else {
                     fprintf(output_file, "%c", temp_char);
                }
                break;
            // quotes
            case '\"':
                fprintf(output_file, "%c", temp_char);
                while(((temp_char = fgetc(input_file)) != '\"') || (last_char == '\\')) {
                    fprintf(output_file, "%c", temp_char);
                    last_char = temp_char;
                }
                fprintf(output_file, "\"\n");
                last_char = -1;
                break;
                //include statements
            case '#':
                do {
                    fprintf(output_file, "%c", temp_char);
                } while((temp_char = fgetc(input_file)) != '\n');
                fprintf(output_file, "\n");
                last_char = -1;
                break;
            default:
                fprintf(output_file, "%c", temp_char);
                break;
            
        }
        if (last_char != -1) last_char = temp_char;
        else (last_char = '\n'); // parser manually put in newline, so keep track of this
    }

    if(fclose(input_file)){
        printf("Error closing input file.\n");
        exit(1);
    }

    if(fclose(output_file)){
        printf("Error closing output file.\n");
        exit(1);
    }
}

void storeInputFile (char * the_file, char * out_file) {
    FILE * input_file;
    FILE * output_file;
    Class_Struct * first_class = NewClassStruct("head");
    char * temp_string = "";
    char * one_ahead;
    char * two_ahead;
    size_t num_chars = 100;
    int error = 0;
    int in_class = 0;
    int num_of_reps = 0;
    int need_new_value = 1;

    if((input_file = fopen(the_file, "r")) == NULL){
        printf("Error opening input file.\n");
        exit(1);
    }

    if((output_file = fopen(out_file, "w")) == NULL){
        printf("Error opening output file.\n");
        exit(1);
    }

    while (error != -1) {
        if (need_new_value) {
            temp_string = (char *) malloc (num_chars + 1);
            error = getline (&temp_string, &num_chars, input_file);
            replaceNewLineAndSemiColon(temp_string);
        } else {
            need_new_value = 1; // only do it once
        }

        if(isInclude(temp_string) || (isComment(temp_string) && in_class == 0)) {
            fprintf(output_file, "%s\n", temp_string);
        } else if (isReturnType(temp_string)) {
            //too see if it is a variable/class/function check 2 ahead
            one_ahead = (char *) malloc (num_chars + 1);
            two_ahead = (char *) malloc (num_chars + 1);
            error = getline (&one_ahead, &num_chars, input_file);
            replaceNewLineAndSemiColon(one_ahead);
            error = getline (&two_ahead, &num_chars, input_file);
            replaceNewLineAndSemiColon(one_ahead);

            if (!isOpenBrace(two_ahead)){ // know its a variable
                Local_Vars * new_global_var = CreateLocalVar(temp_string, one_ahead);
                new_global_var->global = 1;
                printInstanceVar(new_global_var, output_file);
                freeInstanceVar(new_global_var);

                //multiple declarations on the same line
                if(two_ahead[0] == ',') {
                    while(one_ahead[0] != ';') {
                        error = getline(&one_ahead, &num_chars, input_file);
                        replaceNewLineAndSemiColon(one_ahead);

                        new_global_var = CreateLocalVar(temp_string, one_ahead);
                        new_global_var->global = 1;
                        printInstanceVar(new_global_var, output_file);
                        freeInstanceVar(new_global_var);

                        error = getline(&one_ahead, &num_chars, input_file);
                        replaceNewLine(one_ahead);
                    }
                }

                need_new_value = 0;
                free(temp_string);
                temp_string = two_ahead; // we want to check the value of two_ahead in the while loop
            } else { // know its a function or method
                Methods * new_method = CreateMethod(temp_string, one_ahead);

                getAndStoreParamList(new_method, input_file);
                getAndStoreFunctionBody(new_method, input_file, first_class);

                printMethod(new_method, output_file);

                freeMethod(new_method);

                free(two_ahead);
            }
            free(one_ahead);
        } else if (isClass(temp_string)) {
            //get the name
            one_ahead = (char *) malloc (num_chars + 1);
            error = getline (&one_ahead, &num_chars, input_file);
            replaceNewLineAndSemiColon(one_ahead);
            
            Class_Struct * new_class = NewClassStruct(one_ahead);
            addClass(first_class, new_class);
            
            storeClass(new_class, input_file, first_class);

            MethodsRename(new_class);
            
            printClass(new_class, output_file);

            free(one_ahead);
            //freeClass(new_class);
        }
        if (need_new_value) free(temp_string); // else its already been freed
        num_of_reps ++;
    }
   
    if(fclose(input_file)){
        printf("Error closing input file.\n");
        exit(1);
    }

    if(fclose(output_file)){
        printf("Error closing output file.\n");
        exit(1);
    }

    Class_Struct * temp_class = first_class;

    while(temp_class != NULL){
        first_class = temp_class;
        temp_class = first_class->next;
        freeClass(first_class);
    }
    
}

void storeClass(Class_Struct * the_class, FILE * the_file, Class_Struct * the_classes) {
    int num_of_bracket = -1;
    int need_new_value = 1;
    int error;
    char * one_ahead;
    char * two_ahead;
    size_t num_chars = 100;
    while(num_of_bracket != 0){
        if(num_of_bracket == -1) num_of_bracket = 0;
        char * temp_string = "";
        if (need_new_value) {
            temp_string = (char *) malloc (num_chars + 1);
            error = getline (&temp_string, &num_chars, the_file);
            replaceNewLineAndSemiColon(temp_string);
        } else {
            need_new_value = 1; // only do it once
        }

        num_of_bracket += addBracket(temp_string);

        if (isReturnType(temp_string)) {
            //too see if it is a variable/class/function check 2 ahead
            one_ahead = (char *) malloc (num_chars + 1);
            two_ahead = (char *) malloc (num_chars + 1);
            error = getline (&one_ahead, &num_chars, the_file);
            replaceNewLineAndSemiColon(one_ahead);
            error = getline (&two_ahead, &num_chars, the_file);
            replaceNewLineAndSemiColon(two_ahead);

            if (!isOpenBrace(two_ahead)){ // know its a variable
                Local_Vars * new_local_var = CreateLocalVar(temp_string, one_ahead);
                addInstanceVar(the_class, new_local_var);

                //multiple declarations on the same line
                if(two_ahead[0] == ',') {
                    while(one_ahead[0] != ';') {
                        error = getline(&one_ahead, &num_chars, the_file);
                        replaceNewLineAndSemiColon(one_ahead);

                        new_local_var = CreateLocalVar(temp_string, one_ahead);
                        addInstanceVar(the_class, new_local_var);

                        error = getline(&one_ahead, &num_chars, the_file);
                        replaceNewLine(one_ahead);
                    }
                }

                need_new_value = 1;
                free(temp_string);
                temp_string = two_ahead; // we want to check the value of two_ahead in the while loop
            } else { // know its a function or method
                Methods * new_method = CreateMethod(temp_string, one_ahead);

                // add a reference to the current class
                char * temp_param_type = malloc(sizeof(char) * (10 + sizeof(the_class->name)));
                char * temp_param_name = malloc(sizeof(char) * (sizeof(the_class->name) + 4));
                sprintf(temp_param_type, "struct %s *", the_class->name);
                sprintf(temp_param_name, "the_%s", the_class->name);
                Local_Vars * class_ref = CreateLocalVar(temp_param_type, temp_param_name);

                addParam(new_method, class_ref);

                getAndStoreParamList(new_method, the_file);
                getAndStoreFunctionBody(new_method, the_file, the_classes);

                addMethod(the_class, new_method);

                free(temp_param_type);
                free(temp_param_name);
                free(two_ahead);
            }
            free(one_ahead);
        }
        if(need_new_value) {
            free(temp_string);
        }
        if(error); //get rid of compiler errors, in case i want to check error value of getline later
    }
    
}

void getAndStoreParamList(Methods * the_method, FILE * the_file) {
    char * type;
    char * name;
    char * check_for_bracket;
    int error = 0;
    size_t num_chars = 100;

    while(error != -1) {
        type = (char *) malloc (num_chars + 1);
        error =getline (&type, &num_chars, the_file);
        if(*type == ')') {
            free(type);
            return;
        }
        name = (char *) malloc (num_chars + 1);
        check_for_bracket = (char *) malloc (num_chars + 1);

        
        error =getline (&name, &num_chars, the_file);
        error =getline (&check_for_bracket, &num_chars, the_file);

        replaceNewLineAndSemiColon(type);
        replaceNewLineAndSemiColon(name);
        replaceNewLineAndSemiColon(check_for_bracket);
        if(*check_for_bracket == ')') error = -1;

        Local_Vars * new_param = CreateLocalVar(type, name);
        addParam(the_method, new_param);
        

        free(type);
        free(name);
        free(check_for_bracket);
    }
    
}

void getAndStoreFunctionBody(Methods * the_method, FILE * the_file, Class_Struct * the_classes) {
    char * temp_string;
    char * one_ahead;
    char * two_ahead;
    char * string_to_pass;
    char * new_name;
    int i;
    char **class_names = (char**) malloc(sizeof(char*) * 50);
    char **local_var = (char**) malloc(sizeof(char*) * 50);

    for ( i = 0; i < 50; i++ )
    {
        local_var[i] = (char*) malloc(sizeof(char) * 256);
        class_names[i] = (char*) malloc(sizeof(char) * 256);
    }
    int curr_pos = 0;
    int curr_pos_vars = 0;
    size_t num_chars = 100;
    int error = 0;
    if(error); // compiler warning
    int bracket_count = -1;
    

    // get current Class 
    // know the last class added will be our current class
    Class_Struct * current_class = the_classes;
    while(current_class->next->next != NULL) {
        current_class = the_classes->next;
    }
    current_class = current_class->next;

    while(bracket_count != 0) {
        if(bracket_count == -1) bracket_count = 0;

        temp_string = (char *) malloc (num_chars + 1);
        error = getline (&temp_string, &num_chars, the_file);
        replaceNewLineAndSemiColon(temp_string);

        bracket_count += addBracket(temp_string);

        if (isClass(temp_string)) {
            //get the name
            one_ahead = (char *) malloc (num_chars + 1);
            error = getline (&one_ahead, &num_chars, the_file);
            replaceNewLineAndSemiColon(one_ahead);
            two_ahead = (char *) malloc (num_chars + 1);
            error = getline (&two_ahead, &num_chars, the_file);
            replaceNewLineAndSemiColon(two_ahead);

            // //store the name, so we know we found a class and can change later references
            strcpy(class_names[curr_pos], two_ahead);
            curr_pos++;
            
            /* * 6 (struct) + name + identifier + 15 (constructor();) + name + 5(spaces, etc)*/
            string_to_pass = (char *) malloc(((7 + strlen(one_ahead) + strlen(two_ahead) + 20 + strlen(one_ahead) + strlen(two_ahead) +strlen(two_ahead) + 5))); 

            sprintf(string_to_pass, "struct %s %s ;\n\tconstructor%s(&%s);\n\t", one_ahead,two_ahead, one_ahead,two_ahead );
            assignBody(the_method, string_to_pass);

            error = getline (&one_ahead, &num_chars, the_file); // get rid of semi colon
            free(one_ahead);
            free(two_ahead);
        } else if (isCurrentClass(temp_string, class_names, curr_pos)) {
            one_ahead = (char *) malloc (num_chars + 1);
            error = getline (&one_ahead, &num_chars, the_file); // get rid of .
            error = getline (&one_ahead, &num_chars, the_file);
            replaceNewLineAndSemiColon(one_ahead);

            if((new_name = instanceInClass(one_ahead, the_classes)) != NULL) {
                char * class_name = getIdentifierOfClass(temp_string, class_names, curr_pos);
                string_to_pass = malloc((strlen(temp_string) + strlen(new_name) + 5));

                sprintf(string_to_pass, "%s.%sPtr( &%s", temp_string, new_name, class_name);
                assignBody(the_method, string_to_pass);

                error = getline (&one_ahead, &num_chars, the_file); //get rid of (
            }
            // print out the rest
            int end = 0;
            int first_time = 1;
            while(!end) {
                string_to_pass = (char *) malloc (num_chars + 1);
                error = getline (&string_to_pass, &num_chars, the_file);
                replaceNewLine(string_to_pass);
                if(strcmp(string_to_pass, ";") == 0) {
                    end = 1;
                } else if (first_time == 1 && (string_to_pass[0] != ')')) {
                    char * comma_in_front = strdup(",");
                    assignBody(the_method, comma_in_front);
                    first_time = 0;
                }
                assignBody(the_method, string_to_pass);
            }
            free(one_ahead);
        } else if(isReturnType(temp_string)) {
            // print to the semi colon, dont want to confuse local vars with instance vars
            int end = 0;
            if (temp_string[0] == '{') end = 1;
            assignBody(the_method, temp_string);
            temp_string = malloc(sizeof(char)); // since assign body freed it

            string_to_pass = (char *) malloc (num_chars + 1);
            // get name
            error = getline (&string_to_pass, &num_chars, the_file);
            replaceNewLine(string_to_pass);
            
            strcpy(local_var[curr_pos_vars], string_to_pass);
            curr_pos_vars++;
            bracket_count += addBracket(string_to_pass);
            if((strcmp(string_to_pass, ";") == 0) || (string_to_pass[0] == '/')) {
                end = 1;
            }
            assignBody(the_method, string_to_pass);

            while(!end && bracket_count > 0) {
                string_to_pass = (char *) malloc (num_chars + 1);
                error = getline (&string_to_pass, &num_chars, the_file);
                replaceNewLine(string_to_pass);
                bracket_count += addBracket(string_to_pass);
                if((strcmp(string_to_pass, ";") == 0) || (string_to_pass[0] == '/')) {
                    end = 1;
                }
                assignBody(the_method, string_to_pass);
            }
        } else if (isClassVar(current_class, temp_string) && (strcmp(the_method->name, "main") != 0)) {
            // print out reference in front
            char * class_name = current_class->name;
            string_to_pass = (char *) malloc(strlen(temp_string) + strlen(class_name) + 7);
            sprintf(string_to_pass, "the_%s->%s", class_name, temp_string);
            assignBody(the_method, string_to_pass);
        } else if(isComment(temp_string)) {
            assignBody(the_method, temp_string);
            temp_string = malloc(sizeof(char)); // since assign body freed it
        } else {
            int end = 0;
            char * most_recent_class_identifier = NULL;
            if (temp_string[0] == '{') end = 1;
            assignBody(the_method, temp_string);
            
            temp_string = malloc(sizeof(char)); // since assign body freed it

            while(!end && bracket_count > 0) {
                string_to_pass = (char *) malloc (num_chars + 1);
                error = getline (&string_to_pass, &num_chars, the_file);
                replaceNewLine(string_to_pass);

                //used for replacing class function call indentifier names
                if(isCurrentClass(string_to_pass, class_names, curr_pos)) {
                    free(most_recent_class_identifier);
                    most_recent_class_identifier = strdup(string_to_pass);
                }

                bracket_count += addBracket(string_to_pass);

                //check for instance vars within the function body that are not at the start of a line
                if((!isLocalVar(local_var, curr_pos_vars, string_to_pass) && isClassVar(current_class, string_to_pass)
                    && !isParam(the_method->first_param, string_to_pass)) && (strcmp(the_method->name, "main") != 0)) {
                    // print out reference in front
                    char * class_name = current_class->name;
                    char * new_var = (char *) malloc(strlen(string_to_pass) + strlen(class_name) + 7);
                    sprintf(new_var, "the_%s->%s", class_name, string_to_pass);
                    free(string_to_pass);
                    assignBody(the_method, new_var);
                } else if ((new_name = instanceInClass(string_to_pass, the_classes)) != NULL) {
                    free(string_to_pass);
                    //check for method calls within function calls
                    char * new_var = (char*) malloc(strlen(new_name) + 9 + strlen(most_recent_class_identifier));
                    sprintf(new_var, "%sPtr ( &%s", new_name, most_recent_class_identifier);

                    assignBody(the_method, new_var);

                    char * bracket = (char *) malloc(num_chars + 1);
                    error = getline (&bracket, &num_chars, the_file); //get rid of (
                    free(bracket);

                    // print out the rest
                    int end = 0;
                    int first_time = 1;
                    while(!end) {
                        new_var = (char *) malloc (num_chars + 1);
                        error = getline (&new_var, &num_chars, the_file);
                        replaceNewLine(new_var);
                        if(strcmp(new_var, ";") == 0) {
                            end = 1;
                        } else if (first_time == 1 && (new_var[0] != ')')) {
                            char * comma_in_front = strdup(",");
                            assignBody(the_method, comma_in_front);
                            first_time = 0;
                        }
                        assignBody(the_method, new_var);
                    }
                } else {
                    if((strcmp(string_to_pass, ";") == 0) || (string_to_pass[0] == '/')) {
                        end = 1;
                    }
                    assignBody(the_method, string_to_pass);
                }
            }

            free(most_recent_class_identifier);
        }
        free(temp_string); 
    }
    for ( i = 0; i < 50; i++ )
    {   
        free(local_var[i]);
        free(class_names[i]);
    }
    free(local_var);
    free(class_names);
}

/* frees temp_string*/
void assignBody(Methods * the_method, char * temp_string) {
    char * new_body;
    if(the_method->body == NULL) {
        /*
        new_body = strdup(temp_string);
        strcpy(new_body, temp_string);
        the_method->body = new_body;*/
        // know its a bracket so ignore it
        new_body = strdup("\t");
        the_method->body = new_body;
        free(temp_string);
        return;
    } else {
        new_body = the_method->body;
    }

    char * old_body;

    if(addBracket(temp_string) == 0){ // not a {}
        old_body = new_body;
        new_body = (char*) malloc((strlen(old_body) + strlen(temp_string) + 3)); 
        sprintf(new_body, "%s%s", old_body, temp_string);
        free(old_body);
        if(*temp_string == ';' || isComment(temp_string)) {
            strcat(new_body, "\n\t");
        } else if (isdigit(*temp_string) || *temp_string == '.') {
            // do nothing to the format
        } else {
            char test_char = temp_string[strlen(temp_string) - 2];
            if(test_char != '\n'){ // fix formatting issues with constructors
                strcat(new_body, " ");
            }
        }
    }
    free(temp_string);
    new_body[strlen(new_body)] = '\0';


    the_method->body = new_body;
}

void replaceNewLineAndSemiColon(char * string) {
    int i;
    for (i = 0; i < strlen(string);i++){
        if(string[i] == '\n' || string[i] == ';') {
            string[i] = '\0';
        }
    }
}

void replaceNewLine(char * string) {
    int i;
    for (i = 0; i < strlen(string);i++){
        if(string[i] == '\n') {
            string[i] = '\0';
        }
    }
}
int enterNewLine (int last_char) {
    if (isspace(last_char)) return 0;
    switch (last_char) {
        case '(':
        case ')':
        case ';':
        case '{':
        case ',':
        case '}':
        case '\n':
            return 0;
        default:
            return 1;
    }
}

int isInclude(char * the_string) {
    if (the_string[0] == '#') return 1;
    else return 0;
}

int isComment(char * the_string) {
    if (the_string[0] == '/') return 1;
    else return 0;
}

int isReturnType(char * the_string) {
    if (strcmp(the_string, "int") == 0) {
        return 1;
    } else if (strcmp(the_string, "char") == 0) {
        return 1;
    } else if (strcmp(the_string, "float") == 0) {
        return 1;
    } else if (strcmp(the_string, "double") == 0) {
        return 1;
    } else if (strcmp(the_string, "void") == 0) {
        return 1;
    } else {

        return 0;
    }
}

int isOpenBrace(char * the_string) {
    if (the_string[0] == '{' || the_string[0] == '(') return 1;
    else return 0;
}

int isClass(char * the_string) {
    if (strcmp(the_string, "class") == 0) return 1;
    else return 0;
}

int isClassVar(Class_Struct * the_class, char * the_string) {
    Local_Vars * temp_instance_var = the_class->first_instance_vars;

    while(temp_instance_var != NULL ) {
        if(strcmp(the_string, temp_instance_var->name) == 0) {
            return 1;
        }
        temp_instance_var = temp_instance_var->next;
    }
    return 0;
}

int isParam(Local_Vars * the_params, char * the_string) {
    Local_Vars * temp_the_params = the_params;

    while(temp_the_params != NULL ) {
        if(strcmp(the_string, temp_the_params->name) == 0) {
            return 1;
        }
        temp_the_params = temp_the_params->next;
    }
    return 0;
}

int isLocalVar(char ** local_vars, int num_filled, char * temp_string) {
    int i;
    if(num_filled == 0){
        return 0;
    }
    
    for(i = 0;i < num_filled;i++){
        if(strcmp(temp_string, local_vars[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char * instanceInClass(char * the_name, Class_Struct * the_classes) {
    Class_Struct * temp_class = the_classes;
    if(the_classes == NULL) {
        return 0;
    }
    
    while (temp_class != NULL) {
        if(temp_class->first_method != NULL){
            Methods * temp_method = temp_class->first_method;

            while (temp_method != NULL) {
                if(strcmp(the_name, temp_method->old_name) == 0) {
                    return temp_method->name;
                }
                temp_method = temp_method->next;
            }
        }
        temp_class = temp_class->next;
    }
    return NULL;
}

char * getClassName(char * the_name, Class_Struct * the_classes) {
    Class_Struct * temp_class = the_classes;
    if(the_classes == NULL) {
        return 0;
    }
    
    while (temp_class != NULL) {
        if(temp_class->first_method != NULL){
            Methods * temp_method = temp_class->first_method;

            while (temp_method != NULL) {
                if(strcmp(the_name, temp_method->old_name) == 0) {
                    return temp_class->name;
                }
                temp_method = temp_method->next;
            }
        }
        temp_class = temp_class->next;
    }
    return NULL;
}

int isCurrentClass(char * the_string, char ** class_names, int num_filled) { 
    int i;
    if(num_filled == 0){
        return 0;
    }
    
    for(i = 0;i < num_filled;i++){
        if(strcmp(the_string, class_names[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char * getIdentifierOfClass (char * the_string, char ** class_names, int num_filled) {
    int i;
    if(num_filled == 0){
        return 0;
    }
    
    for(i = 0;i < num_filled;i++){
        if(strcmp(the_string, class_names[i]) == 0) {
            return class_names[i];
        }
    }
    return 0;
}

int addBracket(char * the_string) {
    if(the_string[0] == '{') {
        return 1;
    } else if (the_string[0] == '}') {
        return -1;
    } else return 0;
}
 #include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ClassStorage.h"

/*
int main () {
    Class_Struct * temp_class = NewClassStruct("silasClass");
    Methods * temp_method = CreateMethod ("int", "firstMethod");
    Methods * temp_method2 = CreateMethod ("int", "SecondMethod");
    
    Local_Vars * temp = CreateLocalVar("int", "first");
    Local_Vars * temp2 = CreateLocalVar("int", "second");
    Local_Vars * temp3 = CreateLocalVar("int", "third");
    
    Local_Vars * temp_param = CreateLocalVar("int", "first");
    Local_Vars * temp_param2 = CreateLocalVar("int", "second");
    Local_Vars * temp_param3 = CreateLocalVar("int", "third");
    
    addInstanceVar(temp_class, temp);
    addInstanceVar(temp_class, temp2);
    addInstanceVar(temp_class, temp3);
    
    addParam(temp_method, temp_param);
    addParam(temp_method, temp_param2);
    addParam(temp_method, temp_param3);
    
    setBody(temp_method, "silas is the bomb.com");
    setBody(temp_method2, "silas is the bomb.com2");
    
    addMethod(temp_class, temp_method);
    addMethod(temp_class, temp_method2);

    MethodsRename(temp_class);
    printClass(temp_class, stdout);
    freeClass(temp_class);
}
*/

Class_Struct * NewClassStruct (char * name) {
    Class_Struct * new_class_struct = malloc(sizeof(Class_Struct));
    
    char * temp_name = strdup(name);

    new_class_struct->name = temp_name;
    new_class_struct->num_instance_vars = 0;
    new_class_struct->first_instance_vars = NULL;
    new_class_struct->num_methods = 0;
    new_class_struct->first_method = NULL;
    new_class_struct->next = NULL;

    return new_class_struct;
}

Local_Vars * CreateLocalVar (char * type, char * name) {
    Local_Vars * new_local_var = malloc(sizeof(Local_Vars));

    char * temp_type = strdup(type);

    char * temp_name = strdup(name);

    new_local_var->type = temp_type;
    new_local_var->name = temp_name;
    new_local_var->value_after_var = NULL;
    new_local_var->next = NULL;
    new_local_var->global = 0;

    return new_local_var;
}

Methods * CreateMethod (char * return_type, char * name) {
    Methods * new_method = malloc(sizeof(Methods));

    char * temp_return_type = strdup(return_type);

    char * temp_name = strdup(name);

    char * temp_old_name = strdup(name);

    new_method->return_type = temp_return_type;
    new_method->name = temp_name;
    new_method->old_name = temp_old_name;
    new_method->num_param = 0;
    new_method->first_param = NULL;
    new_method->body = NULL;
    new_method->next = NULL;

    return new_method;
}

void addInstanceVar (Class_Struct * the_class, Local_Vars * the_var) {
    Local_Vars * temp_local_var = the_class->first_instance_vars;

    // first one
    if (temp_local_var == NULL){
        the_class->first_instance_vars = the_var;
    } else {
        // find the end of the list
    while (temp_local_var->next != NULL) {
        temp_local_var = temp_local_var->next;
    }

    // Once at the end of the list, add the given var
    temp_local_var->next = the_var;
    }   
}

void setBody(Methods * the_class, char * the_string) {
    char * temp_body = strdup(the_string);

    the_class->body = temp_body;
}

void addMethod (Class_Struct * the_class, Methods * the_method) {
    Methods * temp_method = the_class->first_method;

    // first one
    if (temp_method == NULL){
        the_class->first_method = the_method;
    } else {
        // find the end of the list
        while (temp_method->next != NULL) {
            temp_method = temp_method->next;
        }

        // Once at the end of the list, add the given method
        temp_method->next = the_method;
    }
}

void addClass(Class_Struct * first_class, Class_Struct * add_class) {
    Class_Struct * temp_class = first_class;
    
    while (temp_class->next != NULL) {
        temp_class = temp_class->next;
    }

    // Once at the end of the list, add the given method
    temp_class->next = add_class;
}

void addParam (Methods * the_method, Local_Vars * the_param) {
    Local_Vars * temp_param = the_method->first_param;

    // first one
    if (temp_param == NULL){
        the_method->first_param = the_param;
    } else {
        // find the end of the list
    while (temp_param->next != NULL) {
        temp_param = temp_param->next;
    }

    // Once at the end of the list, add the given var
    temp_param->next = the_param;
    }   
}

void printInstanceVar (Local_Vars * the_vars, FILE * the_stream) {
    Local_Vars * temp_local_var = the_vars;

    while (temp_local_var != NULL) {
        if (temp_local_var->value_after_var != NULL){
            fprintf(the_stream, "\t%s %s %s;\n", temp_local_var->type, temp_local_var->name, 
                temp_local_var->value_after_var);
        } else if (temp_local_var->global == 1) {
            fprintf(the_stream, "%s %s;\n", temp_local_var->type, temp_local_var->name);
        } else {
            fprintf(the_stream, "\t%s %s;\n", temp_local_var->type, temp_local_var->name);
        }
        
        temp_local_var = temp_local_var->next;
    }
}

void printParam (Local_Vars * the_param, FILE * the_stream) {
    Local_Vars * temp_param = the_param;

    fprintf(the_stream, " (");
    while (temp_param != NULL) {
        fprintf(the_stream, "%s %s", temp_param->type, temp_param->name);

        temp_param = temp_param->next;
        if (temp_param != NULL) fprintf(the_stream, ", "); //printing out commas between
    }
    fprintf(the_stream, ") ");
}

void printMethod (Methods * the_method, FILE * the_stream) {
    Methods * temp_method = the_method;

    while (temp_method != NULL) {
        // printing return_type and name
        fprintf(the_stream, "%s %s", temp_method->return_type, temp_method->name);

        // printing the parameters
        printParam(temp_method->first_param, the_stream);

        //print body and braces
        fprintf(the_stream, "{\n%s\n\n", temp_method->body);
 
        temp_method = temp_method->next;
    }
}

void printFunctionPointersParam (Local_Vars * the_param, FILE * the_stream) {
    Local_Vars * temp_param = the_param;

    while (temp_param != NULL) {
        fprintf(the_stream, "%s", temp_param->type);

        temp_param = temp_param->next;
        if (temp_param != NULL) fprintf(the_stream, ", "); //printing out commas between
    }
}

void printFunctionPointers (Methods * the_method, FILE * the_stream) {
    Methods * temp_method = the_method;

    while (temp_method != NULL) {
        fprintf(the_stream, "\t%s (*%sPtr)(",  temp_method->return_type, temp_method->name);
        printFunctionPointersParam(temp_method->first_param, the_stream);
        fprintf(the_stream, ");\n");

        temp_method = temp_method->next;
    }
}

void generateConstructors (Class_Struct * the_class, FILE * the_stream) {
    Methods * temp_method = the_class->first_method;

    fprintf(the_stream, "void constructor%s (", the_class->name);

    //print reference to struct
    fprintf(the_stream, "struct %s * the_%s", the_class->name, the_class->name);
    
    fprintf(the_stream, ") {\n");
    temp_method = the_class->first_method;

    // printing body
     while (temp_method != NULL) {
        fprintf(the_stream, "\tthe_%s->%sPtr = &%s;\n", the_class->name, temp_method->name,
            temp_method->name);

        temp_method = temp_method->next;
    }

    fprintf(the_stream, "}\n\n");
}

void printClass (Class_Struct * the_class, FILE * the_stream) {
    Class_Struct * temp_class = the_class;

    fprintf(the_stream, "struct %s {\n", temp_class->name);

    printInstanceVar(temp_class->first_instance_vars, the_stream);

    printFunctionPointers(temp_class->first_method, the_stream);

    fprintf(the_stream, "};\n\n");
    
    printMethod(temp_class->first_method, the_stream);

    generateConstructors(temp_class, the_stream);

}

void MethodsRename (Class_Struct * the_class) {
    Methods * temp_method = the_class->first_method;

    while(temp_method != NULL) {
        // create a new name
        char * old_name = temp_method->name;
        char * new_name = malloc(sizeof(char) * sizeof(the_class->name) + sizeof(temp_method->name));
        //set the new name
        sprintf(new_name, "%s%s", the_class->name, temp_method->name);

        temp_method->name = new_name;
        free(old_name);

        //set the last part to the first letter of the params
        Local_Vars * temp_param = temp_method->first_param;
        while(temp_param != NULL) {
            char * old_name = temp_method->name;
            char * new_name = (char*) malloc((strlen(temp_method->name) + 2));
            sprintf(new_name, "%s%c", old_name, temp_param->type[0]);
            free(old_name);
            temp_method->name = new_name;

            temp_param = temp_param->next;
        }

        temp_method = temp_method->next;
    } 
}

void freeInstanceVar(Local_Vars * the_var) {
    free(the_var->name);
    free(the_var->type);
    free(the_var);
}

void freeMethod(Methods * the_method) {
    free(the_method->return_type);
    free(the_method->name);
    free(the_method->old_name);
    if (the_method->body != NULL) free(the_method->body);
    Local_Vars * temp_param = the_method->first_param;
    Local_Vars * free_instance;
    // free all the params
    while(temp_param != NULL) {
        free_instance = temp_param;
        temp_param = temp_param->next;
        freeInstanceVar(free_instance);
    }
    free(the_method);
}

void freeClass (Class_Struct * the_class) {
    free(the_class->name);
    Local_Vars * temp_instance = the_class->first_instance_vars;
    Local_Vars * free_instance;
    // free all the instance vars
    
    while(temp_instance != NULL) {
        free_instance = temp_instance;
        temp_instance = temp_instance->next;
        freeInstanceVar(free_instance);
    }
    Methods * temp_method = the_class->first_method;
    Methods * free_method;
    // free all the methods
    while(temp_method != NULL) {
        free_method = temp_method;
        temp_method = temp_method->next;
        freeMethod(free_method);
    }
    free(the_class);
}
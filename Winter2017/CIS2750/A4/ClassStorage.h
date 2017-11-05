struct Local_Vars {
    char * type;
    char * name;
    int global;
    char * value_after_var;
    struct Local_Vars * next;
}; typedef struct Local_Vars Local_Vars;

struct Methods {
    char * return_type;
    char * old_name;
    char * name;
    int num_param;
    Local_Vars * first_param;
    char * body;
    struct Methods * next;
}; typedef struct Methods Methods;

struct Class_Struct {
    char * name;
    int num_instance_vars;
    Local_Vars * first_instance_vars;
    int num_methods;
    Methods * first_method;
    struct Class_Struct * next;
}; typedef struct Class_Struct Class_Struct;

// Init Functions

/* NewClassStruct 
    Only value needed at init is the class name, as the other values
    will be added to the newly created Class_Struct
*/
Class_Struct * NewClassStruct (char * name);

/* CreateLocalVar
    Will return a pointer to a new Local_Vars that has the type and 
    name passed into the function 
*/
Local_Vars * CreateLocalVar (char * type, char * name);

/* CreateMethod
    Will return a new Methods that has the return_type and name that are
    passed in, will set values for num_param, first_param to NULL, user needs to
    set these after calling CreateMethod
*/
Methods * CreateMethod (char * return_type, char * name);

// Adding functions 

/* addLocalVar
    Will add the given variable to the Class_Struct, add the end of the list of already
    existing variable
*/
void addInstanceVar (Class_Struct * the_class, Local_Vars * the_var);

/* addMethod
    Will add the given method to the Class_Struct, add the end of the list of already
    existing methods
*/

void setBody(Methods * the_class, char * the_string);
void addMethod (Class_Struct * the_class, Methods * the_method);

void addClass(Class_Struct * first_class, Class_Struct * add_class);

/* addParam
    Works the same way that addInstanceVar works with a class, but on a 
    method
*/
void addParam (Methods * the_method, Local_Vars * the_param);

// Printing functions 

/* printInstanceVar
    Will print the given list of instance varaibles to a given file stream
*/
void printInstanceVar (Local_Vars * the_vars, FILE * the_stream);

/* printParam 
    prints the given param to the given stream
*/
void printParam (Local_Vars * the_param, FILE * the_stream);

/* printMethod
    Will print the given method to the given file stream.
*/
void printMethod (Methods * the_method, FILE * the_stream);

/* printFunctionPointersParam
    Will print the parameter types to the stream for when printing function pointers 
*/
void printFunctionPointersParam (Local_Vars * the_param, FILE * the_stream);

/* printMethod
    Will print the function pointers of the given methods to the given file stream.
*/
void printFunctionPointers (Methods * the_method, FILE * the_stream);

/* generateConstructors 
    Will generate constructors for the given methods
*/
void generateConstructors (Class_Struct * the_class, FILE * the_stream);

/* printClass
    Will print given class to given stream
*/
void printClass (Class_Struct * the_class, FILE * the_stream);

// Renaming Functions

/* MethodRename **potential mem leak? assigning oldname value to newname without freeing**
    Will rename the given methods in the class to have:
    - the first part of their name as the class name
*/
void MethodsRename (Class_Struct * the_class);

/* freeing functions */

void freeInstanceVar(Local_Vars * the_var);

void freeMethod(Methods * the_method);

void freeClass(Class_Struct * the_class);

*********************************************************************
Silas Sales
0922973
Cis_3110 Assignment 1
*********************************************************************
Algorithm:
My Algorithm consisted of checking different indexes of the 2-d argument array
to determine what function to run. 
Special cases that were looked for were:
exit -> quit the program
topSecret -> my function
& at the end -> run procces in the background
add -> add all the following arguments together
arg -> print out number and list of arguments provided

If none of these cases were met, I knew that the user wanted to run a 
normal shell procces and could use fork() and other functions to run those
proccesses.

Assumptions:
-A file after either a >,< will be the last argument provided
-Input redirected from files would normally be directed from stdout
-Arguments given within quotes can not contain quotes themselves (eg no \")

Execution:

After untaring the assingment file
type make
run ./msh to start the shell

exit to close the shell
type topSecret to run my defined function (it may take a minute so if its not doing anything
be patient)

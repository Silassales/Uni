#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stream.h"

int main (int argc, char * argv[]) {
    int remove_user = 0;
    if(strcmp(argv[3], "remove") == 0) {
        remove_user = 1;
    }


    if (remove_user) {
        removeUser(argv[1], argv[2]);
    } else {
        addUser(argv[1], argv[2]);
    }

    return(0);
}
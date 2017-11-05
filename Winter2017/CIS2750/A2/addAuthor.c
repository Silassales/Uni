#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "stream.h"

int main (int argc, char * argv[]) {
    if (argc == 1) {
        puts("Incorrect number of args provided.");
        exit(1);
    }

    int bytes_to_malloc = 0;
    int i;
    int remove_user = 0;
    char * username = NULL;
    for(i = 1;i < argc;i++){
        bytes_to_malloc += strlen(argv[i]) + 1; // plus one to account for the space between them
    }

    username = (char*) malloc(bytes_to_malloc + 1);

    for(i = 1;i < argc;i++){
        if(strcmp(argv[i], "-r") != 0) {
            sprintf(username, "%s %s", username, argv[i]);
        } else {
            remove_user = 1;
        }
    }

    char * stream_list = NULL;
    size_t num_chars = 100;
    
    printf("list streams: ");

    if((getline(&stream_list, &num_chars, stdin) != -1)) {
        stream_list[strlen(stream_list) - 1] = '\0';
        if (remove_user) {
            removeUser(username, stream_list);
        } else {
            addUser(username, stream_list);
        }
    } else {
        puts("Error getting the stream list.");
    }

    free(username);
    free(stream_list);
    return(0);
}
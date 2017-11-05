#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"

class PostEntry {
    userPost * the_post;
    the_post = NULL;
    char * username;
    username = NULL;
    char * stream_name;
    stream_name = NULL;
    char * date;
    date = NULL;
    char * text;
    text = NULL;

    void readInput(char* stream, char* text_value) {

        stream_name = strdup(stream);

        text = strdup(text_value);
    }

    void formatEntry() {
        the_post = createPost(username, stream_name, date, text);
    }

    void getTimeDate() {
        // ref: http://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811
        time_t the_time;
        date = (char*)malloc(sizeof(char) * 26);
        struct tm* tm_info;

        time(&the_time);
        tm_info = localtime(&the_time);

        strftime(date, 26, "%m/%d/%y @ %H:%M:%S", tm_info);
    }

    void submitPost() {
        if(authorHasPermission(stream_name, username)) {
            updateStream(the_post);
        } else {
            printf("CANT_WRITE");
        }
    }
};

int main(int argc, char * argv[]) {
    //if(argc == 1) exit(1);
    class PostEntry new_class;
    
    int bytes_to_malloc = 0;
    int i;
    for(i = 1;i < 2;i++){
        bytes_to_malloc += strlen(argv[i]);
    }

    new_class.username = (char*) malloc(bytes_to_malloc + 2);

    for(i = 1;i < 2;i++){
        if (new_class.username != NULL) {
            sprintf(new_class.username, "%s %s", new_class.username, argv[i]);
        }
    }

    new_class.readInput(argv[2], argv[3]);
    new_class.getTimeDate();
    new_class.formatEntry();
    new_class.submitPost();
    freePost(new_class.the_post);
    free(new_class.username);
    free(new_class.stream_name);
    free(new_class.date);
    free(new_class.text);
    return 0;
}
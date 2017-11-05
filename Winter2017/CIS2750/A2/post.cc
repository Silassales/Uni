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

    void readInput() {
        int error;
        char * temp_read_value = NULL;
        size_t len = 0;
        printf("stream: ");
        getline(&temp_read_value, &len, stdin);
        temp_read_value[strlen(temp_read_value) - 1] = '\0';
        stream_name = strdup(temp_read_value);

        printf("enter text: ");

        getline(&temp_read_value, &len, stdin);
        printf("- ");
        text = strdup(temp_read_value);
        while((error = getline(&temp_read_value, &len, stdin)) != -1) {
            printf("- ");
            if (text == NULL) {
                text = strdup(temp_read_value);
            } else {
                char * old_text = strdup(text);
                free(text);
                text = (char*)malloc(strlen(old_text) + strlen(temp_read_value) + 1);
                sprintf(text, "%s%s", old_text, temp_read_value);
                free(old_text);
            }
        }

        free(temp_read_value);
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
            printf("You cannot write to this stream.\n");
        }
    }
};

int main(int argc, char * argv[]) {
    if(argc == 1) exit(1);
    class PostEntry new_class;
    
    int bytes_to_malloc = 0;
    int i;
    for(i = 1;i < argc;i++){
        bytes_to_malloc += strlen(argv[i]);
    }

    new_class.username = (char*) malloc(bytes_to_malloc + 2);

    for(i = 1;i < argc;i++){
        if (new_class.username != NULL) {
            sprintf(new_class.username, "%s %s", new_class.username, argv[i]);
        }
    }

    new_class.readInput();
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
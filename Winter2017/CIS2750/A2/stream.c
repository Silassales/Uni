#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for access function
#include <ctype.h>

#include "stream.h"

// int main() { // for testing
//     userPost * temp = createPost("silas", "myStream", "1/1/1", "some stuff\nmore stuff\n");

//     updateStream(temp);

//     char string[] = "this,that,and,your,mom";
//     char string2[] = "this,that";
//     addUser("Silas", string);
//     removeUser("Silas", string2);

//     freePost(temp);
//     return 0;
// }


void updateStream (userPost * st) {
    if(st == NULL) return;
    char * filename = malloc(strlen("messages/") + strlen(st->stream_name) + strlen("Stream") + 1);
    sprintf(filename, "messages/%sStream", st->stream_name);
    char * data_filename = malloc(strlen("messages/") + strlen(st->stream_name) + strlen("StreamData") + 1);
    sprintf(data_filename, "messages/%sStreamData", st->stream_name);

    if(access(filename, F_OK) != 0) { // file does not exist
        createNeededFiles(st->stream_name);
    } 

    int num_of_bytes_written = writePostToStreamFile(filename, st);
    writeToDataFile(data_filename, num_of_bytes_written);

    free(filename);
    free(data_filename);
}


void addUser(char * username, char * list) {
    if (*username == ' ') username++;
    char * token = strtok(list, ",");
    while(token != NULL) {
        char * filename = malloc(strlen("messages/") + strlen(token) + strlen("Stream") + 1);
        sprintf(filename, "messages/%sStream", token);
        char * user_filename = malloc(strlen("messages/") + strlen(token) + strlen("StreamUsers") + 1);
        sprintf(user_filename, "messages/%sStreamUsers", token);

        if(access(filename, F_OK) != 0){
            createNeededFiles(token);
        }

        writeToUserFile(user_filename, username);

        token = strtok(NULL, ",");
        free(filename);
        free(user_filename);
    }
}


void removeUser(char * username, char * list) {
    if (*username == ' ') username++;
    char * token = strtok(list, ",");
    while(token != NULL) {
        char * filename = malloc(strlen("messages/") + strlen(token) + strlen("Stream") + 1);
        sprintf(filename, "messages/%sStream", token);
        char * user_filename = malloc(strlen("messages/") + strlen(token) + strlen("StreamUsers") + 1);
        sprintf(user_filename, "messages/%sStreamUsers", token);

        if(access(filename, F_OK) != 0){
            return; // cant remove users from a stream that doesnt exist
        }

        removeUserFromFile(user_filename, username);

        token = strtok(NULL, ",");
        free(filename);
        free(user_filename);
    }
}

userPost * createPost (char * username, char * stream_name, char * date, char * text) {
    if (*username == ' ') username++;
    userPost * temp_userpost = malloc(sizeof(userPost));

    temp_userpost->username = strdup(username);
    temp_userpost->stream_name = strdup(stream_name);
    temp_userpost->date = strdup(date);
    temp_userpost->text = strdup(text);

    return temp_userpost;
}

void freePost(userPost * temp_userpost) {
    free(temp_userpost->username);
    free(temp_userpost->stream_name);
    free(temp_userpost->date);
    free(temp_userpost->text);
    
    free(temp_userpost);
}

int writePostToStreamFile (char * filename, userPost * st) {
    if (*(st->username) == ' ') st->username++;
    FILE * fp = fopen(filename, "a");
    int num_of_bytes_written = 0;

    char * stuff_to_write = malloc(strlen("Sender: \n") + strlen(st->username) + strlen("Date: \n")
        + strlen(st->date) + strlen(st->text) + 1);
    sprintf(stuff_to_write, "Sender: %s\nDate: %s\n%s", st->username, st->date, st->text);
    
    fprintf(fp, "%s", stuff_to_write);

    /* reopen the file and count how many bytes have been written to it */
    fclose(fp);
    fp = fopen(filename, "r");
    while(getc(fp) != EOF) {
        num_of_bytes_written ++;
    }

    free(stuff_to_write);
    fclose(fp);
    return num_of_bytes_written;
}

void writeToDataFile (char * filename, int added_bytes) {
    FILE * fp = fopen(filename, "a");
    
    fprintf(fp, "%d\n", added_bytes);

    fclose(fp);
}

void writeToUserFile(char * filename, char * username) {
    if (*username == ' ') username++;
    FILE * fp = fopen(filename, "r+");

    int already_exists = 0;
    size_t num_chars = 100;
    char * temp_string = NULL;
    int error = 0;

    // make sure user doesnt already exist
    while(error != -1) {
        error = getline (&temp_string, &num_chars, fp);
        if(error == -1) break;
        char * name = getFirstValue(temp_string);

        if(strcmp(username, name) == 0) {
            already_exists = 1;
        }

        free(name);
    }
    
    free(temp_string);
    if(already_exists == 0) fprintf(fp, "%s 0\n", username);

    fclose(fp);
}

void removeUserFromFile(char * filename, char * username) {
    if (*username == ' ') username++;
    FILE * fp = fopen(filename, "r");

    // Read the file and write to a string all the names that arent the username, then print that back out into the file
    char * body = NULL;
    char * temp_body;
    size_t num_chars = 100;

    char * temp_string;
    char * name;
    int error = 0;

    while(error != -1) {
        temp_string = (char *) malloc (num_chars + 1);
        error = getline (&temp_string, &num_chars, fp);
        if(error == -1) break;
        name = getFirstValue(temp_string);
        
        if (body == NULL) {
            if(strcmp(name, username) != 0) body = strdup(temp_string);
        } else if (strcmp(name, username) != 0) {
            temp_body = body;
            body = malloc(strlen(temp_body) + strlen(temp_string) + 1);
            sprintf(body, "%s%s", temp_body, temp_string);
            free(temp_body);
        }

        free(temp_string);
        free(name);
    } 

    

    fclose(fp);
    fp = fopen(filename, "w");
    if (body != NULL) fprintf(fp, "%s", body);

    free(body);

    fclose(fp);
}


void createNeededFiles (char * stream_name) {
    char * stream_file_name = malloc(strlen("messages/") + strlen(stream_name) + strlen("Stream") + 1);
    sprintf(stream_file_name, "messages/%sStream", stream_name);
    char * data_file_name = malloc(strlen("messages/") + strlen(stream_name) + strlen("StreamData") + 1);
    sprintf(data_file_name, "messages/%sStreamData", stream_name);
    char * users_file_name = malloc(strlen("messages/") + strlen(stream_name) + strlen("StreamUsers") + 1);
    sprintf(users_file_name, "messages/%sStreamUsers", stream_name);

    /* we want to create new files (or overwrite old ones), even if they exist, this is because, if the nameStream file does not
    exist (only way to get to this function) that means there has been a corruption somewhere so we want
    to reset all files associated with this stream */
    fclose(fopen(stream_file_name, "w"));
    fclose(fopen(data_file_name, "w"));
    fclose(fopen(users_file_name, "w"));

    free(stream_file_name);
    free(data_file_name);
    free(users_file_name);
}

int streamExists (char * username) {
    if (*username == ' ') username++;
    char * stream_file_name = malloc(strlen("messages/") + strlen(username) + strlen("Stream") + 1);
    sprintf(stream_file_name, "messages/%sStream", username);

    if(access(stream_file_name, F_OK) != 0) {
        return 0;
    } else {
        return 1;
    }
}

int authorHasPermission(char * stream_name, char * username) {
    if (*username == ' ') username++;
    char * users_file_name = malloc(strlen("messages/") + strlen(stream_name) + strlen("StreamUsers") + 1);
    sprintf(users_file_name, "messages/%sStreamUsers", stream_name);
    
    // first check to see if stream even exists
    if(access(users_file_name, F_OK) != 0) {
        free(users_file_name);
        return 0;
    }

    // open file and check for the username
    FILE * fp = fopen(users_file_name, "r");
    int error = 0;
    size_t num_chars = 100;
    char * temp_string;

    while(error != -1) {
        temp_string = NULL;
        error = getline (&temp_string, &num_chars, fp);
        if(error == -1) break;

        char * name = getFirstValue(temp_string);

        if(strcmp(username, name) == 0) {
            free(users_file_name);
            free(temp_string);
            free(name);
            fclose(fp);
            return 1;
        }
        free(temp_string);
        free(name);
    }

    free(users_file_name);
    fclose(fp);
    return 0;
}

//returns malloced memory
char * getFirstValue(char * the_value) {
    char * temp = strdup(the_value);
    int length = strlen(temp);
    if(length <= 1) return NULL;
    int i;
    // loops until it finds a digit, places a null terminator and returns
    for(i = 0; i < length;i++) {
        if(isdigit(temp[i])) {
            temp[i - 1] = '\0';
            return temp;
        }
    }
}


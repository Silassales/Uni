#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //for access function
#include <ctype.h>
#include <mysql/mysql.h>

#include "stream.h"
#define MAX_QUERY 512
#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "ssales"
#define PASSWORD "0922973"
#define DATABASE "ssales"

void writePostToStreamFile (char * filename, userPost * st, MYSQL);

void writeToUserFile(char * filename, char * username, MYSQL);
void removeUserFromFile(char * filename, char * username, MYSQL);

// int main() { // for testing
//     //  userPost * temp = createPost("silas", "myStream", "1/1/1", "some stuff\nmore stuff\n");

//     //  //updateStream(temp);

//     //  char string[] = "this,that,and,your,mom";
//     //  char string2[] = "this,that,asdfasdfasdf";
//     //  addUser("Silas", string);
//     //  removeUser("Silas", string2);

//     //  freePost(temp);

//      printf("Res: %d\n", authorHasPermission("this", "bob"));
//      return 0;
// }


void updateStream (userPost * st) {
    char query[MAX_QUERY];
    MYSQL mysql;
    MYSQL_RES * result;
    MYSQL_ROW row;
    
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
        exit(1);
    }
    if(st == NULL) return;
    char * filename = malloc(strlen(st->stream_name) + strlen("Stream") + 1);
    sprintf(filename, "%sStream", st->stream_name);

    /* check if the table exists */
    sprintf(query, "select table_name from information_schema.tables where table_name=\'%s\'", filename);
    if(mysql_query(&mysql, query)) {
        printf("%s\n", mysql_error(&mysql));
        exit(1);
    }
    if(!(result = mysql_store_result(&mysql))) {
        printf("%s\n", mysql_error(&mysql));
        exit(1);
    }

    createNeededFiles(st->stream_name);

    writePostToStreamFile(filename, st, mysql);

    free(filename);
    mysql_free_result(result);
    mysql_close(&mysql);
}


void addUser(char * username, char * list) {
    MYSQL mysql;
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
        exit(1);
    }

    if (*username == ' ') username++;
    char * token = strtok(list, ",");
    while(token != NULL) {
        char * filename = malloc(strlen("") + strlen(token) + strlen("Stream") + 1);
        sprintf(filename, "%sStream", token);
        char * user_filename = malloc(strlen("") + strlen(token) + strlen("Users") + 1);
        sprintf(user_filename, "%sUsers", token);

        createNeededFiles(token);

        writeToUserFile(user_filename, username, mysql);

        token = strtok(NULL, ",");
        free(filename);
        free(user_filename);
    }
    mysql_close(&mysql);
}


void removeUser(char * username, char * list) {
    MYSQL mysql;
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
        exit(1);
    }

    if (*username == ' ') username++;
    char * token = strtok(list, ",");
    while(token != NULL) {
        char * filename = malloc(strlen("") + strlen(token) + strlen("Stream") + 1);
        sprintf(filename, "%sStream", token);
        char * user_filename = malloc(strlen("") + strlen(token) + strlen("Users") + 1);
        sprintf(user_filename, "%sUsers", token);

        removeUserFromFile(user_filename, username, mysql);

        token = strtok(NULL, ",");
        free(filename);
        free(user_filename);
    }

    mysql_close(&mysql);
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

void writePostToStreamFile (char * filename, userPost * st,  MYSQL mysql) {
    char query[MAX_QUERY];

    if (*(st->username) == ' ') st->username++;
    
    sprintf(query, "insert into %s values (\'%s\',\'%s\',\'%s\',\'%s\')", filename, st->username, st->stream_name, st->date, st->text);
    if(mysql_query(&mysql, query)) {
        printf("%s\n", mysql_error(&mysql));
    }
}

void writeToUserFile(char * filename, char * username,  MYSQL mysql) {
    MYSQL_RES * result;
    MYSQL_ROW row;
    char query[MAX_QUERY];
    int i;
    if (*username == ' ') username++;

    int already_exists = 0;

    // make sure user doesnt already exist
    query[0] = '\0';
    sprintf(query, "select username from %s", filename);
    if(mysql_query(&mysql, query)) {
        printf("%s\n", mysql_error(&mysql));
    }
    if(!(result = mysql_store_result(&mysql))) {
        printf("%s\n", mysql_error(&mysql));
    }
    while((row = mysql_fetch_row(result))) {
        for(i=0;i<mysql_num_fields(result);i++) {
            if(strcmp(username, row[i]) == 0) {
                already_exists = 1;
            }
        }
    }
    
    if(already_exists == 0) {
        query[0] = '\0';
        sprintf(query, "insert into %s values (\'%s\',\'0\')", filename, username);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
    }
}

void removeUserFromFile(char * filename, char * username,  MYSQL mysql) {
    MYSQL_RES * result;
    MYSQL_ROW row;
    char query[MAX_QUERY];
    int i;

    if (*username == ' ') username++;

    query[0] = '\0';
    sprintf(query, "delete from %s where username=\'%s\'", filename, username);
    if(mysql_query(&mysql, query)) {
        //printf("%s\n", mysql_error(&mysql));
    }

}


void createNeededFiles (char * stream_name) {
    MYSQL mysql;
    MYSQL_RES * result;
    MYSQL_ROW row;
    char query[MAX_QUERY];
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
        exit(1);
    }

    char * stream_file_name = malloc(strlen("") + strlen(stream_name) + strlen("Stream") + 1);
    sprintf(stream_file_name, "%sStream", stream_name);
    char * users_file_name = malloc(strlen("") + strlen(stream_name) + strlen("Users") + 1);
    sprintf(users_file_name, "%sUsers", stream_name);

    query[0] = '\0';
    sprintf(query, "create table %s (username varchar(256), streamname varchar(256), date varchar(256), text text)", stream_file_name);
    if(mysql_query(&mysql, query)) {
        return;
    }
    query[0] = '\0';
    sprintf(query, "create table %s (username varchar(256), lastread int)", users_file_name);
    if(mysql_query(&mysql, query)) {
        return;
    }

    free(stream_file_name);
    free(users_file_name);
    mysql_close(&mysql);
}

int streamExists (char * username) {
    if (*username == ' ') username++;
    char * stream_file_name = malloc(strlen("") + strlen(username) + strlen("Stream") + 1);
    sprintf(stream_file_name, "%sStream", username);

    if(access(stream_file_name, F_OK) != 0) {
        return 0;
    } else {
        return 1;
    }
}

int authorHasPermission(char * stream_name, char * username) {
    char query[MAX_QUERY];
    int i;
    MYSQL mysql;
    MYSQL_RES * result;
    MYSQL_ROW row;
    
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
        exit(1);
    }

    if (*username == ' ') username++;
    char * users_file_name = malloc(strlen("") + strlen(stream_name) + strlen("Users") + 1);
    sprintf(users_file_name, "%sUsers", stream_name);
    
    query[0] = '\0';
    sprintf(query, "select username from %s", users_file_name);
    if(mysql_query(&mysql, query)) {
        printf("%s\n", mysql_error(&mysql));
    }
    if(!(result = mysql_store_result(&mysql))) {
        printf("%s\n", mysql_error(&mysql));
    }
    while((row = mysql_fetch_row(result))) {
        for(i=0;i<mysql_num_fields(result);i++) {
            if(strcmp(username, row[i]) == 0) {
                return 1;
            }
        }
    }

    mysql_free_result(result);
    mysql_close(&mysql);
    mysql_library_end();
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


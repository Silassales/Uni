#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME "dursley.socs.uoguelph.ca"
#define USERNAME "ssales"
#define PASSWORD "0922973"
#define DATABASE "ssales"

int main(int argc, char * argv[]) {
    int i;
    int j;

    char * command = argv[1];

    MYSQL mysql;
    MYSQL_RES * result;
    MYSQL_RES * second_result;
    MYSQL_ROW row;
    MYSQL_ROW second_row;
    MYSQL_FIELD * field;

    char query[MAX_QUERY];

    /* set up the database and connect */
    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if(!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD, DATABASE, 0, NULL, 0)) {
        printf("Could not connect to host.\n%s\n", mysql_error(&mysql));
        exit(1);
    }

    if(strcmp(command, "-clear") == 0) {
        query[0] = '\0';
        strcpy(query, "show tables");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                query[0] = '\0';
                sprintf(query, "truncate table %s", row[i]);
                if(mysql_query(&mysql, query)) {
                    printf("%s\n", mysql_error(&mysql));
                }
                printf("Trucated table: %s\n", row[i]);
            }
        }
    } else if(strcmp(command, "-reset") == 0) {
        query[0] = '\0';
        strcpy(query, "show tables");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                query[0] = '\0';
                sprintf(query, "drop table %s", row[i]);
                if(mysql_query(&mysql, query)) {
                    printf("%s\n", mysql_error(&mysql));
                }
                printf("Dropped table: %s\n", row[i]);
            }
        }
    } else if(strcmp(command, "-posts") == 0) {
        query[0] = '\0';
        strcpy(query, "select table_name from information_schema.tables where table_name like \'%Stream%%\'");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                query[0] = '\0';
                sprintf(query, "select * from %s", row[i]);
                if(mysql_query(&mysql, query)) {
                    printf("%s\n", mysql_error(&mysql));
                }
                if(!(second_result = mysql_store_result(&mysql))) {
                    printf("%s\n", mysql_error(&mysql));
                }
                while((second_row = mysql_fetch_row(second_result))) {
                    for(j=0;j<mysql_num_fields(second_result);j++) {
                        printf("%s\n", second_row[j]);
                    }
                }
            }
        }
    } else if(strcmp(command, "-users") == 0) {
        query[0] = '\0';
        strcpy(query, "select table_name from information_schema.tables where table_name like \'%Users%%\'");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                query[0] = '\0';
                sprintf(query, "select username from %s", row[i]);
                if(mysql_query(&mysql, query)) {
                    printf("%s\n", mysql_error(&mysql));
                }
                if(!(second_result = mysql_store_result(&mysql))) {
                    printf("%s\n", mysql_error(&mysql));
                }
                while((second_row = mysql_fetch_row(second_result))) {
                    for(j=0;j<mysql_num_fields(second_result);j++) {
                        printf("%s\n", second_row[j]);
                    }
                }
            }
        }
    } else if(strcmp(command, "-streams") == 0) {
        query[0] = '\0';
        strcpy(query, "select table_name from information_schema.tables where table_name like \'%Stream%%\'");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("Stream %d: %s\n", i+1, row[i]);
            }
        }
    } else if(strcmp(command, "-help") == 0) {
        puts("Run with -clear to clear all info in tables");
        puts("Run with -reset to remove all table in database");
        puts("Run with -posts to print out all posts stored within database");
        puts("Run with -users to print out all users within database");
        puts("Run with -stream to print out all the stream names within database");
    } else if (strcmp(command, "getUserStreams") == 0) {
        char * username = argv[2];

        query[0] = '\0';
        strcpy(query, "select table_name from information_schema.tables where table_name like \'%Users%%\'");
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                query[0] = '\0';
                sprintf(query, "select username from %s", row[i]);
                if(mysql_query(&mysql, query)) {
                    printf("%s\n", mysql_error(&mysql));
                }
                if(!(second_result = mysql_store_result(&mysql))) {
                    printf("%s\n", mysql_error(&mysql));
                }
                while((second_row = mysql_fetch_row(second_result))) {
                    for(j=0;j<mysql_num_fields(second_result);j++) {
                        if(strcmp(username, second_row[j]) == 0) {
                            printf("%s\n", row[i]);
                        }
                    }
                }
            }
        }
    } else if (strcmp(command, "getStreamData") == 0) {
        char * streamname = argv[2];

        query[0] = '\0';
        sprintf(query, "select text from %sStream", streamname);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("%s\n", row[i]);
            }
        }

    } else if (strcmp(command, "getUsernameData") == 0) {
        char * streamname = argv[2];

        query[0] = '\0';
        sprintf(query, "select username from %sStream", streamname);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("%s\n", row[i]);
            }
        }

    } else if (strcmp(command, "getDateData") == 0) {
        char * streamname = argv[2];

        query[0] = '\0';
        sprintf(query, "select date from %sStream", streamname);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("%s\n", row[i]);
            }
        }

    } else if (strcmp(command, "getStreamNameData") == 0) {
        char * streamname = argv[2];

        query[0] = '\0';
        sprintf(query, "select streamname from %sStream", streamname);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("%s\n", row[i]);
            }
        }

    } else if (strcmp(command, "getLastReadData") == 0) {
        char * stream_name = argv[2];
        char * username = argv[3];

        query[0] = '\0';
        sprintf(query, "select lastread from %sUsers where username=\'%s\'", stream_name, username);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("%s\n", row[i]);
            }
        }

    } else if (strcmp(command, "MarkAllAsRead") == 0) {
        char * stream_name = argv[2];
        char * username = argv[3];
        int lastread = 0;

        query[0] = '\0';
        sprintf(query, "select count(*) from %sStream", stream_name);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                lastread = strtol(row[i], NULL, 10);
            }
        }
        query[0] = '\0';
        sprintf(query, "update %sUsers set lastread=\'%d\' where username=\'%s'", stream_name, (int)lastread, username);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }

    } else if (strcmp(command, "updateUserData") == 0) {
        char * stream_name = argv[2];
        char * username = argv[3];
        int value_to_update = (int) strtol(argv[4], NULL, 10);

        query[0] = '\0';
        sprintf(query, "update %sUsers set lastread=\'%d\' where username=\'%s'", stream_name, value_to_update, username);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }

    } else if (strcmp(command, "getUserData") == 0) {
        char * stream_name = argv[2];
        char * username = argv[3];

        query[0] = '\0';
        sprintf(query, "select lastread from %sUsers where username=\'%s\'", stream_name, username);
        if(mysql_query(&mysql, query)) {
            printf("%s\n", mysql_error(&mysql));
        }
        if(!(result = mysql_store_result(&mysql))) {
            printf("%s\n", mysql_error(&mysql));
        }
        while((row = mysql_fetch_row(result))) {
            for(i=0;i<mysql_num_fields(result);i++) {
                printf("%s", row[i]);
            }
        }

    } else {
        printf("Invalid command arg provided: %s\n", command);
    }

    /*close the database and disconnect*/
    mysql_close(&mysql);
    mysql_library_end();

    return 0;
}
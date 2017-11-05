struct userPost {
    char * username;
    char * stream_name;
    char * date;
    char * text;
}; typedef struct userPost userPost;

/* From asst descrip:
The updateStream() function adds a new posting
- to the stream specified in streamname. If the stream does not exist then
- it creates the files necessary for the stream. You will need to determine
- the time and date of the post, format it as a string, and store it
- in the date string.
*/
void updateStream (userPost * st);

/* From asst descrip:
The addUser() function adds a user with username to all of the streams
- listed in the list string. The list string is a comma separated list of
- stream names. If a stream name in the list does not exist then a new
- stream with that name should be created.
*/
void addUser(char * username, char * list);

/* From asst descrip:
The removeUser() function
- removes the username from the stream(s) in the list.
*/
void removeUser(char * username, char * list);

// User defined/helper functions

//Functions for creation and modification of struct

userPost * createPost (char * username, char * stream_name, char * date, char * text);

void freePost(userPost *);

// Helper functions for updateStream
// returns the number of bytes added to file
int writePostToStreamFile (char * filename, userPost * st);

void writeToDataFile (char * filename, int added_bytes);

void writeToUserFile(char * filename, char * second_file, char * username);

void removeUserFromFile(char * filename, char * second_file, char * username);

void createNeededFiles (char * stream_name);

// Functions for determining user permissions 

int streamExists (char * username);

int authorHasPermission(char * stream_name, char * username);

// Miscelanous Functions

char * getFirstValue(char * the_value);

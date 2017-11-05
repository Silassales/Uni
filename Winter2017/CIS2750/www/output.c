#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"
struct PostEntry {
	userPost* the_post;
	char* username;
	char* stream_name;
	char* date;
	char* text;
	void (*PostEntryreadInputsccPtr)(struct PostEntry *, char*, char*);
	void (*PostEntryformatEntrysPtr)(struct PostEntry *);
	void (*PostEntrygetTimeDatesPtr)(struct PostEntry *);
	void (*PostEntrysubmitPostsPtr)(struct PostEntry *);
};

void PostEntryreadInputscc (struct PostEntry * the_PostEntry, char* stream, char* text_value) {
	the_PostEntry->stream_name = strdup ( stream ) ;
	the_PostEntry->text = strdup ( text_value ) ;
	}
	

void PostEntryformatEntrys (struct PostEntry * the_PostEntry) {
	the_PostEntry->the_post = createPost ( the_PostEntry->username , the_PostEntry->stream_name , the_PostEntry->date , the_PostEntry->text ) ;
	}
	

void PostEntrygetTimeDates (struct PostEntry * the_PostEntry) {
	// ref: http://stackoverflow.com/questions/3673226/how-to-print-time-in-format-2009-08-10-181754-811
	time_t the_time ;
	the_PostEntry->date = ( char* ) malloc ( sizeof ( char ) * 26) ;
	struct tm* tm_info ;
	time ( &the_time ) ;
	tm_info = localtime ( &the_time ) ;
	strftime ( the_PostEntry->date , 26, "%m/%d/%y @ %H:%M:%S" , tm_info ) ;
	}
	

void PostEntrysubmitPosts (struct PostEntry * the_PostEntry) {
	if ( authorHasPermission ( the_PostEntry->stream_name , the_PostEntry->username ) ) {
	updateStream ( the_PostEntry->the_post ) ;
	}
	else {
	printf ( "CANT_WRITE" ) ;
	}
	}
	

void constructorPostEntry (struct PostEntry * the_PostEntry) {
	the_PostEntry->PostEntryreadInputsccPtr = &PostEntryreadInputscc;
	the_PostEntry->PostEntryformatEntrysPtr = &PostEntryformatEntrys;
	the_PostEntry->PostEntrygetTimeDatesPtr = &PostEntrygetTimeDates;
	the_PostEntry->PostEntrysubmitPostsPtr = &PostEntrysubmitPosts;
}

int main (int argc, char* argv[]) {
	//if(argc == 1) exit(1)
	struct PostEntry new_class ;
	constructorPostEntry(&new_class);
	int bytes_to_malloc = 0;
	int i ;
	for ( i = 1;
	i < 2;
	i++ ) {
	bytes_to_malloc += strlen ( argv[i] ) ;
	}
	new_class .username = ( char* ) malloc ( bytes_to_malloc + 2) ;
	for ( i = 1;
	i < 2;
	i++ ) {
	if ( new_class .username !=  NULL ) {
	sprintf ( new_class .username , "%s %s" , new_class .username , argv[i] ) ;
	}
	}
	new_class .PostEntryreadInputsccPtr ( &new_class , argv[2] , argv[3] ) ;
	new_class .PostEntrygetTimeDatesPtr ( &new_class ) ;
	new_class .PostEntryformatEntrysPtr ( &new_class ) ;
	new_class .PostEntrysubmitPostsPtr ( &new_class ) ;
	freePost ( new_class .the_post ) ;
	free ( new_class .username ) ;
	free ( new_class .stream_name ) ;
	free ( new_class .date ) ;
	free ( new_class .text ) ;
	return 0;
	}
	


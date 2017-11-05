Silas Sales
0922973

Special Notes about my program:

Any spaces entered when entering the stream names in a list will result
in a space within the file name of the created stream.

The .c files create a rather large amount of valgrind errors but do not 
have memory leaks

** If all posts have been read than the last post will be displayed unless mark all ass 
read is used and then the first one will be displayed**

The program will seg fault if the messages folder is deleted.

Unspecified behaviour if the files in the messages folder are modified 
without modifying them through the post/addauthor programs

For the wpml parser:

extra text needs to be added after all other specifed tags
tags need to be entered in the order that they are in the spec
the extra tags for addauthor and post are .a and .z, both dont have any tags and will not do anything with extra text entered within them
.e will not output extra text within the tags

For a3/a4 specifically:

You need to press the forward and back buttons twice when switching from forward to back or vise versa
after adding a new user to a post, if that user is the current user or that post is the current post, you need to go back to the select user/stream page (using the switch user or change stream buttons) in order to change the permissions of the view screen

doesnt check db for args validity due to using custom args throughout the assignment, so unspecified 
behaviour is you use anything outside of commands listed through -help

If you add spaces at the end of a username ex. 'bob ', then you have to use that space when
using the website throughout the session (viewing posts, posting, etc).

I have one file that I still use, literally just for storing whether the stream is sorted by 
date or by name (sortFile.txt). I didnt know if this fell under the required files that needed to be in the
database, but I figured that it wasnt neseccary as it wasnt part of the spec for a3, therefore
doesnt fall under the required tables for a4.

If a user is subscribed to multiple streams, using ./db -users will print out duplicates

** super annoying but found this bug a without enough time to have it worth fixing without 
risking making it worse. As when you first load the webpage, and add new authors to the stream
you have to then go and select stream again with the author and stream you want, before posting any
messages, messages will apear to post, but wont actually write to database until you go back
and reselect the user/stream. I appologize. 
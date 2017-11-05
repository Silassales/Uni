#!/usr/bin/python3

import sys
import glob
import os
import subprocess
from operator import attrgetter
import copy
import tty, termios

# my classes

class Post(object):
    
    def __init__(self, stream, sender, date, body, pos_within_stream):
        self.stream = stream
        self.sender = sender
        self.sender_to_lower = sender.lower()
        self.pos_within_string = -1
        self.pos_within_stream = pos_within_stream
        self.date = date
        self.date_code = self.getDateCode(date)
        self.body = body

    def __str__(self):
        return "Stream: " + self.stream + "\nSender: " + self.sender + "\nDate: " + self.date + "\n" + self.body + "\n"

    def getDateCode(self, date):
        date = date.replace("/", "")
        date = date.replace(" ", "")
        date = date.replace("@", "")
        date = date.replace(":", "")

        month = date[0:2]
        day = date[2:4]
        year = date[4:6]
        hour = date[6:8]
        minute = date[8:10]
        second = date[10:12]

        date_code = year + month + day + hour + minute + second

        date_code = int(date_code)

        return date_code

class Stream(object):
    
    def __init__(self, name, posts):
        self.name = name
        self.posts = posts
        self.originalOrder = copy.deepcopy(posts)
        self.post_indexes = []
        self.sorted_by = "D"
        self.last_read = 0

    def __str__(self):
        string_value = ""
        for post in self.posts:
            string_value += str(post)
        return string_value

    def sortByUsername(self):
        self.posts.sort(key=attrgetter('sender_to_lower'))
        self.sorted_by = "N"
    
    def sortByDate(self):
        self.posts = copy.deepcopy(self.originalOrder)
        self.sorted_by = "D"
    
    def getNumberOfStreams(self):
        return len(self.posts)

class getch:
    def __init__(self):
        self.run = unix()

    def __call__(self):
        list_of_chars = []
        #try to get three chars -> up/down arrow keys need 3 chars
        for i in range(3):
            try:list_of_chars.append(self.run())
            except:pass
            #check the ranges
            if list_of_chars[i] not in [chr(27),chr(91)]:
                break
            if len(list_of_chars) > 1:
                if list_of_chars == [chr(27),chr(27)]:
                    break
        if len(list_of_chars) == 1:
            return list_of_chars[0]
        if len(list_of_chars) == 3:
            #check for up and down chars 
            if list_of_chars[2] == 'A':
                return 'Up'
            if list_of_chars[2] == 'B':
                return 'Down'
        return ''

class unix:
   def __call__(self):
      fd = sys.stdin.fileno()
      term_param = termios.tcgetattr(fd)
      try:
         tty.setraw(sys.stdin.fileno())
         ch = sys.stdin.read(1)
      finally:
         termios.tcsetattr(fd, termios.TCSADRAIN, term_param)
      return ch
def stripLastToken(line):
    temp = line

    temp = temp.split(" ")
    temp = temp[:len(temp) - 1]

    final = " ".join(temp)
    return final
    
def getUserStreams(username):
    available_streams = []
    result = subprocess.Popen(["./db", "getUserStreams", username], stdout=subprocess.PIPE)

    for i in result.stdout:
        available_streams.append(i.decode("utf-8").replace("Users\n","",1))
    return available_streams


#stores all of the posts within a stream and then returns a stream object with all of those 
#posts stored
def getAndStoreStreamData(stream_name):
    #list of post objects
    list_of_posts = []
    list_of_text = []
    list_of_username = []
    list_of_date = []
    list_of_streamname = []

    result = subprocess.Popen(["./db", "getStreamData", stream_name], stdout=subprocess.PIPE)

    for i in result.stdout:
        if str(i.decode("utf-8")) != "\n":
            list_of_text.append(i.decode("utf-8").strip())

    result = subprocess.Popen(["./db", "getUsernameData", stream_name], stdout=subprocess.PIPE)

    for i in result.stdout:
        if str(i.decode("utf-8")) != "\n":
            list_of_username.append(i.decode("utf-8").strip())
    
    result = subprocess.Popen(["./db", "getDateData", stream_name], stdout=subprocess.PIPE)

    for i in result.stdout:
        if str(i.decode("utf-8")) != "\n":
            list_of_date.append(i.decode("utf-8").strip())
    
    result = subprocess.Popen(["./db", "getStreamNameData", stream_name], stdout=subprocess.PIPE)

    for i in result.stdout:
        if str(i.decode("utf-8")) != "\n":
            list_of_streamname.append(i.decode("utf-8").strip())

    for i in range(len(list_of_text)):
        new_post = Post(list_of_streamname[i], list_of_username[i], list_of_date[i], list_of_text[i], i)
        list_of_posts.append(new_post)

    new_stream = Stream(stream_name, list_of_posts)

    return new_stream

def getStartingPos(stream_list, list_of_posts, username):
    last_read_messages = []
    for stream in stream_list:
        result = subprocess.Popen(["./db", "getLastReadData", stream.name, username], stdout=subprocess.PIPE)

        for i in result.stdout:
            pos = int(i.decode("utf-8"))
        try: 
            last_read_messages.append(stream.posts[pos].pos_within_string)
        except:
            pass 

    try: most_unread_message = last_read_messages[0]
    except: most_unread_message = 0
    for pos in last_read_messages:
        if (pos < most_unread_message):
            most_unread_message = pos

    most_unread_message -= 10 #we added 10 to the pos to make it easer to find ranges
    if (most_unread_message < 0): most_unread_message = 0 #make sure we didnt go out of bounds 

    return most_unread_message
   
def updateAllUserFiles(pos_min, pos_max, the_posts, username): 
    last_post = 0       
    for post in the_posts:

        if ((pos_min < post.pos_within_string) and (pos_max > post.pos_within_string) and (post.pos_within_string != -1)):
            result = subprocess.Popen(["./db", "getUserData", post.stream, username], stdout=subprocess.PIPE)

            for i in result.stdout:
                last_post = int(i.decode("utf-8"))
            if (post.pos_within_stream > last_post): #this post is the most recently read post
                last_post = post.pos_within_stream
                result = subprocess.Popen(["./db", "updateUserData", post.stream, username, str(last_post)], stdout=subprocess.PIPE)

                for i in result.stdout:
                    last_post = int(i.decode("utf-8"))

def printNextPost(the_string, the_pos):
    #get the exact number of lines we want printed out of the_string
    number_of_char_printed = 0
    number_of_post = 0

    substring = the_string[int(the_pos):]
    substring_lines = substring.split("\n")

    for line in substring_lines:
        if "Stream: " in line:
            number_of_post += 1
            if (number_of_post > 1):
                return number_of_char_printed
            else:
                print(line)
                number_of_char_printed += len(line) + 1
        else:
            print(line)
            number_of_char_printed += len(line) + 1

    return number_of_char_printed

def printPrevPost(the_string, the_pos):
    #get the exact number of lines we want printed out of the_string
    string_to_print = ""
    substring_lines = the_string[0:the_pos].split("\n")
    number_of_char_printed = 0
    temp_string = ""
    if(the_pos == -1):
        printNextPost(the_string, 0)
        return -1

    for i in range(len(substring_lines) - 1, -1, -1):
        if "Stream: " in substring_lines[i]:
            number_of_char_printed += len(substring_lines[i]) + 1
            temp_string = "\n" + substring_lines[i] + string_to_print
            string_to_print = temp_string
            print(string_to_print)
            return number_of_char_printed
        else:
            number_of_char_printed += len(substring_lines[i]) + 1
            temp_string = "\n" + substring_lines[i] + string_to_print
            string_to_print = temp_string

    printNextPost(the_string, 0)
    return -1

def markAllAsRead(username, the_streams):
    #Get the number of streams from the stream object, then update the user file with that
    for the_stream in the_streams:
        subprocess.Popen(["./db", "MarkAllAsRead", the_stream.name, username])

def sortAllPostsByName(list_of_posts):
    list_of_posts.sort(key=attrgetter('sender_to_lower'))
    return list_of_posts

def sortAllPostsByDate(list_of_posts):
    list_of_posts.sort(key=attrgetter('date_code'))

    #store the value of the positions within the posts
    global_pos = 0
    for post in list_of_posts:
        post.pos_within_string = global_pos + 10 #10 so we give ourselves a bit of a buffer for determining location within 
        #a range, 10 is fine as each post will have alteast 14 characters of sender, date, so we wont go within another
        #posts range
        global_pos += len(post.stream) + len(post.body) + len(post.sender) + len(post.date) + 25 #25 for the lengths of Stream, Sender, Date

    return list_of_posts

def writeCurrentPost(username, the_streams, command, curpos):
    with open("sortFile.txt", "r+") as tempFile:
            for line in tempFile:
                if line == "D":
                    sorted_by = "D"
                else:
                    sorted_by = "N"

    stream_list = []
    for desired_stream in the_streams:
        #store all the stream values
        new_stream = getAndStoreStreamData(desired_stream)
        list_of_indexes = []
        for post in new_stream.posts:
            list_of_indexes.append(len(str(post)))
        new_stream.post_indexes = list_of_indexes
        stream_list.append(new_stream)

    list_of_posts = []
    #get a list of posts so its easier to sort when needed
    for stream in stream_list:
        for post in stream.posts:
            list_of_posts.append(post)
    list_of_posts = sortAllPostsByDate(list_of_posts)
    orig_list_of_posts = copy.deepcopy(list_of_posts)

    stream_string = ""

    for post in list_of_posts:
        stream_string += str(post)

    current_printing_pos = getStartingPos(stream_list, list_of_posts, username)

    if(command == 'O'):
        with open("sortFile.txt", "r+") as tempFile:
            for line in tempFile:
                if line == "D":
                        #sort by dates then re-display
                    list_of_posts = sortAllPostsByName(list_of_posts)
                    with open("sortFile.txt", "w+") as another_tempFile:
                        another_tempFile.write("N")
                    stream_string = ""
                    for post in list_of_posts:
                        #get the string representation of the Stream
                        stream_string += str(post)
                    current_printing_pos = printNextPost(stream_string, 0)
                    print("CUR_POS:" + str(current_printing_pos))
                else: 
                    with open("sortFile.txt", "w+") as another_tempFile:
                        another_tempFile.write("D")
                    for post in list_of_posts:
                        #get the string representation of the Stream
                        stream_string += str(post)
                    current_printing_pos = printNextPost(stream_string, 0)
                    print("CUR_POS:" + str(current_printing_pos))            
    elif(command == 'M'):
        markAllAsRead(username, stream_list)
    elif(command == 'C'):
        stream_list = []
        for desired_stream in the_streams:
            #store all the stream values
            new_stream = getAndStoreStreamData(desired_stream)
            stream_list.append(new_stream)

        list_of_posts = []
        #get a list of posts so its easier to sort when needed
        for stream in stream_list:
            for post in stream.posts:
                list_of_posts.append(post)
        orig_list_of_posts = copy.deepcopy(list_of_posts)
        
        stream_string = ""
        for post in list_of_posts:
            #get the string representation of the Stream
            stream_string += str(post)
        current_printing_pos = getStartingPos(stream_list, list_of_posts, username)
        old_pos = current_printing_pos
        if (sorted_by == "N"):
            list_of_posts = sortAllPostsByName(list_of_posts)
            stream_string = ""
            for post in list_of_posts:
                #get the string representation of the Stream
                stream_string += str(post)
        current_printing_pos += printNextPost(stream_string, old_pos)
        if(new_stream.sorted_by is "D"): updateAllUserFiles(old_pos, current_printing_pos, list_of_posts, username)
    elif(command == 'S'):
        fake_args = []
        fake_args.append("blah")
        fake_args.append(username)
        #clear screen
        for i in range(23):
            print()
        print("Streams:")
        main(fake_args)
    elif(command == "Forward"):
        current_printing_pos = int(curpos)
        old_pos = current_printing_pos
        if (sorted_by == "N"):
            list_of_posts = sortAllPostsByName(list_of_posts)
            stream_string = ""
            for post in list_of_posts:
                #get the string representation of the Stream
                stream_string += str(post)
        return_val = printNextPost(stream_string, current_printing_pos)
        if (return_val != -1): current_printing_pos += return_val
        if(new_stream.sorted_by is "D"): updateAllUserFiles(old_pos, current_printing_pos, list_of_posts, username)
        #return the value to the php
        print("CUR_POS:" + str(current_printing_pos))
    elif(command == "Backwards"):
        #just print at the new_pos, 24 lines after the first printLines call
        current_printing_pos = int(curpos)
        old_pos = current_printing_pos
        if (sorted_by == "N"):
            list_of_posts = sortAllPostsByName(list_of_posts)
            stream_string = ""
            for post in list_of_posts:
                #get the string representation of the Stream
                stream_string += str(post)
        return_val = printPrevPost(stream_string, current_printing_pos)
        if(return_val != -1): current_printing_pos -= return_val
        else: current_printing_pos = 0
        #return the value to the php
        print("CUR_POS:" + str(current_printing_pos))
    else:
        old_pos = current_printing_pos
        if (sorted_by == "N"):
            list_of_posts = sortAllPostsByName(list_of_posts)
            stream_string = ""
            for post in list_of_posts:
                #get the string representation of the Stream
                stream_string += str(post)
        current_printing_pos += printNextPost(stream_string, old_pos)
        if(new_stream.sorted_by is "D"): updateAllUserFiles(old_pos, current_printing_pos, list_of_posts, username)
        #return the value to the php
        print("CUR_POS:" + str(current_printing_pos))

def main(argv):
    username = argv[1]
    available_streams = getUserStreams(username)

    desired_stream = argv[2] 
    command = argv[3]
    curpos = argv[4]

    if(len(available_streams) == 0):
         print("Can't access this stream")
         sys.exit(1)

    temp_array = []
    temp_array.append(desired_stream)

    if (desired_stream == "all"):
        writeCurrentPost(username, available_streams, command, curpos)
    elif (desired_stream not in available_streams):
        print("Can't access this stream")
        sys.exit(1)
    else:
        writeCurrentPost(username, temp_array, command, curpos)

if __name__ == "__main__":
    main(sys.argv)
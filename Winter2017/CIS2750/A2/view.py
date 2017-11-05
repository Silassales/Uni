#!/usr/bin/python3

import sys
import glob
import os
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
        return "Stream: " + self.stream + "\nSender: " + self.sender + "\nDate: " + self.date + "\n" + self.body

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
    
def printOutUserStreams(username):
    available_streams = []
    for filename in glob.glob('messages/*'):
        if "Users" in filename:
            with open(filename, "r") as userFile:
                for line in userFile:
                    line = stripLastToken(line)
                    if (username == line):
                        #format the filename to get the streamname
                        streamName = filename.replace("messages/", "")
                        streamName = streamName.replace("StreamUsers", "")
                        available_streams.append(streamName)
                        print(streamName, end=' ')
    if (len(available_streams) != 0): print("all", end = '')
    else: print("You are not subscribed to any streams.", end = '')
    print() #print out the final newline
    return available_streams


#stores all of the posts within a stream and then returns a stream object with all of those 
#posts stored
def getAndStoreStreamData(stream_name):
    #list of post objects
    list_of_posts = []
    
    #get the entire string value of the file
    filename = "messages/" + stream_name + "Stream"
    with open(filename, "r") as stream_file:
        entire_file = stream_file.read()

    #get the postioning of the posts from the dataFile
    data_filename = "messages/" + stream_name + "StreamData"
    with open(data_filename, "r") as data_file:
        list_of_post_indexes = data_file.read().split("\n")
        #we have and extra index because of the last newline, so remove it
        list_of_post_indexes.pop()

    #start parsing entire_file and storing it in posts/streams
    
    #first create substrings of all the different streams
    previous_index = 0
    substring_list = []
    for i in list_of_post_indexes:
        substring_list.append(entire_file[int(previous_index):int(i)])
        previous_index = i

    #loop through the list of strings and create posts, then add those posts to the list of posts 
    post_num = 0
    for post in substring_list:
        post_num += 1
        #get the first 2 lines, sender, date
        first_two_lines = post.split("\n", 2)
        sender = first_two_lines[0].replace("Sender: ", "", 1)
        date = first_two_lines[1].replace("Date: ", "", 1)
        #now get the body by removing sender and date
        post = post.replace(first_two_lines[0] + "\n", "", 1)
        post = post.replace(first_two_lines[1] + "\n", "", 1)
        body = post
        
        new_post = Post(stream_name.replace("Stream", "", 1), sender, date, body, post_num)
        list_of_posts.append(new_post)

    new_stream =  Stream(stream_name.replace("Stream", "", 1), list_of_posts)
    #save the indexes values so we have a record of them and dont have to get them later on
    new_stream.post_indexes = list_of_post_indexes
    return new_stream

def getStartingPos(stream_list, list_of_posts, username):
    last_read_messages = []
    for stream in stream_list:
        users_filename = "messages/" + stream.name + "StreamUsers"
        with open(users_filename, "r") as userFile:
            for line in userFile:
                #look for username within the user file]
                temp_line = stripLastToken(line)
                if (username == temp_line):
                    #get the last token
                    line = line.strip().split(" ")
                    last_read = int(line.pop())
                    stream.last_read = last_read
    
    list_of_positions = []
    for post in list_of_posts:
        for stream in stream_list:
            data_filename = "messages/" + stream.name + "StreamData"
            with open(data_filename, "r") as dataFile:
                post_count = 0
                for line in dataFile:
                    post_count += 1
                if ((post.stream is stream.name) and (post.pos_within_stream == stream.last_read)):
                    if((post.pos_within_string != -1) and (post.pos_within_stream != post_count)):
                        list_of_positions.append(post.pos_within_string)
    
    try: most_unread_message = list_of_positions[0]
    except: most_unread_message = 0
    for pos in list_of_positions:
        print(pos)
        if (pos < most_unread_message):
            most_unread_message = pos

    most_unread_message -= 10 #we added 10 to the pos to make it easer to find ranges
    if (most_unread_message < 0): most_unread_message = 0 #make sure we didnt go out of bounds   
    return most_unread_message

def updateAllUserFiles(pos_min, pos_max, the_posts, username):        
    for post in the_posts:
        users_filename = "messages/" + post.stream + "StreamUsers"

        if ((pos_min < post.pos_within_string) and (pos_max > post.pos_within_string) and (post.pos_within_string != -1)):
            with open(users_filename, "r") as userFile:
                for line in userFile:
                    #look for username within the user file
                    temp_line = stripLastToken(line)
                    if (username == temp_line):
                        #get the last token
                        line = line.strip().split(" ")
                        last_post = int(line.pop())
            if (post.pos_within_stream > last_post): #this post is the most recently read post
                last_post = post.pos_within_stream
                #now update the file with the new new_post ValueError
                file_body = ""
                with open(users_filename, "r") as userFile:
                    for line in userFile:
                        temp_line = stripLastToken(line)
                        if (username == temp_line):
                            #modify the line and then add it to the body
                            new_line = username + " " + str(last_post) + "\n"
                            file_body += new_line
                        else:
                            #we dont need to modify the file so just add it to the body
                            file_body += line
                #finally write the new body to the file
                with open(users_filename, "w") as userFile:
                    userFile.write(file_body)

def Print24Lines(the_string, the_pos):
    #get the exact number of lines we want printed out of the_string
    number_of_char_printed = 0

    substring = the_string[the_pos:]
    substring_lines = substring.split("\n", 24)

    #print and count the number of chars printed
    for i in range(23):
        if i > (len(substring_lines) - 1):
            print('-') #print out the extra lines so we get a total of 24
        else:
            print('- ' + substring_lines[i])
            number_of_char_printed += len(substring_lines[i]) + 1

    #print out bottom prompt
    print("Up-Page Up  Down-Page Down O-order toggle  M-mark all  S-stream  C-check for new", end='', flush=True)
    
    return number_of_char_printed

def printBackwards24Lines(the_string, the_pos):
    #get the exact number of lines we want printed out of the_string
    number_of_char_printed = 0

    substring = the_string[0:the_pos]
    substring_lines = substring.split("\n")

    #print and count the number of chars printed
    for i in range(len(substring_lines) - 24, (len(substring_lines) - 1), 1):
        if i < 0:
            #reprint from the start 
            Print24Lines(the_string, 0)
            return -1 #so we know we restarted
        else:
            print('- ' + substring_lines[i])
            number_of_char_printed += len(substring_lines[i]) + 1

    #print out bottom prompt
    print("Up-Page Up  Down-Page Down O-order toggle  M-mark all  S-stream  C-check for new", end='', flush=True)
    
    return number_of_char_printed

def markAllAsRead(username, the_streams):
    #Get the number of streams from the stream object, then update the user file with that
    for the_stream in the_streams:
        num_posts = the_stream.getNumberOfStreams()
        
        users_filename = "messages/" + the_stream.name + "StreamUsers"

        #now update the userFile with the new last read post value
        file_body = ""
        with open(users_filename, "r") as userFile:
            for line in userFile:
                temp_line = stripLastToken(line)
                if (username == temp_line):
                    #modify the line and then add it to the body
                    new_line = username + " " + str(num_posts) + "\n"
                    file_body += new_line
                else:
                    #we dont need to modify the file so just add it to the body
                    file_body += line
        #finally write the new body to the file
        with open(users_filename, "w") as userFile:
            userFile.write(file_body)

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

def commandLoop(username, the_streams):
    input_text = ""
    sorted_by = "D"
    temp = getch()
    os.system('setterm -cursor off')

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
    list_of_posts = sortAllPostsByDate(list_of_posts)
    orig_list_of_posts = copy.deepcopy(list_of_posts)
    
    stream_string = ""
    for post in list_of_posts:
        #get the string representation of the Stream
        stream_string += str(post)

    current_printing_pos = getStartingPos(stream_list, list_of_posts, username)
    old_pos = current_printing_pos
    current_printing_pos += Print24Lines(stream_string, old_pos)
    if(new_stream.sorted_by is "D"): updateAllUserFiles(old_pos, current_printing_pos, list_of_posts, username)

    while(input_text is not "q"):
        input_text = temp()

        if(input_text == 'O'):
            if (sorted_by is "D"):
                #sort by dates then re-display
                list_of_posts = sortAllPostsByName(list_of_posts)
                sorted_by = "N"
                stream_string = ""
                for post in list_of_posts:
                    #get the string representation of the Stream
                    stream_string += str(post)
                current_printing_pos = Print24Lines(stream_string, 0)
            elif (sorted_by is "N"):
                #sort by names then re-display
                list_of_posts = copy.deepcopy(orig_list_of_posts)
                sorted_by = "D"
                stream_string = ""
                for post in list_of_posts:
                    #get the string representation of the Stream
                    stream_string += str(post)
                current_printing_pos = Print24Lines(stream_string, 0)
        elif(input_text == 'M'):
            markAllAsRead(username, stream_list)
        elif(input_text == 'C'):
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
            current_printing_pos += Print24Lines(stream_string, old_pos)
            if(new_stream.sorted_by is "D"): updateAllUserFiles(old_pos, current_printing_pos, list_of_posts, username)
        elif(input_text == 'S'):
            fake_args = []
            fake_args.append("blah")
            fake_args.append(username)
            #clear screen
            for i in range(23):
                print()
            print("Streams:")
            os.system('setterm -cursor on')
            main(fake_args)
        elif(input_text is "Up"):
            return_val = printBackwards24Lines(stream_string, current_printing_pos)
            if (return_val != -1): current_printing_pos -= return_val
        elif(input_text is "Down"):
            #just print at the new_pos, 24 lines after the first printLines call
            old_pos = current_printing_pos
            current_printing_pos += Print24Lines(stream_string, old_pos)
            if(new_stream.sorted_by is "D"): updateAllUserFiles(old_pos, current_printing_pos, list_of_posts, username)

    os.system('setterm -cursor on')

def main(argv):
    username = ""
    available_streams = []
    if len(argv) == 1:
        print("Incorrect amount of args provided.")
        sys.exit(1)

    username = " ".join(argv[1:])
    available_streams = printOutUserStreams(username)

    if(len(available_streams) == 0):
        sys.exit(1)

    desired_stream = input()

    temp_array = []
    temp_array.append(desired_stream)

    if (desired_stream == "all"):
        commandLoop(username, available_streams)
    elif (desired_stream not in available_streams):
        print("You arent subscribbed to that stream.")
        sys.exit(1)
    else:
        commandLoop(username, temp_array)

if __name__ == "__main__":
    main(sys.argv)
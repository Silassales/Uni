*****************************************************
Silas Sales
0922973
Cis*3110 Assignment 3
*****************************************************
Usage:
    To complie, type make in root directory
    - note: when compiling, there are warning pertaining
    to the Usage of pthread_create, but these warning do 
    not affect the performance of the program and I could 
    not figure out how to silence them.

    To run phil:
    ./phil "num_of_phil" "num_of_times_to_eat"

    To run holes:
    ./holes "text_file_to_use"
*****************************************************
Algorithm:
    Phil:
        -First I get the value of the command line arguments.
        -I then set up and initalize my semphores which represent my 
        utensils/chopsticks/forks, I also set up a lock for each
        philospher so that when the philospher is eating, it will
        always finish eating.
        -I do both of these using the sem_init function
        -I then create my thread which represent my philosphers
        -I do this with the pthread_create function.
        -I also increment the semphores so that we actually have progress
        -I then make thread wait for termination using the pthread_join
        function
        -Finally I destory all my semaphores using sem_destory
        -Within my function used during pthread_create I first wait for the 
        lock semaphore, this is the start of my critical section, I then wait for 
        the forks on my left and right.
        -After I have both forks on my left and right, I eat(and sleep for 10milsec)
        -I then realese the forks on my left and right back to the world, unlock my lock,
        go back to thiking and exit the thread

    Holes:
    ** Data structures used: one linked list for the waiting queue and then
    one array of struct pointers to represent my memory map **
        -I just used the Algorithms provided in lecture notes which are:

        First Fit: Allocate the first spot in memory that is big enough to satisfy the 
        requirments
        Best Fit: Search through all of memory and look for the holes that best fits the 
        requirments
        Next Fit: Allocate the first spot in memory from the last spot memory was allocated
        Worst Fit: Use the largets peice of memory to allocate for new process

        If any of these cases were not met, I just started removing the oldest processes in memory
        until they were met.
*****************************************************
Assumptions:

cant have process named '0' in the text file 
the text file has to be exactly to spec
On the server longbottom once the number of threads create is >= 50, I get a pthread_create return an error
value, since this does not happen on any other computer I test, I am assuming this is server related
No error checking is provided for argument value being correct (the number is checked but not the type)


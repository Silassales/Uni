**********************************************************
Silas Sales
0922973
Cis*3110 Assignment 2
**********************************************************
Compilation:
After unpacking the tar file, cd into the parent directory
type: gcc simcpu.c structures.c -std=c99 -o simcpu

Usage:
run with: ./simcpu < "textfile.txt"
include the -d,-v,-r flags between the '<' and the executable
**********************************************************
Algorithm:
-When first run the data will be stored within a linked list, by process.
-The event queue is then initialized and sorted by arrival time.
-The simulation is then started
-The simulation is just a large while loop that starts by popping an event off the front of the event queue
-The cpu downtime and global time are then appropriately updated
-There are 5 types of events: arrival, interupt, io_complete, termination and time_slice
-The event popped off the event queue will be acted upon based off the type of event it is
If it is arrival:
    I first add the event->thread to the ready queue
    I then make sure that the cpu is busy, and if not I pop a thread off the ready queue and assign it to the cpu
If its an interupt:
    I put the next thread in the ready queue on the cpu (and in effect take the current thread off the cpu)
    I then add a io_complete event to the ready queue at the end of the next io_burst time
If its an io_complete: 
    First I make sure that the cpu is busy
    Then increment the current burst number value of the thread, so we know what burst we will be on
    Then I create either a new interrupt or new termination for the next cpu burst, depending on if this is the last cpu burst
If its a termination:
    First make sure that the cpu is working
    Then set the threads type to terminated
    Then I print out the thread termination stats if detailed mode is enabled
If its a time slice:
    Check if cpu_burst > quantum, if so create another time slice
    Then check if its the last cpu_burst-> create a termination
    Otherwise create an interupt

After checking the type of event, I calculated and print out the stats for the simulation

For each checking if the cpu is busy:
    I check if the cpu is busy
    Then pop a thread off the ready queue and make sure its not null
    I then check if the cpu is in round robin mode and if so schedule a time slice
    Otherwise I check for if this is the last cpu-> schedule a termination
    or I add new interupt for the end of the new cpu_burst of the thread

At the end of each event I also print out infomation based on the event if in verbose mode
**********************************************************
Questions:
i. Yes, because the cpu might have a previous state to save and later restore to that it needs to save before running my threads.

ii. No, because the cpu is idle, and therefore has no process/thread block to save before running the next thread.

iii. Yes, because just because the ready queue is empty when the thread is blocked doesnt mean that it will be empty later, and may need to restore.

iv. Yes, for similar reasons to the answer before, the ready queue may changed during the interuption.









#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structers.h"

void simulation(event *, int, int, int, int, int, int, int);
event * initEventQueue(process *);
process * storeData();

int main (int argc, char ** argv) {
    int detailed_info_mode = 0;
    int verbose_mode = 0;
    int round_robin_mode = 0;
    int round_robin_time = 0;

    for (int i = 0; i < argc; i++){
        if(strcmp(argv[i], "-d") == 0) {
            detailed_info_mode = 1;
        } else if(strcmp(argv[i], "-v") == 0) {
            verbose_mode = 1;
        } else if(strcmp(argv[i], "-r") == 0) {
            round_robin_mode = 1;
            i++;
            round_robin_time = strtol(argv[i], argv, 10);
        }
    }

    int num_process = 0;
    int thread_switch = 0;
    int process_switch = 0;

    process * head = storeData(&num_process, &thread_switch, &process_switch);

    event * event_queue_head = initEventQueue(head);

    int global_num_threads = globalNumThreads(head);
    simulation(event_queue_head, process_switch, thread_switch, global_num_threads, round_robin_mode, round_robin_time, detailed_info_mode, verbose_mode);
    
    freeProcessList(head);
    return 0;
}

void simulation(event * event_queue, int process_switch, int thread_switch, int num_threads, int round_robin, int round_robin_time, int detailed_info_mode, int verbose_mode) {
    unsigned int global_clock = 0;
    int turnaround_time = 0;
    int cpu_downtime = 0;
    thread * ready_queue = NULL;
    event * temp_event;
    thread * cpu_temp_thread;
    int cpu_busy = 0;

    while(event_queue != NULL) {
        temp_event = popEventFromFront(&event_queue);

        if (!cpu_busy) {
            cpu_downtime += (temp_event->time - global_clock);
        }
        
        global_clock = temp_event->time;

        if (temp_event->type == ARRIVAL) {
            //add event to the ready queue to be processed
            addToReadyQueue(&ready_queue, temp_event->the_thread);
                        
            if (cpu_busy == 0) {
                //if the cpu is not busy, get it working
                //we dont have to check if pop returns null as we know we just added
                //a thread to the ready queue
                cpu_temp_thread = popThreadFromFrontReadyQueue(&ready_queue);
                cpu_temp_thread->type = Running;
                /*either generate a interrupt or a termination if the next IO is -1*/
                // get info for interrupt event for after cpu burst
                int time = global_clock + cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst];
                if (round_robin && cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] > round_robin_time) {
                    cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] -= round_robin_time;
                    time = global_clock + round_robin_time;
                    event * new_time_slice = createEvent(5, time, cpu_temp_thread);
                    addToEventQueue(&event_queue, new_time_slice);
                } else {
                    if(cpu_temp_thread->IO_burst[cpu_temp_thread->current_burst] == -1) {
                        event * new_termination = createEvent(4, time, cpu_temp_thread);
                        addToEventQueue(&event_queue, new_termination);
                    } else {
                        event * new_interupt = createEvent(2, time, cpu_temp_thread);

                        //add the interrupt to event queue
                        addToEventQueue(&event_queue, new_interupt);
                    }
                }
                
                cpu_busy = 1;
            }

            if(verbose_mode) {
                printf("At time %d: Thread #%d moved from new to ready.\n\n", global_clock, temp_event->the_thread->ID);
            }
        } else if(temp_event->type == INTERUPT) {
            /* during an interrupt we want to:
            take the proccess off the cpu, put the next process on the cpu
            then we want to add an IO_COMPLETE event
            */
            cpu_temp_thread = popThreadFromFrontReadyQueue(&ready_queue);
            if(cpu_temp_thread != NULL) {
                int result = compareID(temp_event->the_thread->ID, cpu_temp_thread->ID);
                if(result == 1) {
                    global_clock += thread_switch;
                } else if (result == -1) {
                    global_clock += process_switch;
                }
                cpu_temp_thread->type = Running;
                // get info for interrupt event for after cpu burst
                int time = global_clock + cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst];
                if (round_robin && cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] > round_robin_time) {
                    cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] -= round_robin_time;
                    time = global_clock + round_robin_time;
                    event * new_time_slice = createEvent(5, time, cpu_temp_thread);
                    addToEventQueue(&event_queue, new_time_slice);
                } else {
                    if(cpu_temp_thread->IO_burst[cpu_temp_thread->current_burst] == -1) {
                        event * new_termination = createEvent(4, time, cpu_temp_thread);
                        addToEventQueue(&event_queue, new_termination);
                    } else {
                        event * new_interupt = createEvent(2, time, cpu_temp_thread);

                        //add the interrupt to event queue
                        addToEventQueue(&event_queue, new_interupt);
                    }
                }
                cpu_busy = 1;
            } else {
                // cpu is idle
                cpu_busy = 0;
            }

            //IO_complete event
            int time = global_clock + temp_event->the_thread->IO_burst[temp_event->the_thread->current_burst];
            event * new_IO_complete = createEvent(3, time, temp_event->the_thread);

            addToEventQueue(&event_queue, new_IO_complete);
            if(verbose_mode) {
                printf("At time %d: Thread #%d moved from running to blocked.\n\n", global_clock, temp_event->the_thread->ID);
            }
        } else if(temp_event->type == IO_COMPLETE) {
            //during a IO_complete we want to:
            //make sure cpu is working
            //increment the current burst value of the thread
            //create either a new interrupt or a new termination for the next cpu burst
            if(cpu_busy == 0) {
                cpu_temp_thread = popThreadFromFrontReadyQueue(&ready_queue);
                if(cpu_temp_thread != NULL) {
                    cpu_temp_thread->type = Running;
                    // get info for interrupt event for after cpu burst
                    int time = global_clock + cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst];
                    if (round_robin && cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] > round_robin_time) {
                        cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] -= round_robin_time;
                        time = global_clock + round_robin_time;
                        event * new_time_slice = createEvent(5, time, cpu_temp_thread);
                        addToEventQueue(&event_queue, new_time_slice);
                    } else {
                        if(cpu_temp_thread->IO_burst[cpu_temp_thread->current_burst] == -1) {
                            event * new_termination = createEvent(4, time, cpu_temp_thread);
                            addToEventQueue(&event_queue, new_termination);
                        } else {
                            event * new_interupt = createEvent(2, time, cpu_temp_thread);

                            //add the interrupt to event queue
                            addToEventQueue(&event_queue, new_interupt);
                        }
                    }
                    cpu_busy = 1;
                } else {
                    // cpu is idle
                    cpu_busy = 0;
                }
            }
            
            temp_event->the_thread->current_burst += 1;

            /*either generate a interrupt or a termination if the next IO is -1*/
            // get info for interrupt event for after cpu burst
            int time = global_clock + temp_event->the_thread->CPU_burst[temp_event->the_thread->current_burst];
            if (round_robin && temp_event->the_thread->CPU_burst[temp_event->the_thread->current_burst] > round_robin_time) {
                temp_event->the_thread->CPU_burst[temp_event->the_thread->current_burst] -= round_robin_time;
                time = global_clock + round_robin_time;
                event * new_time_slice = createEvent(5, time, temp_event->the_thread);
                addToEventQueue(&event_queue, new_time_slice);
            } else {
                if(temp_event->the_thread->IO_burst[temp_event->the_thread->current_burst] == -1) {
                    event * new_termination = createEvent(4, time, temp_event->the_thread);
                    addToEventQueue(&event_queue, new_termination);
                } else {
                    event * new_interupt = createEvent(2, time, temp_event->the_thread);

                    //add the interrupt to event queue
                    addToEventQueue(&event_queue, new_interupt);
                }
            }
            
            if(verbose_mode) {
                printf("At time %d: Thread #%d moved from blocked to running.\n\n", global_clock, temp_event->the_thread->ID);
            }
        } else if(temp_event->type == TERMINATION) {
            //for termination we want to:
            //make sure cpu is working
            //set the threads type to terminated
            if(cpu_busy == 0) {
                cpu_temp_thread = popThreadFromFrontReadyQueue(&ready_queue);
                if(cpu_temp_thread != NULL) {
                    cpu_temp_thread->type = Running;
                    // get info for interrupt event for after cpu burst
                    int time = global_clock + cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst];
                    if (round_robin && cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] > round_robin_time) {
                        cpu_temp_thread->CPU_burst[cpu_temp_thread->current_burst] -= round_robin_time;
                        time = global_clock + round_robin_time;
                        event * new_time_slice = createEvent(5, time, cpu_temp_thread);
                        addToEventQueue(&event_queue, new_time_slice);
                    } else {
                        if(cpu_temp_thread->IO_burst[cpu_temp_thread->current_burst] == -1) {
                            event * new_termination = createEvent(4, time, cpu_temp_thread);
                            addToEventQueue(&event_queue, new_termination);
                        } else {
                            event * new_interupt = createEvent(2, time, cpu_temp_thread);

                            //add the interrupt to event queue
                            addToEventQueue(&event_queue, new_interupt);
                        }
                    }
                    cpu_busy = 1;
                } else {
                    // cpu is idle
                    cpu_busy = 0;
                }
            }
            if(verbose_mode) {
                printf("At time %d: Thread #%d moved from running to terminated.\n\n", global_clock, temp_event->the_thread->ID);
            }

            temp_event->the_thread->type = Terminated;
            turnaround_time += temp_event->time - temp_event->the_thread->arrival_time;
            if (detailed_info_mode) {
                //print out info for each thread
                printf("\nThread #%d\n\n", temp_event->the_thread->ID);
                printf("Arrival Time: %d\n", temp_event->the_thread->arrival_time);
                printf("Service Time: %d\n", getServiceTime(temp_event->the_thread));
                printf("I/O Time: %d\n", getIOTime(temp_event->the_thread));
                printf("Turnaround Time: %d\n", temp_event->time - temp_event->the_thread->arrival_time);
                printf("Finish Time: %d\n\n", temp_event->time);
            }

        } else if(temp_event->type == TIME_SLICE) {
            //For a time slice we want to:
            //if cpu_burst > time_slice create another time slice
            //else if this is not the last cpu burst create and interrupt
            //else create a termination
            int time = global_clock + round_robin_time;
            if (temp_event->the_thread->CPU_burst[temp_event->the_thread->current_burst] > round_robin_time) {
                temp_event->the_thread->CPU_burst[temp_event->the_thread->current_burst] -= round_robin_time;
                event * new_time_slice = createEvent(5, time, temp_event->the_thread);
                addToEventQueue(&event_queue, new_time_slice);
            } else if (temp_event->the_thread->IO_burst[temp_event->the_thread->current_burst] != -1) {
                event * new_interrupt = createEvent(2, time, temp_event->the_thread);
                addToEventQueue(&event_queue, new_interrupt);
            } else {
                event * new_termination = createEvent(4, time, temp_event->the_thread);
                addToEventQueue(&event_queue, new_termination);
            }
            if(verbose_mode) {
                printf("At time %d: Thread #%d is running within a time slice.\n\n", global_clock, temp_event->the_thread->ID);
            }
        }

        freeEvent(temp_event);
    }

    //stats 

    double average_turnaround_time = (double)turnaround_time/num_threads;
    int total_time = global_clock;
    int total_cpu_time = total_time - cpu_downtime;
    double cpu_util = 0;
    if (total_time != 0) cpu_util = ((double)total_cpu_time/total_time) * 100;
    else cpu_util = 0;

    if(round_robin) printf("Round Robin (quantum = %d time units).\n\n", round_robin_time);
    else printf("\nFCFS:\n\n");
    printf("Total Time required is %d time units.\n", total_time);
    printf("Average Turnaround time is %.2lf time units.\n", average_turnaround_time);
    printf("CPU Utilization is %.2lf%%.\n", cpu_util);

}

event * initEventQueue(process * head_of_process_list) {
    event * event_queue_temp = NULL;
    int first_time = 0;

    process * temp_process = head_of_process_list;

    while(temp_process != NULL) {
        thread * temp_thread = temp_process->thread_array;
        while(temp_thread != NULL) {
            event * new_event = createEvent(1, temp_thread->arrival_time, temp_thread);
            if (first_time == 0) {
                event_queue_temp = new_event;
                first_time = 1;
            } else {
                addToEventQueue(&event_queue_temp, new_event);  
            }
            temp_thread = temp_thread->next;
        }
        temp_process = temp_process->next;
    }

    return event_queue_temp;
}

process * storeData(int * num_process, int * thread_switch, int * process_switch) {
    process * head = NULL;
    char line[256];

    fgets(line, 256, stdin);
    
    //get num_procces, process_switch and thread_switch
    char * token;
    *num_process = strtol(token = strtok(line, " "), &token, 10);
    *thread_switch = strtol(token = strtok(NULL, " "), &token, 10);
    *process_switch = strtol(token = strtok(NULL, " "), &token, 10);

    //start storing proccess and threads
    for(int i = 0; i < *num_process; i++) {
        // make and store a new process
        process * temp_process;
        fgets(line, 256, stdin);
        int ID = strtol(token = strtok(line, " "), &token, 10);
        int num_threads = strtol(token = strtok(NULL, " "), &token, 10);
        temp_process = createProcess(ID, num_threads);
        addProcessToList(&head, temp_process);

        // make and store new threads
        for(int j = 0; j < num_threads; j++) {
            thread * temp_thread;
            fgets(line, 256, stdin);
            //make the id the combinatino of the process ID and thread ID to make it unique
            token = strtok(line, " ");
            int count = snprintf(NULL, 0, "%i", temp_process->ID);
            char * temp_ID = (char*)malloc(count + strlen(token) + 1);
            char * free_temp_ID = temp_ID;
            sprintf(temp_ID, "%d9%s", temp_process->ID, token);
            int ID = strtol(temp_ID, &temp_ID, 10);
            free(free_temp_ID);
            int arrival_time = strtol(token = strtok(NULL, " "), &token, 10);
            int num_bursts = strtol(token = strtok(NULL, " "), &token, 10);
            temp_thread = createThread(ID, arrival_time, num_bursts);
            addThreadToProcess(&temp_process, temp_thread);

            //store CPU,IO bursts
            for(int k = 0; k < num_bursts;k++) {
                fgets(line, 256, stdin);
                strtok(line, " ");
                int CPU_time = strtol(token = strtok(NULL, " "), &token, 10);
                int IO_time;
                if (k == (num_bursts - 1)) {
                    // the last one only has CPU time
                    IO_time = -1;
                } else {
                    IO_time = strtol(token = strtok(NULL, " "), &token, 10);
                }
                addBurstTimesToThread(&temp_thread, k, CPU_time, IO_time);
            }
        }
    }

    return head;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structers.h"


//for testing
// int main() {
    
// }

void printProcessList(process * head) {
    process * temp_process = head;
    while(temp_process != NULL) {
        printf("Process #%d\n", temp_process->ID);
        thread * temp_thread = temp_process->thread_array;
        for(int i = 0; i < temp_process->num_threads; i++){
            printf("  Thread #%d arrived @ %d num_bursts %d\n", temp_thread->ID, temp_thread->arrival_time, temp_thread->num_bursts);
            for(int j = 0; j < temp_thread->num_bursts; j++){
                 printf("    CPU Burst #%d is %d\n", j, temp_thread->CPU_burst[j]);
                 printf("    IO Burst #%d is %d\n", j, temp_thread->IO_burst[j]);
            }
            temp_thread = temp_thread->next;
        }
        temp_process = temp_process->next;
    }
}

process * createProcess(int ID, int num_threads) {
    process * temp_process = (process*)malloc(sizeof(process));

    temp_process->ID = ID;
    temp_process->num_threads = num_threads;
    temp_process->thread_array = NULL;
    temp_process->next = NULL;

    return temp_process;
}

int globalNumThreads(process * head) {
    if (head == NULL) {
        return 0;
    }

    process * temp = head;
    int count = 0;

    while(temp != NULL) {
        count += temp->num_threads;
        temp = temp->next;
    }
    return count;
}

void freeProcessList(process * head) {
    if(head == NULL) {
        return;
    }
    process * temp;
    while(head != NULL){
        temp = head->next;
        freeThreadList(head->thread_array);
        free(head);
        head = temp;
    }
}

void addProcessToList(process ** head, process * the_process) {
    if (*head == NULL) {
        *head = the_process;
        return;
    }
    if ((*head)->next == NULL) {
        (*head)->next = the_process;
        return;
    }
    process * temp = *head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    //once at the end add the new process
    temp->next = the_process;
}

thread * createThread(int ID, int arrival_time, int num_bursts) {
    thread * temp = (thread *)malloc(sizeof(thread));

    temp->ID = ID;
    temp->arrival_time = arrival_time;
    temp->num_bursts = num_bursts;
    temp->CPU_burst = (int*)malloc(sizeof(int) * num_bursts);
    temp->IO_burst = (int*)malloc(sizeof(int) * num_bursts);
    temp->current_burst = 0;
    temp->type = New;
    temp->next = NULL;
    temp->ready_queue_next = NULL;

    return temp;
}

void freeThreadList(thread * head) {
    thread * temp;
    while(head != NULL) {
        temp = head->next;
        free(head->CPU_burst);
        free(head->IO_burst);
        free(head);
        head = temp;
    }
}

void addThreadToProcess(process ** the_process, thread* the_thread) {
    thread * temp = (*the_process)->thread_array;

    if (temp == NULL) {
        (*the_process)->thread_array = the_thread;
        return;
    }
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = the_thread;
}

void addBurstTimesToThread(thread ** the_thread, int pos, int CPU_time, int IO_time) {
    (*the_thread)->CPU_burst[pos] = CPU_time;
    (*the_thread)->IO_burst[pos] = IO_time;
}

void addToReadyQueue(thread ** head, thread * new_thread) {
    thread * temp = *head;
    new_thread->type = Ready;

    if (temp == NULL) {
        *head = new_thread;
        return;
    }
    while(temp->ready_queue_next != NULL) {
        temp = temp->ready_queue_next;
    }
    temp->ready_queue_next = new_thread;
}

thread * popThreadFromFrontReadyQueue(thread ** ready_queue) {
    if(*ready_queue == NULL) {
        return NULL;
    }

    thread * return_thread = *ready_queue;

    *ready_queue = (*ready_queue)->ready_queue_next;

    return return_thread;
}

event * createEvent(int type, int time, thread * the_thread) {
    event * new_event = malloc(sizeof(event));

    switch(type) {
        case 1:
            new_event->type = ARRIVAL;
            break;
        case 2:
            new_event->type = INTERUPT;
            break;
        case 3:
            new_event->type = IO_COMPLETE;
            break;
        case 4:
            new_event->type = TERMINATION;
            break;
        case 5:
            new_event->type = TIME_SLICE;
            break;
        default:
            new_event->type = ARRIVAL;
            break;
    }
    new_event->time = time;
    new_event->the_thread = the_thread;
    new_event->next = NULL;
    new_event->event_queue_next = NULL;

    return new_event;
}



void addToEventQueue(event ** head, event * new_event) {
    event * temp_event = *head;

    if(temp_event == NULL) {
        *head = new_event;
        return;
    }
    
    while(temp_event->event_queue_next != NULL) {
        if(temp_event->event_queue_next->time >= new_event->time) {
            new_event->event_queue_next = temp_event->event_queue_next;
            temp_event->event_queue_next = new_event;
            return;
        }
        temp_event = temp_event->event_queue_next;
    }
    temp_event->event_queue_next = new_event;
}

void printEventQueue(event * queue_head) {
    event * temp_event = queue_head;

    while(temp_event != NULL) {
        printf("Thread #%d @ %d %d %d\n", temp_event->the_thread->ID, temp_event->time, temp_event->type, temp_event->the_thread->current_burst);
        temp_event = temp_event->event_queue_next;
    }
}

void freeEvent(event * the_event) {
    free(the_event);
}

event * popEventFromFront(event ** event_queue) {
    if(*event_queue == NULL) {
        return NULL;
    }

    event * return_event = *event_queue;

    *event_queue = (*event_queue)->event_queue_next;

    return return_event;
}

int getServiceTime(thread * the_thread) {
    int total = 0;
    for(int i = 0; i < the_thread->num_bursts;i++) {
        total += the_thread->CPU_burst[i];
    }
    return total;
}

int getIOTime(thread * the_thread) {
    int total = 0;
    for(int i = 0; i < the_thread->num_bursts;i++) {
        total += the_thread->IO_burst[i];
    }
    //the last io will have a value of -1
    return total + 1;
}

int compareID(int x, int y) {
    //will return 0 is they are equal, same thread
    //will return 1 if they are different but same process
    //will return -1 if they are different and from different processes
    if(x == y) {
        return 0;
    }

    char * first_string = (char*)malloc(10);
    sprintf(first_string, "%d", x);
    char * second_string = (char*)malloc(10);
    sprintf(second_string, "%d", y);

    char token[3];
    strncpy(token, first_string, 2);
    token[2] = '\0';
    char token2[3];
    strncpy(token2, second_string, 2);
    token2[2] = '\0';

    if(strcmp(token, token2) == 0) {
        free(first_string);
        free(second_string);
        return 1;
    }

    free(first_string);
    free(second_string);
    return -1;
}
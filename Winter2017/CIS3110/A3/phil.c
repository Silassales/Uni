#define _BSD_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

void phil_eat(void *);

sem_t * utensil;
pthread_t * philosophers;
sem_t * lock;
int phil_count;
int eat_count;

int main(int argc, char * argv[]) {
    if (argc != 3) {
        puts("Invalid number of args provided.");
        exit(1);
    }

    phil_count = (int) strtol(argv[1], NULL, 10);
    eat_count = (int) strtol(argv[2], NULL, 10);

    sem_t utensil_temp[phil_count];
    sem_t lock_temp[phil_count];
    pthread_t philosophers_temp[phil_count];
    lock = lock_temp;
    utensil = utensil_temp;
    philosophers = philosophers_temp;

    // init my utensils/semphores
    for(int i = 0; i < phil_count; i++) {
        if(sem_init(&utensil[i],0,1)) {
            puts("Semphore init error");
            exit(1);
        }
        if(sem_init(&lock[i], 0, phil_count - 1)){
            puts("Lock init error");
            exit(1);
        }
    }

    // create my philosphers/threads
    for(int i = 0; i < phil_count; i++) {
        if(pthread_create(&philosophers[i],NULL, phil_eat, (void *)i)) {
            puts("pthread create error");
            exit(1);
        }

        //increment semphore
        sem_post(&utensil[i]);
    }

    // make threads wait for termination
    for(int i = 0; i < phil_count; i++) {
        if(pthread_join(philosophers[i], NULL)) {
            puts("pthread join error");
            exit(1);
        }
    }

    for(int i = 0; i < phil_count; i++) {
        if(sem_destroy(&utensil[i])) {
            puts("Semphore destruction error");
            exit(1);
        }
    }
    

    return 0;
}

void phil_eat(void * ID) {
    int pid = (int) ID;
    

    // wait till free then eat
    for(int i = 0; i < eat_count; i++) {
        printf("Philosopher %d thinking.\n", pid + 1);
        sem_wait(&lock[pid]);
        sem_wait(&utensil[pid]);
        sem_wait(&utensil[(pid + 1)% phil_count]);
        printf("Philosopher %d eating.\n", pid + 1);
        usleep(10);
        sem_post(&utensil[pid]);
        sem_post(&utensil[(pid + 1) % phil_count]);
        sem_post(&lock[pid]);
    }

    //close current thread
    printf("Philosopher %d thinking.\n", pid + 1);
    pthread_exit(NULL);
}

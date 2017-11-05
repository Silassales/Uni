struct thread {
    int ID;
    int num_bursts;
    int * CPU_burst; //will set this to a array of size n when n is known
    int * IO_burst; //will set this to a array of size n when n is known
    enum status_type {
        New,
        Ready,
        Running,
        Blocked,
        Terminated
    } type;
    int current_burst;
    int arrival_time;
    struct thread * next;
    struct thread * ready_queue_next;
}; 
typedef struct thread thread;

struct process {
    int ID;
    int num_threads;
    thread * thread_array;
    struct process * next;
}; 
typedef struct process process;

struct event {
    enum event_type {
        ARRIVAL,
        INTERUPT,
        IO_COMPLETE,
        TERMINATION,
        TIME_SLICE
    } type;
    int time;
    thread * the_thread;
    struct event * next;
    struct event * event_queue_next;
};
typedef struct event event;

void printProcessList(process *);

//Process helper functions
process * createProcess(int, int);
void freeProcessList(process *);
int globalNumThreads(process *);
void addProcessToList(process **, process *);

//Thread helper functions 
int getServiceTime(thread * the_thread);
int getIOTime(thread * the_thread);
thread * createThread(int, int, int);
void freeThreadList(thread *);

void addThreadToProcess(process **, thread*);
void addBurstTimesToThread(thread **, int, int, int);

void addToReadyQueue(thread **, thread *);
thread * popThreadFromFrontReadyQueue(thread **);

//Event helper functions
event * createEvent(int, int, thread*);
void addToEventQueue(event **, event *);
void printEventQueue(event *);
void freeEvent(event *);
event * popEventFromFront(event **);

int compareID(int, int);

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MEM_SIZE 128
// global value time to track time added and determine what process'
int time = 0;
//global value total_holes and total_loads to track overall cpu usage
int total_process = 0;
int total_loads = 0;

struct process {
	char * ID;
	int mem_size;
	int num_times_run;
	int time_added;
	struct process * next;
}; typedef struct process process;

process * addToWaitingQueue(process *, char * ID, int mem_size, int num_times_run, int time_added);
process * createProcess(char * ID, int mem_size, int num_times_run, int time_added);
process * popFrontWaitingQueue(process **);
void freeWaitingQueue(process *);
void freeProcess(process *);
void printMap(process **);
void printWaitingQueue(process *);
void firstFit(process **, process *);
void bestFit(process **, process *);
void nextFit(process **, process *);
void worstFit(process **, process *);
void removeProcessFromMap(process **, process **, int, int);
int getIndexOfNextHole(process **, int);
int getIndexOfOldestProcess(process **);
int getNumProcess(process **);
int getNumHoles(process **);
double getMemUsage(process **);
double getCumMemUsage(process **);
int getIndexOfBestHole(process ** map, int size_of_hole);
int getIndexOfBiggestHole(process ** map, int size_of_hole);

int main(int argc, char * argv[]) {
	if(argc != 2) {
		puts("Not enough arguments provided");
		exit(1);
	}

	char * output_filename = argv[1];

	if(access(output_filename, F_OK) == -1) {
		puts("Filename does not exist");
		exit(1);
	}

	FILE * fp = fopen(output_filename, "r");
	char line[256];
	process * waiting_queue_head = NULL;

	while(fgets(line, 256, fp) != NULL) {
		char * ID = strtok(line, " ");
		char * mem_size_char = strtok(NULL, "");

		int mem_size = (int) strtol(mem_size_char, NULL, 10);

		if(mem_size > MEM_SIZE) {
			puts("Memory size in file greater than total memory size (128 MB)");
			exit(1);
		}
		waiting_queue_head = addToWaitingQueue(waiting_queue_head, ID, mem_size, 0, 0);
	}

	process * map[MEM_SIZE];
	process * blank_process = createProcess("0", 0, 0, 0);
	int i;
	/* using a array of struct process of size 128 to simulate memory 
		each index represents a MB of memory */ 
	for (i = 0;i < MEM_SIZE;i++) {
		map[i] = blank_process;
	}

	firstFit(map, waiting_queue_head);

	freeWaitingQueue(waiting_queue_head);
	waiting_queue_head = NULL;
	fclose(fp);
	fp = fopen(output_filename, "r");

	while(fgets(line, 256, fp) != NULL) {
		char * ID = strtok(line, " ");
		char * mem_size_char = strtok(NULL, "");

		int mem_size = (int) strtol(mem_size_char, NULL, 10);

		if(mem_size > MEM_SIZE) {
			puts("Memory size in file greater than total memory size (128 MB)");
			exit(1);
		}
		waiting_queue_head = addToWaitingQueue(waiting_queue_head, ID, mem_size, 0, 0);
	}

	for (i = 0;i < MEM_SIZE;i++) {
		map[i] = blank_process;
	}

	bestFit(map, waiting_queue_head);


	freeWaitingQueue(waiting_queue_head);
	waiting_queue_head = NULL;
	fclose(fp);
	fp = fopen(output_filename, "r");

	while(fgets(line, 256, fp) != NULL) {
		char * ID = strtok(line, " ");
		char * mem_size_char = strtok(NULL, "");

		int mem_size = (int) strtol(mem_size_char, NULL, 10);

		if(mem_size > MEM_SIZE) {
			puts("Memory size in file greater than total memory size (128 MB)");
			exit(1);
		}
		waiting_queue_head = addToWaitingQueue(waiting_queue_head, ID, mem_size, 0, 0);
	}

	for (i = 0;i < MEM_SIZE;i++) {
		map[i] = blank_process;
	}

	nextFit(map, waiting_queue_head);	

	freeWaitingQueue(waiting_queue_head);
	waiting_queue_head = NULL;
	fclose(fp);
	fp = fopen(output_filename, "r");

	while(fgets(line, 256, fp) != NULL) {
		char * ID = strtok(line, " ");
		char * mem_size_char = strtok(NULL, "");

		int mem_size = (int) strtol(mem_size_char, NULL, 10);

		if(mem_size > MEM_SIZE) {
			puts("Memory size in file greater than total memory size (128 MB)");
			exit(1);
		}
		waiting_queue_head = addToWaitingQueue(waiting_queue_head, ID, mem_size, 0, 0);
	}

	for (i = 0;i < MEM_SIZE;i++) {
		map[i] = blank_process;
	}

	worstFit(map, waiting_queue_head);

	freeWaitingQueue(waiting_queue_head);
	freeProcess(blank_process);

	fclose(fp);
}

void removeProcessFromMap(process ** map, process ** waiting_queue, int start, int size) {
	process * temp = map[start];
	process * blank_process = createProcess("0", 0, 0, 0);
	if(temp->num_times_run < 2) {
		*waiting_queue = addToWaitingQueue(*waiting_queue, temp->ID, temp->mem_size, temp->num_times_run + 1, temp->time_added);
	}
	freeProcess(temp);
	int i;
	for(i = start; i < start + size;i++) {
		map[i] = blank_process;
	}
}

int getIndexOfOldestProcess(process ** map) {
	int return_index = -1;
	//first find the first process time
	int i;
	int current_oldest_time = -1;
	for(i = 0; i < MEM_SIZE;i ++) {
		if((strcmp(map[i]->ID, "0") != 0) && current_oldest_time == -1) {
			current_oldest_time = map[i]->time_added;
			return_index = i;
		}
	}
	//if there are no process in the map
	if (current_oldest_time == -1) {
		return return_index;
	}

	//loop through and compare everytime with the current one and find the index of the most oldest one
	for(i = 0; i < MEM_SIZE;i++) {
		if(((strcmp(map[i]->ID, "0") != 0)) && map[i]->time_added < current_oldest_time) {
			current_oldest_time = map[i]->time_added;
			return_index = i;	
		}
	}

	return return_index;
}

void firstFit(process ** map, process * waiting_queue_head) {
	printf("Running First Fit...\n");
	process * pop_process;
	int num_loads = 0;
	int num_holes = 0;
	int num_process = 0;
	while(waiting_queue_head != NULL) {
		/* Algo :
			* Pop the first process from the waiting queue 
			* try and put the process in the first large enough hole
			* if there is no hole that fits:
			* find the process that has been in memory the longest, remove it, place it in waiting queue (if num_times_run !> 3)
			* continue doing this until the waiting queue is empty
			*/
		pop_process = popFrontWaitingQueue(&waiting_queue_head);
		int cont = 0;
		while(!cont) {
			int insert_pos = getIndexOfNextHole(map, pop_process->mem_size);
			if(insert_pos != -1) {
				cont = 1;
				int i;
				pop_process->time_added = time;
				time++;
				for(i = insert_pos; i < insert_pos + pop_process->mem_size; i++) {
					map[i] = pop_process;
				}
				total_loads += 1;
				num_loads += 1;
				num_holes += getNumHoles(map);
				num_process += getNumProcess(map);
				double mem_usage = getMemUsage(map);
				double cum_mem_usage = getCumMemUsage(map);
				printf("%s, #processes = %d, #holes = %d, %%memusage = %.2lf, cummulative %%mem = %.2lf\n", pop_process->ID, getNumProcess(map), getNumHoles(map), mem_usage, cum_mem_usage);
			} else {
				int remove_pos = getIndexOfOldestProcess(map);
				removeProcessFromMap(map, &waiting_queue_head, remove_pos, map[remove_pos]->mem_size);
			}
		}
	}

	printf("Total loads = %d, average #processes = %.2lf, average #holes = %.2lf, cumulative %mem = %.2lf\n", num_loads, num_process / (double) num_loads
		, num_holes / (double) num_loads, getCumMemUsage(map));
}

void bestFit(process ** map, process * waiting_queue_head) {
	printf("Running Best Fit...\n");
	time = 0;
	total_loads = 0;
	total_process = 0;
	process * pop_process;
	int num_loads = 0;
	int num_holes = 0;
	int num_process = 0;
	while(waiting_queue_head != NULL) {
		/* Algo :
			* Check through all of memory and find the hole that fits the process the Best
			* if there is none that fit, then remove the oldest and try again
			*/
		pop_process = popFrontWaitingQueue(&waiting_queue_head);
		int cont = 0;
		while(!cont) {
			int insert_pos = getIndexOfBestHole(map, pop_process->mem_size);
			if(insert_pos != -1) {
				cont = 1;
				int i;
				pop_process->time_added = time;
				time++;
				for(i = insert_pos; i < insert_pos + pop_process->mem_size; i++) {
					map[i] = pop_process;
				}
				total_loads += 1;
				num_loads += 1;
				num_holes += getNumHoles(map);
				num_process += getNumProcess(map);
				double mem_usage = getMemUsage(map);
				double cum_mem_usage = getCumMemUsage(map);
				printf("%s, #processes = %d, #holes = %d, %%memusage = %.2lf, cummulative %%mem = %.2lf\n", pop_process->ID, getNumProcess(map), getNumHoles(map), mem_usage, cum_mem_usage);
			} else {
				int remove_pos = getIndexOfOldestProcess(map);
				removeProcessFromMap(map, &waiting_queue_head, remove_pos, map[remove_pos]->mem_size);
			}
		}
	}

	printf("Total loads = %d, average #processes = %.2lf, average #holes = %.2lf, cumulative %mem = %.2lf\n", num_loads, num_process / (double) num_loads
		, num_holes / (double) num_loads, getCumMemUsage(map));
}

void nextFit(process ** map, process * waiting_queue_head) {
	printf("Running Next Fit...\n");
	time = 0;
	total_loads = 0;
	total_process = 0;
	process * pop_process;
	int num_loads = 0;
	int num_holes = 0;
	int num_process = 0;
	while(waiting_queue_head != NULL) {
		/* Algo :
			* Pop the first process from the waiting queue 
			* try and put the process in the first large enough hole
			* if there is no hole that fits:
			* find the process that has been in memory the longest, remove it, place it in waiting queue (if num_times_run !> 3)
			* continue doing this until the waiting queue is empty
			*/
		pop_process = popFrontWaitingQueue(&waiting_queue_head);
		int cont = 0;
		while(!cont) {
			int insert_pos = getIndexOfNextHole(map, pop_process->mem_size);
			if(insert_pos != -1) {
				cont = 1;
				int i;
				pop_process->time_added = time;
				time++;
				for(i = insert_pos; i < insert_pos + pop_process->mem_size; i++) {
					map[i] = pop_process;
				}
				total_loads += 1;
				num_loads += 1;
				num_holes += getNumHoles(map);
				num_process += getNumProcess(map);
				double mem_usage = getMemUsage(map);
				double cum_mem_usage = getCumMemUsage(map);
				printf("%s, #processes = %d, #holes = %d, %%memusage = %.2lf, cummulative %%mem = %.2lf\n", pop_process->ID, getNumProcess(map), getNumHoles(map), mem_usage, cum_mem_usage);
			} else {
				int remove_pos = getIndexOfOldestProcess(map);
				removeProcessFromMap(map, &waiting_queue_head, remove_pos, map[remove_pos]->mem_size);
			}
		}
	}

	printf("Total loads = %d, average #processes = %.2lf, average #holes = %.2lf, cumulative %mem = %.2lf\n", num_loads, num_process / (double) num_loads
		, num_holes / (double) num_loads, getCumMemUsage(map));
}

void worstFit(process ** map, process * waiting_queue_head) {
	printf("Running Worst Fit...\n");
	time = 0;
	total_loads = 0;
	total_process = 0;
	process * pop_process;
	int num_loads = 0;
	int num_holes = 0;
	int num_process = 0;
	while(waiting_queue_head != NULL) {
		/* Algo :
			* Pop the first process from the waiting queue 
			* try and put the process in the first large enough hole
			* if there is no hole that fits:
			* find the process that has been in memory the longest, remove it, place it in waiting queue (if num_times_run !> 3)
			* continue doing this until the waiting queue is empty
			*/
		pop_process = popFrontWaitingQueue(&waiting_queue_head);
		int cont = 0;
		while(!cont) {
			int insert_pos = getIndexOfBiggestHole(map, pop_process->mem_size);
			if(insert_pos != -1) {
				cont = 1;
				int i;
				pop_process->time_added = time;
				time++;
				for(i = insert_pos; i < insert_pos + pop_process->mem_size; i++) {
					map[i] = pop_process;
				}
				total_loads += 1;
				num_loads += 1;
				num_holes += getNumHoles(map);
				num_process += getNumProcess(map);
				double mem_usage = getMemUsage(map);
				double cum_mem_usage = getCumMemUsage(map);
				printf("%s, #processes = %d, #holes = %d, %%memusage = %.2lf, cummulative %%mem = %.2lf\n", pop_process->ID, getNumProcess(map), getNumHoles(map), mem_usage, cum_mem_usage);
			} else {
				int remove_pos = getIndexOfOldestProcess(map);
				removeProcessFromMap(map, &waiting_queue_head, remove_pos, map[remove_pos]->mem_size);
			}
		}
	}

	printf("Total loads = %d, average #processes = %.2lf, average #holes = %.2lf, cumulative %mem = %.2lf\n", num_loads, num_process / (double) num_loads
		, num_holes / (double) num_loads, getCumMemUsage(map));
}

int getIndexOfBiggestHole(process ** map, int size_of_hole) {
	int i;
	int in_hole = 0;
	int largest_index = 0;
	int current_size = 0;
	int largest_size = 0;
	for(i = 0; i < MEM_SIZE;i++) {
		if(strcmp(map[i]->ID, "0") == 0) {
			if(in_hole == 0) {
				current_size = 0;
			}
			current_size += 1;
			if(current_size > largest_size) {
				largest_size = current_size;
				largest_index = i - current_size;
			}
			if(i == MEM_SIZE-1) {
				largest_index = i - current_size;
			}
			in_hole = 1;
		} else {
			if(in_hole == 1) {
				current_size = 0;
			}
			in_hole = 0;
		}
	}

	if(largest_size < size_of_hole) {
		return -1;
	} else {
		return largest_index + 1;
	}
}

int getIndexOfNextHole(process ** map, int size_of_hole) {
	int i;
	int counter = 0;
	// count through map, if there is a process reset counter, else add up the counter
	// at each interation check if we have a big enough hole
	// if there is no hole big enough return -1
	for(i = 0;i < MEM_SIZE;i++) {
		if(strcmp(map[i]->ID, "0") != 0) { // has a process
			counter = 0;
		} else {
			counter ++;
		}
		if (counter >= size_of_hole) {
			return i - size_of_hole + 1;
		}
	}
	return -1;
}

int getIndexOfBestHole(process ** map, int size_of_hole) {
	int i;
	int counter = 0;
	int indexes[MEM_SIZE];
	int sizes[MEM_SIZE];
	int num_holes = 0;
	int in_hole = 0;
	/* find the best for the size given
	* search the map and find size and positions of all the holes
	* compare with the size and return the index of the best fitting hole
	* or -1 for no hole found */
	for(i = 0;i < MEM_SIZE;i++) {
		if(strcmp(map[i]->ID, "0") == 0) { // a hole
			if(in_hole == 0) {
				//found a new hole
				indexes[num_holes] = i;
				num_holes++;
			}
			in_hole = 1;
			if(i == MEM_SIZE-1) {
				sizes[num_holes-1] = MEM_SIZE - indexes[num_holes-1];
			}
		} else {
			if(in_hole == 1) {
				sizes[num_holes] = i - indexes[num_holes];
			}
			in_hole = 0;
		}
	}

	if(num_holes == 0) {
		return -1;
	}

	int smallest_difference = sizes[0] - size_of_hole;
	int pos = indexes[0];

	if(pos + size_of_hole > MEM_SIZE) {
		return -1;
	}
	
	for(i = 0;i < num_holes - 1;i++) {
		if((sizes[i] - size_of_hole >= 0) && (sizes[i] - size_of_hole < smallest_difference)) {
			if(sizes[i] >= size_of_hole) {
				pos = indexes[i];
			} else {
				pos = -1;
			}
		}
	}

	return pos;
}

void printMap(process ** map) {
	int i;
	for (i = 0;i < MEM_SIZE/2;i++) {
		printf("%d: %s %d %d %d		%d: %s %d %d %d\n", i + 1, map[i]->ID, map[i]->mem_size, map[i]->num_times_run, map[i]->time_added,
		 i + MEM_SIZE/2 + 1, map[i + MEM_SIZE/2]->ID, map[i + MEM_SIZE/2]->mem_size, map[i + MEM_SIZE/2]->num_times_run,  map[i + MEM_SIZE/2]->time_added);
	}
}

void printWaitingQueue(process * waiting_queue) {
	puts("-------------------");
	process * temp = waiting_queue;
	while(temp != NULL) {
		puts(temp->ID);
		temp = temp->next;
	}
	puts("--------------------");
}

process * createProcess(char * ID, int mem_size, int num_times_run, int time_added) {
	process * temp_process = malloc(sizeof(process));

	char * temp_ID = strdup(ID);

	temp_process->ID = temp_ID;
	temp_process->mem_size = mem_size;
	temp_process->num_times_run = num_times_run;
	temp_process->time_added = time_added;
	temp_process->next = NULL;

	return temp_process;
}

void freeProcess(process * the_process) {
	free(the_process->ID);
	free(the_process);
}

void freeWaitingQueue(process * queue_head) {
	process * temp_process = queue_head;
	while(queue_head != NULL) {
		temp_process = queue_head->next;
		free(queue_head);
		queue_head = temp_process;
	}
}

process * popFrontWaitingQueue(process ** queue_head) {
	if(queue_head == NULL) {
		return NULL;
	}

	process * return_process = *queue_head;
	*queue_head = (*queue_head)->next;
	return return_process;
}

process * addToWaitingQueue(process * queue_head, char * ID, int mem_size, int num_times_run, int time_added) {
	if(queue_head == NULL) {
		process * temp_process = createProcess(ID, mem_size, num_times_run, time_added);
		return temp_process;
	}

	process * temp_process = queue_head;

	while(temp_process->next != NULL) {
		temp_process = temp_process->next;
	}

	process * addProcess = createProcess(ID, mem_size, num_times_run, time_added);
	temp_process->next = addProcess;

	return queue_head;
}

int getNumProcess(process ** map) {
	int i;
	int cur_in_hole = 1;
	int pro_count = 1;
	for(i = 0; i < MEM_SIZE;i++) {
		if(strcmp(map[i]->ID, "0") != 0) {
			if(i != 0) {
				/* dont want to go out of bounds */
				if(strcmp(map[i]->ID, map[i-1]->ID) != 0){
					/* we have a new process */
					pro_count += 1;
				} else if (cur_in_hole == 1) {
					/* we are just coming out of a hole */
					pro_count += 1;
				}
			}
			cur_in_hole = 0;
		} else {
			cur_in_hole = 1;
		}
	}
	return pro_count;
}

int getNumHoles(process ** map) {
	int i;
	int cur_in_process = 1;
	int hole_count = 0;
	for(i = 0; i < MEM_SIZE;i++) {
		if(strcmp(map[i]->ID, "0") != 0) {
			cur_in_process = 1;
		} else {
			/* if we are currently at a hole, and we were at a process last position
			 or this is the first one (cur_in_process) is set to 1 at default, then we 
			 have new hole */
			if(cur_in_process == 1) {
				hole_count += 1;
			}
			cur_in_process = 0;
		}
	}
	return hole_count;
}

double getMemUsage(process ** map) {
	int i;
	int process_count_cell = 0;
	for(i = 0; i < MEM_SIZE;i++) {
		if(strcmp(map[i]->ID, "0") != 0) {
			process_count_cell += 1;
		}
	}

	double usage = (double)process_count_cell / (double) MEM_SIZE;
	total_process += process_count_cell;
	return usage;
}

double getCumMemUsage(process ** map) {
	double usage = (double) total_process / (double) (MEM_SIZE * total_loads);
	return usage;
}

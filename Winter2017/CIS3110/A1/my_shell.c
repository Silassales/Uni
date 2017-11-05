#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

//function decl
extern char ** getln();
void forkProccess (char **, int);
long int addValue(char **);
void countArgs(char **);
int containsQuotes (char *);
int containsGreaterLessThan(char **);

int main() {
	int num_of_args;
	char **args;
	char outputFile[256];
	int been_redirected = 0;
	FILE * fp = NULL;
	
	while(1) {
		printf(">> ");
		args = getln();

		int location_of_arrow;
		if((location_of_arrow = containsGreaterLessThan(args)) != 0) {
			strcpy(outputFile, args[location_of_arrow + 1]);
			fp = freopen(outputFile, "w+", stdout);
			args[location_of_arrow] = NULL;
			been_redirected = 1;
		}

		// get the number of args
		for(num_of_args = 0; args[num_of_args] != NULL; num_of_args++);

		if (strcmp(args[0], "exit") == 0) {
			if (num_of_args == 1) exit(0);
			else printf("Error: exit takes no arguments.\n");
		} else if (strcmp(args[0], "topSecret") == 0) {
			system("python secret.py");
		} else if (*args[num_of_args - 1] == '&') {
			args[num_of_args - 1] = NULL;
			forkProccess(args, 1);
		} else if (strcmp(args[0], "add") == 0) {
			if (num_of_args < 2) printf("Error: add needs atlest one argument.\n");
			else printf("Sum: %ld\n", addValue(args));
		} else if (strcmp(args[0], "arg") == 0) {
			countArgs(args);
		} else {
			forkProccess(args, 0);
			if(been_redirected) { // redirect back to stdout
			// May cause progarm to crash on non-linux systems, you have been warned
				fp = freopen("/dev/tty", "a", stdout);				
				been_redirected = 0;
			}
		}
	} 
	
	fclose(fp);
	return 0;
}

void forkProccess (char ** command_list, int background) {
	pid_t pid;
	int error, child_error;

	pid = fork();

	if(pid == 0) { //child
		error = execvp(command_list[0], command_list);
		if (error == -1) {
			perror("msh");
			exit(1);
		} 
		if(background) {
			signal(SIGCONT, NULL);
			return;
		}
	} else if (pid < 0) {
		perror("msh");	
	} else {
		wait(&child_error);
	}
}

long int addValue(char ** args) {
	long int sum = 0;
	int i;
	for(i = 1; args[i] != NULL; i++) {
		sum += strtol(args[i], NULL, 0);
	}
	return sum;
}

void countArgs(char ** args) {
	int found_quotes = -1; // -1 equals no, 1 equals yes
	int num_of_args = 0;
	int i;
	for(i = 1; args[i] != NULL; i++) {
		if (containsQuotes(args[i])) {
			found_quotes = found_quotes * -1;
		}
		if (found_quotes == -1) {
			num_of_args += 1;
		}
	}

	printf("argc = %d, args = ", num_of_args);

	for(i = 1; args[i + 1] != NULL; i++) {
		printf("%s, ", args[i]);
	}
	printf("%s\n", args[i]);
}

int containsQuotes (char * the_string) {
	if (strchr(the_string, '\"') == NULL) return 0;
	return 1;
}

int containsGreaterLessThan(char ** args) {
	int i;
	for(i = 1; args[i] != NULL; i++) {
		if (*args[i] == '>' || *args[i] == '<') {
			return i;
		}
	}
	return 0;
}

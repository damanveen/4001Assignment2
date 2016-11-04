#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct process process;

process *curr, *iter;
process *ListProcess;
FILE *file;

int i;
int totalProcesses = 0;
int totalTurnaroundTime = 0;
int waitTime = 0;
time_t start_t, end_t;

struct process{
	pthread_t tid;
	int priority;
	int arrival_time;
	int cpu_time;
	int added;
	int waiting_time;
	int finish_time;
	double turnaround_time;
	process *next;
};

/*
 * Get the total number of entries that has been entered
 * in the input file
 */ 
int getNumEntries(){
	int numProcess = 0;	
	file = fopen("testPriority.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        numProcess++;
    }
    return numProcess;
}

/*
 * Have the threads run their process
 */
void* running(void *i){
	//Get the index that the thread was created at
	int a = *((int*) i);
	process *NewItem = malloc(sizeof(process));
	NewItem->priority = ListProcess[a].priority;

	while(1){
		//Check if the thread is the current reference in the linked list & has been added to the linked list
		if(curr != NULL && curr->tid == pthread_self()  && curr->added == 1){
			time_t current_t = time(NULL);

			//Begin "Simulation" of execution
			printf("---------\n");
			printf("|THREAD %d|\n", a);
			printf("---------\n");
			printf("Priority = %i\n", curr->priority);
			//Execute
			printf("Requested Time: %i Executing at: %f\n", curr->cpu_time,difftime(current_t, start_t));
			sleep(curr->cpu_time/1000);
			current_t = time(NULL);

			//Display time taken to run
			printf("Finished running at: %f\n", difftime(current_t, start_t));
			curr->turnaround_time = difftime(current_t, start_t) - curr->arrival_time/1000;
			totalTurnaroundTime = totalTurnaroundTime + curr->turnaround_time;
		
			//Display other information
			printf("Turnaround time is: %f\n", curr->turnaround_time);
			printf("Waittime is: %d\n\n",  waitTime);
			waitTime = waitTime + (curr->cpu_time/1000);

			//Increment to the total number of processes executed
			totalProcesses++;

			//Change the reference in the linked list
			curr = curr->next;

			//Terminate thread
			return NULL;
		}

		time_t current_t = time(NULL);

		//Check if the time of arrival matches the times and it has not yet been added to the list
		if(ListProcess[a].arrival_time/1000 == difftime(current_t, start_t) && NewItem->added == 0){
			//Establish properties of a new thread to add to the linked list
			NewItem->tid = pthread_self();
			NewItem->arrival_time = ListProcess[a].arrival_time;
			NewItem->cpu_time = ListProcess[a].cpu_time;
			NewItem->next = NULL;
			
			//Check if this thread is the first thread in the list
 			if(curr == NULL){
 				//Set current & tail reference to this thread
 				iter = NewItem;
				curr = NewItem;
			}
			//Else there already exists a thread in the list
			else{
				for(iter; iter != NULL; iter = iter->next){
					if(NewItem->priority > iter->priority && NewItem->priority < iter->next->priority){
						NewItem->next = iter->next;
						iter->next = NewItem;
					}
				}
			}
			printf("%d was added to the ready queue at %f\n\n", a, difftime(current_t, start_t));
			//Set that the thread has successfully been added
			NewItem->added = 1;
		}
	}
}

void summary(double totalTime){
	printf("---------\n");
	printf("|Summary|\n");
	printf("---------\n");


	printf("Total run time: %f seconds\n", totalTime);
	printf("Average turnaround time: %d seconds\n", (totalTurnaroundTime/totalProcesses));
	printf("Average wait time: %d seconds\n", (waitTime/totalProcesses));
	printf("Throughput: %f   %i Processes/second\n", totalProcesses/totalTime, totalProcesses);
}

int main(){
	start_t = time(NULL);
	char line[256];
	char *token, s[2] = ",";
	int number = getNumEntries();
	int pos = 0, row = 0;;
	int *arg;
	int boolean = 0;
	int reading;

	//Create an array of processes
    ListProcess = malloc(sizeof(process)*number);
    arg = malloc(sizeof(*arg));

    //Open a file named test.txt
   	file = fopen("testPriority.txt", "r");
	while (fgets(line, sizeof(line), file)) {
		//For each line, get the first input
	    token = strtok(line,s);
	    //Set the first input to the arrival time
	    pos = 0;
	    while(token != NULL){
	    	reading = atoi(token);
	    	if(pos == 0){
	    		ListProcess[row].arrival_time = reading;
	    	}
	    	else if(pos == 1){
	    		ListProcess[row].cpu_time = reading;
	    	}
	    	else if(pos == 2){
	    		ListProcess[row].priority = reading;
	    	}
	    	token = strtok(NULL, s);
	    	pos++;
	    }
	    row++;
	}
	fclose(file);

	//Create threads for each number of inputs made	
	for(i = 0; i < number; i++){
		printf("Creating thread %i. . . \n\n", i);
		ListProcess[i].added = 0;
		*arg = i;
		pthread_create(&ListProcess[i].tid, NULL, (void*)running, arg);
		//Slow down thread process to allow pointer to change slowly enough
		sleep(1);

	}
    
    //Join the main thread to each process
	for(i = 0; i < number; i++){
		pthread_join(ListProcess[i].tid,NULL);
	}
    
	end_t = time(NULL);

	//Print out summary of information
	double time = difftime(end_t, start_t);
	summary(time);
	exit(0);
}

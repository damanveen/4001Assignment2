#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct process process;

process *curr;
process *ListProcess;
FILE *file;

int i;
int totalProcesses = 0;
int totalTurnaroundTime = 0;
int waitTime = 0;
clock_t start_t , end_t;


struct process{
	pthread_t tid;
	int arrival_time;
	int cpu_time;
	int added;
	int waiting_time;
	int finish_time;
	int turnaround_time;
	process *next;
};

void sec_wait(int sec){
	clock_t wait_till_end;
	wait_till_end = clock() + sec * CLOCKS_PER_SEC; 
	while(clock() < wait_till_end){}
}
 
int getNumEntries(){
	int numProcess = 0;	
	file = fopen("test.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        numProcess++;
    }
    return numProcess;
}

void* running(void *i){
	int a = *((int*) i);
	printf("Index value %i\n", a);
	process *NewItem = malloc(sizeof(process));
	while(1){
		if(ListProcess[a].arrival_time < (double)clock()/CLOCKS_PER_SEC*10000 && ListProcess[a].added == 0){
			NewItem->tid = pthread_self();
			NewItem->arrival_time = ListProcess[a].arrival_time;
			NewItem->cpu_time = ListProcess[a].cpu_time;
			NewItem->next = NULL;


			
 			if(curr == NULL){
				curr = NewItem;
			}
			else{
				curr->next = NewItem;	
			}

			NewItem->added = 1;
		}

		if(curr != NULL && curr->tid == pthread_self()){
			printf("%lu\n", pthread_self());
			printf("Thread executing at %f\n", ((double)clock())/CLOCKS_PER_SEC);
			printf("Thread is running (Sleeping).\n");

			sleep(curr->cpu_time/1000);
			printf("Thread run time: %f, thread arrival time: %f\n", (double)clock(), (double)curr->arrival_time);
			
			printf("Thread is done running: %f\n\n", (double)clock()/CLOCKS_PER_SEC);

			curr->turnaround_time = (curr->cpu_time/1000) - (curr->arrival_time/1000);
			totalTurnaroundTime = totalTurnaroundTime + curr->turnaround_time;
		
			printf("Thread turnaround time is: %d\n", curr->turnaround_time);
			printf("Thread waittime is: %d\n\n", waitTime);

			waitTime = waitTime + (curr->cpu_time/1000);
			totalProcesses++;



			curr = curr->next;


			return NULL;
		}
	}
}

int main(){
	start_t = clock();
	char line[256];
	char *token, s[2] = ",";
	int number = getNumEntries();
	int pos = 0;
	int *arg;
    ListProcess = malloc(sizeof(process)*number);
    arg = malloc(sizeof(*arg));
   	file = fopen("test.txt", "r");
	while (fgets(line, sizeof(line), file)) {
	    token = strtok(line,s);
	    ListProcess[pos].arrival_time = atoi(token);
	    while(token != NULL){
	    	ListProcess[pos].cpu_time = atoi(token);
	    	token = strtok(NULL,s);
	    }
	    pos++;
	}
	fclose(file);
	
	for(i = 0; i < number; i++){
		//Add method of changing pointer slower
		printf("Creating thread. . . \n");
		*arg = i;
		pthread_create(&ListProcess[i].tid, NULL, (void*)running, arg);
		sleep(1);
	}
    
	for(i = 0; i < number; i++){
		pthread_join(ListProcess[i].tid,NULL);
	}
    
	//fflush(stdout);

	end_t = clock();
	printf("---------\n");
	printf("|Summary|\n");
	printf("---------\n");


	printf("Total run time: %f seconds\n", (double)end_t/CLOCKS_PER_SEC);
	printf("Average turnaround time: %d seconds\n", (totalTurnaroundTime/totalProcesses));
	printf("Average wait time: %d seconds\n", (waitTime/totalProcesses));
	printf("Throughput: %f   5 Processes/second\n", (totalProcesses/((double)clock()/CLOCKS_PER_SEC)));
	exit(0);


}

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

typedef struct process process;

process *curr;
process *tail;
process *IOpointer;

process *ListProcess;
FILE *file;

int i;
int totalProcesses = 0;
int totalTurnaroundTime = 0;
int waitTime = 0;
time_t start_t, end_t;




struct process{
	pthread_t tid;
	int arrival_time;
	int cpu_time;
	int added;
	int index;
	int waiting_time;
	int finish_time;
	int io_duration;
	int io_frequency ;
	double turnaround_time;
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
	
	process *NewItem = malloc(sizeof(process));

	while(1){
		
		if(curr != NULL && curr->tid == pthread_self()  && curr->added == 1){
			

			time_t current_t = time(NULL);


			printf("---------\n");
			printf("|THREAD %d|\n", a);
			printf("---------\n");
			printf("Executing at: %f\n",  difftime(current_t, start_t));
			

			sleep(curr->cpu_time/1000);
			current_t = time(NULL);

			printf("Finished running at: %f\n", difftime(current_t, start_t));

			curr->turnaround_time = difftime(current_t, start_t) - curr->arrival_time/1000;
			totalTurnaroundTime = totalTurnaroundTime + curr->turnaround_time;
		
			


			printf("Turnaround time is: %f\n", curr->turnaround_time);
			printf("Waittime is: %d\n\n",  waitTime);

			waitTime = waitTime + (curr->cpu_time/1000);
			totalProcesses++;



			curr = curr->next;


			return NULL;
		}

		time_t current_t = time(NULL);
		
		
		if(ListProcess[a].arrival_time/1000 == difftime(current_t, start_t) && NewItem->added == 0){
			

			NewItem->tid = pthread_self();
			NewItem->arrival_time = ListProcess[a].arrival_time;
			NewItem->cpu_time = ListProcess[a].cpu_time;
			NewItem->next = NULL;


			
 			if(curr == NULL){
				curr = NewItem;
				tail = NewItem;
			}
			else{

				tail->next = NewItem;
				tail = tail->next;	
			}

			printf("%d was added to the ready queue at %f\n\n", a, difftime(current_t, start_t));


			NewItem->added = 1;
		}

		
	}
}

void* io_running(void *i){

	int a = *((int*) i);
	//curr->index = a;
	process *NewItem = malloc(sizeof(process));
	ListProcess[a].io_frequency = 1000;
	ListProcess[a].io_duration = 1000;
	int t = 0;
	while(1){
		if(curr != NULL && curr->tid == pthread_self()  && curr->added == 1){
			

			time_t temp_t = time(NULL);
			

			//printf("---------\n");
			//printf("|THREAD %d|\n", a);
			//printf("---------\n");
			//printf("Executing at: %f\n",  difftime(current_t, start_t));
			

			while(curr->cpu_time > 0){

				printf("Thread %d is executing IO for %d second. Remaining CPU:%d\n",a, curr->io_duration/1000, curr->cpu_time);
				sleep(1);
				curr->cpu_time-=1000;
				//tail->next = curr;
				//curr = curr->next; 

				if(curr->next == NULL){

					sleep(1);

				}else if(curr->next != NULL){
					IOpointer = curr->next;
					
					if(IOpointer->cpu_time > 0){
						printf("Thread %d is executing IO for %d second. Remaining CPU:%d\n",IOpointer->index, IOpointer->io_duration/1000, IOpointer->cpu_time);
						sleep(1);
						IOpointer->cpu_time-=1000;
					}
					

				}
				
			}

			 if(curr->cpu_time <= 0){

				time_t current_t = time(NULL);

				printf("Finished running at: %f\n", difftime(current_t, start_t));

				curr->turnaround_time = difftime(current_t, start_t) - curr->arrival_time/1000;
				totalTurnaroundTime = totalTurnaroundTime + curr->turnaround_time;
		
			


				printf("Turnaround time is: %f\n", curr->turnaround_time);
				printf("Waittime is: %d\n\n",  waitTime);

				waitTime = waitTime + (curr->cpu_time/1000);
				totalProcesses++;

				if(curr->next != NULL){

					curr = curr->next;

				}


				
				return NULL;
			}
			
		}

		
		time_t current_t = time(NULL);
		
		
		if(ListProcess[a].arrival_time/1000 == difftime(current_t, start_t) && NewItem->added == 0){
			

			NewItem->tid = pthread_self();
			NewItem->arrival_time = ListProcess[a].arrival_time;
			NewItem->cpu_time = ListProcess[a].cpu_time;
			NewItem->next = NULL;
			NewItem->io_frequency = ListProcess[a].io_frequency;

			
 			if(curr == NULL){
				curr = NewItem;
				tail = NewItem;
			}
			else{

				tail->next = NewItem;
				tail = tail->next;	
			}

			printf("%d was added to the ready queue at %f\n\n", a, difftime(current_t, start_t));


			NewItem->added = 1;
		}

		//printf("%i\n", t);
	}





}

void summary(double totalTime){

	printf("---------\n");
	printf("|Summary|\n");
	printf("---------\n");


	printf("Total run time: %f seconds\n", totalTime);
	printf("Average turnaround time: %d seconds\n", (totalTurnaroundTime/totalProcesses));
	printf("Average wait time: %d seconds\n", (waitTime/totalProcesses));
	printf("Throughput: %f   5 Processes/second\n", totalProcesses/totalTime);



}



int main(){
	start_t = time(NULL);

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
		printf("Creating thread %i. . . \n\n", i);
		*arg = i;
		ListProcess[i].added = 0;
		
		//ListProcess[i].io_duration = 1000;
		//ListProcess[i].io_frequency = 1000;

		pthread_create(&ListProcess[i].tid, NULL, (void*)io_running, arg);
		
		
		sleep(1);

	}
    
	for(i = 0; i < number; i++){
		pthread_join(ListProcess[i].tid,NULL);
	}
    
	

	end_t = time(NULL);


	summary(difftime(end_t, start_t));

	exit(0);

}


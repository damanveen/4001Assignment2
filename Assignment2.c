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

int temp = 0;
int number;
int i, j;
int totalProcesses = 0;
int totalTurnaroundTime = 0;
int waitTime = 0;
time_t start_t, end_t, current_t;




struct process{
	pthread_t tid;
	int arrival_time;
	int cpu_time;
	int HC_cpu_time;
	int added;
	int done;
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
void endProcess(process *p){


		current_t = time(NULL);


		p->finish_time = difftime(current_t, p->arrival_time);
		p->turnaround_time = p->finish_time - p->arrival_time/1000;
		totalTurnaroundTime = totalTurnaroundTime + p->turnaround_time;
			
		p->waiting_time = p->waiting_time + (p->HC_cpu_time/1000);
				
		p->done = 1;
		totalProcesses++;

		if(p->next != NULL){

			curr = curr->next;

		}


}
void* io_running(void *i){
	int a = *((int*) i);
	//curr->index = a;
	process *NewItem = malloc(sizeof(process));

	while(1){
		



		if(curr != NULL && curr->tid == pthread_self()  && curr->added == 1){
			

			time_t temp_t = time(NULL);
			

			if(curr->cpu_time > 0){

				printf("Main Thread %d is executing IO for %d second. Remaining CPU:%d\n",curr->arrival_time, curr->io_duration/1000, curr->cpu_time);
				sleep(1);
				curr->cpu_time-=1000;
				//tail->next = curr;
				//curr = curr->next; 

				if(curr->next == NULL){

					sleep(1);

				}


				
			}

			if(curr->next != NULL){
					IOpointer = curr->next;
					
					if(IOpointer->cpu_time >= 0){
						printf("IO Thread %d is executing IO for %d second. Remaining CPU:%d\n\n",IOpointer->arrival_time, IOpointer->io_duration/1000, IOpointer->cpu_time);
						sleep(1);
						IOpointer->cpu_time-=1000;
					}

					if(IOpointer->cpu_time == 0){
						IOpointer->done = 1;
						curr->next = IOpointer->next;
						//endProcess(IOpointer);
					}

				}

			 if(curr->cpu_time == 0){
			 		curr->done = 1;
			 		endProcess(curr);

				
			}
			
		}



		
		time_t current_t = time(NULL);
		
		
		if(ListProcess[a].arrival_time/1000 == difftime(current_t, start_t) && NewItem->added == 0){
			

			NewItem->tid = pthread_self();
			NewItem->arrival_time = ListProcess[a].arrival_time;
			NewItem->cpu_time = ListProcess[a].cpu_time;
			NewItem->HC_cpu_time = ListProcess[a].cpu_time;
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

		
		
			if(curr->done == 1){
				temp++;
				//printf("temp: %i\n\n\n", temp);
			}

			if(temp == number){
				break;
			}
		
		
		
		
	}

	double t;
	for(j = 0; j < number; j++){
			
			t = pthread_cancel(ListProcess[j].tid);
			//printf("%f\n", t);
			if(t == 0){

				printf("killing thread %d\n", j);

			}else{

				printf("error killing %d", j);
			}
			
		
		}

	


}

void summary(double totalTime){

	printf("---------\n");
	printf("|Summary|\n");
	printf("---------\n");


	for(i = 0 ; i < number; i++){


			printf("---------\n");
			printf("|THREAD %d|\n", i);
			printf("---------\n");
			


	printf("Finished running at: %i\n", ListProcess[i].finish_time );
	printf("Turnaround time is: %f\n", ListProcess[i].turnaround_time);
	printf("Waittime is: %d\n\n",  ListProcess[i].waiting_time);



	}
	

	printf("Total run time: %f seconds\n", totalTime);
	printf("Average turnaround time: %d seconds\n", (totalTurnaroundTime/totalProcesses));
	printf("Average wait time: %d seconds\n", (waitTime/totalProcesses));
	printf("Throughput: %f   5 Processes/second\n", totalProcesses/totalTime);



}



int main(){
	start_t = time(NULL);

	char line[256];
	char *token, s[2] = ",";
    number = getNumEntries();
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
		//printf("look here: %f\n",(double)ListProcess[i].tid);
		
		
		
		

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


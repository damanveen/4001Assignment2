#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

struct process{
	pthread_t tid;
	int arrival_time;
	int cpu_time;
	int waiting_time;
	struct process *next;
	int finish_time;
	int turnaround_time;
	
};



struct process *p1, *p2, *p3, *p4, *p5;
struct process *curr;
int totalProcesses = 0;
int totalTurnaroundTime = 0;
int waitTime = 0;

void sec_wait(int sec){

	clock_t wait_till_end;

	wait_till_end = clock() + sec * CLOCKS_PER_SEC; 

	while(clock() < wait_till_end){}
}

void* running(){	
	while(1){
		if(curr != NULL && curr->tid == pthread_self()){
			printf("%lu\n", pthread_self());
			printf("Thread executing at %f\n", ((double)clock()/CLOCKS_PER_SEC));
			
			printf("Thread is running (Sleeping).\n");
			
			sec_wait(curr->cpu_time/1000);



			


			printf("Thread is done running.\n\n");
		

			curr->turnaround_time = (((double)clock()/CLOCKS_PER_SEC)) - (curr->arrival_time/1000);
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
	char *token, s[2] = ",";
    FILE *file = fopen("test.txt", "r");
    char line[256];
    while (fgets(line, sizeof(line), file)) {
    	printf("%s", line);
        strtok(line,s);
        while(token != NULL){
        	printf("%s\n", token);
        	token = strtok(NULL,s);
        }
    }
    fclose(file);

	struct process *p1 = (struct process*) malloc(sizeof(struct process));
	struct process *p2 = (struct process*) malloc(sizeof(struct process));
	struct process *p3 = (struct process*) malloc(sizeof(struct process));
	struct process *p4 = (struct process*) malloc(sizeof(struct process));
	struct process *p5 = (struct process*) malloc(sizeof(struct process));

	p1->arrival_time = 0;
	p2->arrival_time = 9000;
	p3->arrival_time = 12000;
	p4->arrival_time = 13000;
	p5->arrival_time = 17000;

	p1->cpu_time = 22000;
	p2->cpu_time = 11000;
	p3->cpu_time = 12000;
	p4->cpu_time = 11000;
	p5->cpu_time = 14000;

	p1->next = p2;
	p2->next = p3;
	p3->next = p4;
	p4->next = p5;
	p5->next = NULL;

	curr = p1;

	sleep(1);

	pthread_create(&p1->tid, NULL, (void*)running, NULL);
	pthread_create(&p2->tid, NULL, (void*)running, NULL);
	pthread_create(&p3->tid, NULL, (void*)running, NULL);
	pthread_create(&p4->tid, NULL, (void*)running, NULL);
	pthread_create(&p5->tid, NULL, (void*)running, NULL);

	pthread_join(p1->tid, NULL);
	pthread_join(p2->tid, NULL);
	pthread_join(p3->tid, NULL);
	pthread_join(p4->tid, NULL);
	pthread_join(p5->tid, NULL);
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	free(p5);

	printf("All` threads finished executing.\n\n");
	fflush(stdout);

	printf("---------\n");
	printf("|Summary|\n");
	printf("---------\n");
	printf("Total run time: %f seconds\n", (double)clock()/CLOCKS_PER_SEC);
	printf("Average turnaround time: %d seconds\n", (totalTurnaroundTime/totalProcesses));
	printf("Average wait time: %d seconds\n", (waitTime/totalProcesses));
	printf("Throughput: %f   5 Processes/second\n", (totalProcesses/((double)clock()/CLOCKS_PER_SEC)));
	exit(0);
}

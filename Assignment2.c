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


struct process{
	pthread_t tid;
	int arrival_time;
	int cpu_time;
	int added;
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
		if(ListProcess[a].arrival_time < ((double)clock())/CLOCKS_PER_SEC){
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
		}

		if(curr != NULL && curr->tid == pthread_self()){
			printf("%lu\n", pthread_self());
			printf("Thread executing at %f\n", ((double)clock())/CLOCKS_PER_SEC);
			printf("Thread is running (Sleeping).\n");
			sleep(curr->cpu_time/1000);
			printf("Thread is done running.\n\n");
			curr = curr->next;
			return NULL;
		}
	}
}

int main(){
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
	for(int i = 0; i < number; i++){
		//Add method of changing pointer slower
		printf("Creating thread. . . \n");
		*arg = i;
		pthread_create(&ListProcess[i].tid, NULL, (void*)running, arg);
		sleep(1);
	}
    
	for(int i = 0; i < number; i++){
		pthread_join(ListProcess[i].tid,NULL);
	}
    


    /*
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
	exit(0);*/
}

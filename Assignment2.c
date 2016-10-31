#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

struct process{
	pthread_t tid;
	int arrival_time;
	int cpu_time;
	int waiting_time;
	struct process *next;
	char name;
};

pthread_t test;
struct process *p1, *p2, *p3, *p4, *p5;
struct process *head;
struct process *curr;

void* running(){	
	while(1){
		if(curr != NULL && curr->tid == pthread_self()){
			printf("%lu\n", pthread_self());
			printf("Thread executing at %f\n", (double)clock()/CLOCKS_PER_SEC);
			fflush(stdout);
			printf("Thread is running (Sleeping).\n");
			fflush(stdout);
			sleep(curr->cpu_time/1000);
			printf("Thread is done running.\n\n");
			fflush(stdout);
			curr = curr->next;
			return NULL;
		}
	}
}

int main(){
	struct process *head = (struct process*) malloc(sizeof(struct process));
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

	p1->cpu_time = 1000;
	p2->cpu_time = 1000;
	p3->cpu_time = 1000;
	p4->cpu_time = 1000;
	p5->cpu_time = 1000;

	head->next = p1;
	p1->next = p2;
	p2->next = p3;
	p3->next = p4;
	p4->next = p5;
	p5->next = NULL;

	curr = head;
	curr = curr->next;

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
	free(head);
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	free(p5);

	printf("All threads finished executing.\n\n");
	fflush(stdout);

	printf("---------\n");
	printf("|Summary|\n");
	printf("---------\n");
	printf("Total run time: %f seconds\n", (double)clock()/CLOCKS_PER_SEC);
	exit(0);
}
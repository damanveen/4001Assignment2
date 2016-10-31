#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

struct process{
	pthread_t pid;
	int arrival_time;
	int cpu_time;
	int waiting_time;
	struct process *next;
};

pthread_t test;
struct process *p1, *p2, *p3, *p4, *p5;
struct process *head;
int i = 0;

void* running(void* data){	
	i++;
	/*
	for(curr; curr != NULL; curr = curr->next){
		printf("test");
	}*/
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

	p1->cpu_time = 22000;
	p2->cpu_time = 11000;
	p3->cpu_time = 12000;
	p4->cpu_time = 11000;
	p5->cpu_time = 14000;

	head->next = p1;
	p1->next = p2;
	p2->next = p3;
	p3->next = p4;
	p4->next = p5;
	p5->next = NULL;

	struct process *curr;
	curr = head;
	curr = curr->next;

	pthread_create(&p1->pid, NULL, (void*)running, curr);
	pthread_create(&p2->pid, NULL, (void*)running, curr);
	pthread_create(&p3->pid, NULL, (void*)running, curr);
	pthread_create(&p4->pid, NULL, (void*)running, curr);
	pthread_create(&p5->pid, NULL, (void*)running, curr);

	sleep(1);
	printf("%i\n", i);
	free(head);
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	free(p5);
}
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

struct process{
	int pid;
	int arrival_time;
	int cpu_time;
	int waiting_time;
	struct process *next;
};


struct process *head = NULL;

int main(){
	struct process *head = (struct process*) malloc(sizeof(struct process));
	struct process *p1 = (struct process*) malloc(sizeof(struct process));
	struct process *curr = head;

	head->pid = 5;
	p1->pid = 1000;

	head->next = p1;

	while(curr != NULL){
		printf("%i\n",curr->pid);
		curr = curr->next;
	}
}
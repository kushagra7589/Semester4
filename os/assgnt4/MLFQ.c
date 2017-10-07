#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_SIZE 1000
#define UNIT 100000000

typedef struct process {
	int id;
	int arrival;
	int burst;
	int start;
	int end;
	int runtime;
	int time_in_queue;
	int curr_queue;
} process;

process NO_PROCESS = {-1, -1, -1, -1, -1, 0, 0};

int isNOPROCESS(process *p){	return (p->id == -1); }

process createProcess(int id, int arrival , int burst) {
	process p;
	p.id = id;
	p.arrival = arrival;
	p.burst = burst;
	p.start = -1;
	p.end = -1;
	p.runtime = 0;
	p.time_in_queue = 0;
	p.curr_queue = -1;
	return p;
}

typedef struct queue {
	process arr[MAX_SIZE];
	int front, rear;
} queue;

void init_queue(queue *q) {
	q->front = 0;
	q->rear = 0;
}

int isEmpty(queue *q) {
	if(q->rear - q->front == 0) {
		init_queue(q);
		return 1;
	}
	return 0;
}

void push(queue *q, process *p) {
	q->arr[q->rear] = *p;
	q->rear += 1;
}

process* TOP(queue *q){ return &q->arr[q->front]; }

process* pop(queue *q) {
	if(!isEmpty(q)) {
		process *temp = &q->arr[q->front];
		q->front += 1;
		return temp;
	}
	else 
		return &NO_PROCESS;
}

int timer = 0;

int MLFQ_empty(queue MLFQ[], int N_q)
{
	int i = 0;
	for(i = 0; i < N_q; i++) {
		if(!isEmpty(&MLFQ[i]))
			return 0;
	}
	return 1;
}

void print(queue MLFQ[], int N_q) {
	printf("-------------------------------------------------------\n");
	printf("At time %d\n", timer);
	for(int i = 0; i < N_q; i++) {
		printf("In queue %d\n", i);
		int j;
		for(j = MLFQ[i].front; j < MLFQ[i].rear; j++) {
			printf("Process : %d -> ", MLFQ[i].arr[j].id);
		}
		printf("\n");
	}
	printf("-------------------------------------------------------\n");
	printf("\n");

}

int main()
{
	queue MLFQ[20];
	int N_q, N_p;
	scanf("%d %d", &N_q, &N_p);
	int i = 0;
	for(i = 0; i < N_q; i++)
		init_queue(&MLFQ[i]);
	int arrival, burst;
	process processes[MAX_SIZE];
	for(i = 1; i <= N_p; i++) {
		scanf("%d %d", &arrival, &burst);
		process p = createProcess(i, arrival, burst);
		processes[i] = p;
	}
	int time_slice[20], boost_time;
	for(i = 0; i < N_q; i++) {
		scanf("%d", &time_slice[i]);
	}
	scanf("%d", &boost_time);
	int next_process = 1;
	int current_queue;
	while(!MLFQ_empty(MLFQ, N_q) || next_process <= N_p) {
		while(next_process <= N_p && processes[next_process].arrival == timer) {
			push(&MLFQ[0], &processes[next_process]);
			next_process += 1;
			printf("New process added.\n");
			print(MLFQ, N_q);
		}
		if(timer % boost_time == 0) {
			for(i = 1; i < N_q; i++) {
				while(!isEmpty(&MLFQ[i])){
					process *p = pop(&MLFQ[i]);
					(*p).time_in_queue = 0;
					push(&MLFQ[0], p);
				}
			}
			printf("Priority Boost\n");
			print(MLFQ, N_q);
		}
		for(i = 0; i < N_q; i++) {
			if(!isEmpty(&MLFQ[i])) {
				current_queue = i;
				break;
			}
		}
		process *p = TOP(&MLFQ[current_queue]);
		if((*p).start == -1){
			(*p).start = timer;
			processes[p->id].start = timer;
		}
		(*p).time_in_queue += 1;
		(*p).runtime += 1;
		int j;
		// for(j = 0; j < UNIT; j++);	//delay
		timer += 1;
		if((*p).runtime == (*p).burst) {
			pop(&MLFQ[current_queue]);
			(*p).end = timer;
			processes[p->id].end = timer;
			printf("Process ended\n");
			print(MLFQ, N_q);
		}
		else if((*p).time_in_queue == time_slice[current_queue]) {
			process *x = pop(&MLFQ[current_queue]);
			(*x).time_in_queue = 0;
			if(!isNOPROCESS(x))
			{
				if(current_queue != N_q - 1)
					push(&MLFQ[current_queue + 1], x);
				else
					push(&MLFQ[current_queue], x);
			}
			printf("Queue changed\n");
			print(MLFQ, N_q);
		}
	}
	for(i = 1; i <= N_p; i++) {
		printf("For process %d, turnaround time = %d\n", i, processes[i].end - processes[i].start);
	}
	return 0;
}
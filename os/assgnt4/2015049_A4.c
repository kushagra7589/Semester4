#include <stdio.h>
#include <pthread.h>

#define MAX_SIZE 1000
#define UNIT 100000000
#define DELAY do{ int j; for(j = 0; j < UNIT; j++); } while(0)

pthread_t process_threads[MAX_SIZE];
pthread_mutex_t main_mutex;
pthread_cond_t cond_thread[MAX_SIZE];

int time_slice[20];
int N_q, N_p;

typedef struct process {
	int id;							//process id -> the processes are indexed with this in the array processes[]
	int arrival;					//arrival time of the process
	int burst;						//burst time of the process
	int start;						//time when the process starts executing for the first time
	int end;						//time when the process completes
	int runtime;					//the total runtime of the process irrespective of the queue
	int time_in_queue;				//time spent in the current queue
	int curr_queue;					//the level of  queue in which the process is currently running in
	int active;						//0 or 1 - to show which process is to run now.
	int done;						//0 or 1 - whether process is complete or not 
} process;

process processes[MAX_SIZE];		//an array which stores all processes indexed by their id's

//utility to create a process with given id, arrival time and burst time. All other entries are initialised as -1 or 0.
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
	p.active = 0;
	p.done = 0;
	return p;
}

typedef struct queue {
	int arr[MAX_SIZE];
	int front, rear;
} queue;

queue MLFQ[20];					//the array of queues which forms our multilevel queue system. 0th level has top-most priority


//-----------------operations on queue------------------------------
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

void push(queue *q, int p) {
	q->arr[q->rear] = p;
	q->rear += 1;
}

int TOP(queue *q){ return q->arr[q->front]; }

void pop(queue *q) {
	if(!isEmpty(q)) {
		q->front += 1;
	}
}

//--------------------initialise the timer variable----------------------
int timer = 0;					//the value of this variable determines the current time 


//utility to print the status of queues
void print() {
	printf("-------------------------------------------------------\n");
	printf("At time %d\n", timer);
	for(int i = 0; i < N_q; i++) {
		printf("In queue %d\n", i);
		int j;
		for(j = MLFQ[i].front; j < MLFQ[i].rear; j++) {
			printf("Process : %d -> ", MLFQ[i].arr[j]);
		}
		printf("\n");
	}
	printf("-------------------------------------------------------\n");
	printf("\n");

}

//function passed on to threads
void *runProcess(void *arg) {
	int id = (int)arg;					//process id is passed as parameter.
	while(processes[id].runtime < processes[id].burst) {
		pthread_mutex_lock(&main_mutex);
		while(processes[id].active == 0) {					//wait until signal to activate thread is received
			pthread_cond_wait(&cond_thread[id], &main_mutex);
		}
		if(processes[id].start == -1) processes[id].start = timer;
		int j = 0;

		DELAY;
		timer += 1;
		
		processes[id].time_in_queue += 1;
		processes[id].runtime += 1;

		//-----------------------if the process is complete------------------
		if(processes[id].runtime == processes[id].burst) {
			processes[id].end = timer;
			processes[id].done = 1;
			pop(&MLFQ[processes[id].curr_queue]);
			printf("Process ended\n");
			print();
		}
		//-----------------------if the time slice of current queue is complete--------------------
		else if(processes[id].time_in_queue == time_slice[processes[id].curr_queue]) {
			pop(&MLFQ[processes[id].curr_queue]);
			processes[id].time_in_queue = 0;
			int push_queue = (processes[id].curr_queue == N_q - 1) ? N_q - 1 : processes[id].curr_queue + 1;
			processes[id].curr_queue = push_queue;
			push(&MLFQ[push_queue], id);
			printf("Queue changed\n");
			print();
		}
		//---------------------done working on this process-------------------
		processes[id].active = 0;

		//------------signal main thread to schedule the next process--------------
		pthread_cond_signal(&cond_thread[0]);
		pthread_mutex_unlock(&main_mutex);
	}
	pthread_exit(NULL);
}

//------------------------utility to check if all processes are over------------------
int arrAllDone() {
	int i = 0;
	for(i = 1; i <= N_p; i++) {
		if(processes[i].done == 0) return 0;
	}
	return 1;
}

int main() {
	//----------------input scan-------------------
	scanf("%d %d", &N_q, &N_p);
	int i = 0;
	for(i = 0; i < N_q; i++)
		init_queue(&MLFQ[i]);
	int arrival, burst;
	for(i = 1; i <= N_p; i++) {
		scanf("%d %d", &arrival, &burst);
		process p = createProcess(i, arrival, burst);
		processes[i] = p;
	}
	int boost_time;
	for(i = 0; i < N_q; i++) {
		scanf("%d", &time_slice[i]);
	}
	scanf("%d", &boost_time);

	//--------------------------------initialise the mutex-----------------------------
	pthread_mutex_init(&main_mutex, NULL);
	int next_process = 1;
	int current_queue;
	int k;

	//--------initialise the conditional variables and create threads for each process--------
	for(k = 1; k <= N_p; k++) {
		pthread_cond_init(&cond_thread[k], NULL);
		pthread_create(&process_threads[k], NULL, runProcess, (void *)k);
	}

	//-------------------------Scheduler--------------------------------
	while(!arrAllDone()) {

		/*
		Acquire a lock since following is a critical section.
		Shared memory includes MLFQ[], processes[], timer, time_slice[]
		*/
		pthread_mutex_lock(&main_mutex);

		//----------------------Check the next process to be scheduled and push into the top queue-------------- 
		while(next_process <= N_p && processes[next_process].arrival == timer){
			processes[next_process].curr_queue = 0;
			push(&MLFQ[0], next_process);
			next_process += 1;
			printf("New process added.\n");
			print();
		}

		//-------------------PRIORITY BOOST CONDITION------------------------------------
		if(timer % boost_time == 0) {
			for(i = 1; i < N_q; i++) {
				while(!isEmpty(&MLFQ[i])){
					int id = TOP(&MLFQ[i]);
					pop(&MLFQ[i]);
					processes[id].curr_queue = 0;
					push(&MLFQ[0], id);
				}
			}
			printf("Priority Boost\n");
			print();
		}

		//------------------Get the topmost non-empty queue------------------------------
		for(i = 0; i < N_q; i++) {
			if(!isEmpty(&MLFQ[i])) {
				current_queue = i;
				break;
			}
		}

		//-----------------------If all queues are empty-------------------------------
		if(i == N_q){
			DELAY;
			timer += 1;
		}
		//--------------------Signal the next process to be run------------------------
		else {
			int id = TOP(&MLFQ[current_queue]);
			processes[id].active = 1;							//make the current process active
			pthread_cond_signal(&cond_thread[id]);				//notify the corresponding thread that it's process is active.

			//---------------wait for the current process's thread to complete-----------
			+-----
			while(processes[id].active == 1) {
				pthread_cond_wait(&cond_thread[0], &main_mutex);
			}
		}

		//--------------------Critical section is over---------------------------------
		pthread_mutex_unlock(&main_mutex);
	}

	//-------------------------Display turnaround time for all  processes---------------------
	for(i = 1; i <= N_p; i++) {
		printf("For process %d, turnaround time = %d, end_time = %d\n", i, processes[i].end - processes[i].arrival, processes[i].end);
	}
	return 0;
}
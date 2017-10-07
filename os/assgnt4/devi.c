#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define FILENAME "config_file.txt"
#define MAXBUF 1024 

int take_time=0,final_count=0,b=0,c=0;

struct config
{
   int queue_no;
   int pid;
   int time_slice;
   int boost_period;
   int number_of_processes;
   int arr[20];
};

struct process
{
	int duration;
	int time_of_arrival;
	int turn_time;
	int priority;
	int finish;
};

struct process processes[20];

struct queue
{
	int proc_arr[4000];
};

void schedule(int arr[],int n,int pr,int time,struct queue queues[5])
{
	int m,j;
	pr-=1;
	if(n>1)
	{
		for(j=0;j<time;j++)
		{
			if(processes[arr[j%n]].duration > 0 && n >= 1 && pr == 0)
			{
				queues[pr].proc_arr[take_time+j]=arr[j%n];
				processes[arr[j%n]].duration--;	
			}
			else if(processes[arr[j%n]].duration > 0 && n >= 1)
			{
				if(b==1 && c>0)
				{
					processes[arr[j%n]].duration--;
					c--;
				}
				queues[pr].proc_arr[take_time+j]=arr[j%n];
			}
			else if(processes[arr[j%n]].duration<=0 && n>=1 && pr==0)
			{
				m=j%n;
				processes[m].finish=take_time+j;
				while(m<n-1)
					arr[m]=arr[m++];
				n--;
				if(n==0)
					break;
				queues[pr].proc_arr[take_time+j]=arr[j%n];
				//printf("Queue %d : Process %d",pr,arr[j%n] );
				processes[arr[j%n]].duration--;	
				final_count++;
			}
			else if(n<1 && j<time && pr==0)
			{
				c=time-j;
				b=1;
			}

		}
	}
	else if(n==1)
	{
		for(j=0;j<time;j++)
		{
			if(processes[arr[0]].duration>0)
			{
				if(pr==1)
					processes[arr[0]].duration--;
				queues[pr].proc_arr[take_time+j]=arr[0];
			}
			else
			{
				if(b==1 && c>0)
				{
					processes[arr[0]].duration--;
					c--;
				}
				processes[arr[0]].finish=take_time+j;
				queues[pr].proc_arr[take_time+j]=-1;
			}
		}
	}
	else
	{
		for(j=0;j<time;j++)
			queues[pr].proc_arr[j+take_time]=-1;
	}
}

void thread_main(struct config configstruct,struct queue queues[5])
{
	int n=configstruct.number_of_processes,i=0,arr1[20],arr2[20],arr3[20],arr4[20],arr5[20],t=0,u=0,v=0,w=0,x=0;
	int m=processes[0].time_of_arrival;

	while(final_count<20)
	{
		int t=0;
		if(take_time%configstruct.boost_period!=0)
		{
			if(take_time%configstruct.time_slice==0)
			{
				printf("%d\n",take_time );
				t=0,u=0,v=0,w=0,x=0,b=0,c=0;
				for(i=0;i<20;i++)
				{
					if(processes[i].time_of_arrival <= take_time && processes[i].duration!=0 && processes[i].priority==0)
					{
						arr1[t++]=i;
						processes[i].priority=1;
					}
					else if(processes[i].time_of_arrival < take_time && processes[i].duration!=0 && processes[i].priority>0)
					{
						if(processes[i].priority<5)
							processes[i].priority+=1;
						if(processes[i].priority==2)
							arr2[u++]=i;
						else if(processes[i].priority==3)
							arr3[v++]=i;
						else if(processes[i].priority==4)
							arr4[w++]=i;
						else if(processes[i].priority==5)
							arr5[x++]=i;
						
					}
				}
				schedule(arr1, t, 1,configstruct.time_slice,queues);
				schedule(arr2, u, 2,configstruct.time_slice,queues);
				schedule(arr3, v, 3,configstruct.time_slice,queues);
				schedule(arr4, w, 4,configstruct.time_slice,queues);
				schedule(arr5, x, 5,configstruct.time_slice,queues);
			}
		}
		else
		{
			t=0,u=0,v=0,w=0,x=0;
			for(i=0;i<20;i++)
			{
				if(processes[i].time_of_arrival <= take_time && processes[i].duration!=0 )
				{
					arr1[t++]=i;
					processes[i].priority=1;
				}
			}
			schedule(arr1, t, 1,configstruct.time_slice,queues);
			schedule(arr2, u, 2,configstruct.time_slice,queues);
			schedule(arr3, v, 3,configstruct.time_slice,queues);
			schedule(arr4, w, 4,configstruct.time_slice,queues);
			schedule(arr5, x, 5,configstruct.time_slice,queues);
		}
		take_time+=configstruct.time_slice;
		
	}
}

struct config get_config(char *filename) 
{
    struct config configstruct;
    int num,b,max=-1;
    FILE *file = fopen (filename, "r");
    if (file != NULL)
    { 
        char line[MAXBUF];
        int i = 0;
        while(! feof(file)){
        	fscanf(file, "%d", &configstruct.queue_no);
        	fscanf(file, "%d", &configstruct.time_slice);
        	fscanf(file, "%d", &configstruct.boost_period);
        	fscanf(file, "%d", &configstruct.number_of_processes);
        	int v=configstruct.number_of_processes;
        	for(b=0;b<v;b++)
        	{
        		fscanf(file, "%d", &processes[b].time_of_arrival);
        		fscanf(file, "%d", &processes[b].duration);
        		processes[b].priority=0;
        		configstruct.arr[b]=processes[b].time_of_arrival;
        	}
     	}
        fclose(file);
    }          
    return configstruct;
}

int main()
{
	struct config configstruct;
	int i=0,max;
    configstruct = get_config(FILENAME);
    struct queue queues[5];
    thread_main(configstruct,queues);
    for(i=0;i<20;i++)
    	printf("Process:%d , start time:%d ,finish time:%d \n",i,processes[i].time_of_arrival,processes[i].finish );
    return 0;
}
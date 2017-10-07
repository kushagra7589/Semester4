#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int p;
int q = 0;

int main()
{
	printf("Hello, pid : %d\n", (int) getpid());
	int *r = (int *)malloc(sizeof(int));
	int rc = fork();
	if(rc < 0)
	{
		fprintf(stderr, "%s\n", "fork failed");
		exit(1);
	}
	else if(rc == 0) // this is the child process
	{
		char *myargs[3];
		printf("hello, I am child (pid:%d)\n", (int) getpid());
        // myargs[0] = strdup("sudo");   // program: "wc" (word count)
        myargs[1] = strdup("README"); // argument: file to count
        myargs[0] = strdup("ac");
        myargs[2] = NULL;           // marks end of array
        execvp(myargs[0], myargs);  // runs word count
		printf("This is child process : pid : (%d)\n", (int) getpid());
		// printf("Global declared : %d\nGlobal undeclared  : %d\n local stack: %d\n Heap : %d\n", &q, &p, *r, &r);
	}
	else //this is the parent process 
	{
		printf("This is parent of %d,  pid : %d\n", rc, (int) getpid()); //rc stores the pid of the child process in parent process's execution
		// printf("Global declared : %d\nGlobal undeclared  : %d\n local stack: %d\n Heap : %d\n", &q, &p, *r, &r);
	}
	return 0;
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main()
{
	pid_t pid = fork();
	char *arg[] = {"gcc", "-o", "lst", "2015049_Shell_2.c", NULL};
	if(pid > 0)
	{
		wait(NULL);
	}
	else if(pid == 0)
	{
		execvp(arg[0], arg);
		printf("%s\n", "Yo");
		exit(1);
	}
	pid_t chpid = fork();
	char *array[] = {"gnome-terminal", "-e", "./lst", NULL};
	if(chpid > 0)
	{
		wait(NULL);
	}
	else if(chpid == 0)
	{
		execvp(array[0], array);
		printf("%s\n", "Nahi chala");
		exit(1);
	}
	return 0;
}
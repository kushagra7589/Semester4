#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define LOGFILE ".logs"

void inthandler(int signo){
// 	// printf("\n");
// 	return;		
	if (signo == SIGINT)
	    printf("received SIGINT\n");

	fflush(stdin);
}

typedef struct Command {
	char **argv;
} Command;

char **parse(char *string)
{
	int buff = 82;
	int pos = 0;
	char **arg = malloc(buff * sizeof(char *));
	char *input;

	if (!arg) {
		fprintf(stderr, "Error\n");
		exit(EXIT_FAILURE);
	}

	input = strtok(string, " \t\r\n\a\v\f");
	while (input != NULL) {
		arg[pos++] = input;
		if (pos >= buff) {
			buff += 80;
			arg = realloc(arg, buff * sizeof(char *));
			if (!arg) {
				fprintf(stderr, "Error\n");
				exit(EXIT_FAILURE);
			}
		}

		input = strtok(NULL, " \t\r\n\a\v\f");
	}
	arg[pos] = NULL;
	return arg;
}

int main()
{
	if (signal(SIGINT, inthandler) == SIG_ERR)
		printf("\ncan't catch SIGINT\n");

	char *string = NULL;
	size_t buff = 0;

	//for parsing the input string
	char *comp1, *comp2, **infile, **outfile, *temp, *temp1, *temp2;

	//for indexes of various command
	int pos, tot_cmd;

	//for input output redirection
	int save_in, save_out;

	//open file for reading and writing logs
	FILE *read_history, *write_history;

	while(1)
	{
		infile = NULL;
		outfile = NULL;

		printf("-> ");

		getline(&string, &buff, stdin);

		if(strcmp(string, "\n") == 0)
			continue;

		write_history = fopen(LOGFILE, "a");
		fprintf(write_history, "%s", string);
		fclose(write_history);

		comp1 = strtok(string, ">");
		if(comp1 == NULL)
			continue;

		comp2 = strtok(NULL, ">");
		// printf("%s\n", comp2);
		if(comp2 != NULL)
		{
			temp1 = strtok(comp2, "<");
			temp2 = strtok(NULL, "<");
			if(temp1)
			{
				outfile = parse(temp1);
				if(outfile[0])
					printf("OOO - %s\n", outfile[0]);
				save_out = dup(STDOUT_FILENO);
				close(1);
				open(outfile[0], O_WRONLY | O_CREAT);
			}
			// printf("%s\n", temp2);
			if(temp2)
			{
				infile = parse(temp2);
				// if(infile[0])
				// 	printf("I - %s\n", infile[0]);
				save_in = dup(STDIN_FILENO);
				close(0);
				open(infile[0], O_RDONLY, 0);
			}
		}
		// printf("I = %s, O = %s\n", infile, outfile);


		temp = strtok(comp1, "|");
		Command cmd[10];
		pos = 0;
		while(temp != NULL)
		{
			cmd[pos++].argv = parse(temp);
			temp = strtok(NULL, "|");
		}

		tot_cmd = pos;
		if(strcmp(cmd[0].argv[0], "exit") == 0)
		{
			return 0;
		}
		if(strcmp(cmd[0].argv[0], "history") == 0)
		{
			char c;
			read_history = fopen(LOGFILE, "r");
			while((c = fgetc(read_history)) != EOF)
			{
				printf("%c", c);
			}
			fclose(read_history);
			// write_history = fopen(LOGFILE, "a");
			continue;
		}
		if(strcmp(cmd[0].argv[0], "cd") == 0)
		{
			chdir(cmd[0].argv[1]);
			continue;
		}
		// if(strcmp(cmd[0],argv[0], "help") == 0)
		// {
		// 	printf("%s\n", );
		// }
		int pid;
		pid = fork();
		if(pid == 0)
		{	
			execvp(cmd[0].argv[0], cmd[0].argv);
			exit(EXIT_FAILURE);
		}
		else if(pid < 0)
		{
			printf("%s\n", "Fork failed");
			exit(EXIT_FAILURE);
		}
		else
		{
			dup2(save_in, STDIN_FILENO);
			dup2(save_out, STDOUT_FILENO);
			wait(NULL);
		}
		fflush(stdin);
	}
	return 0;
}
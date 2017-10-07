#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>


void inthandler(int dummy)
{
	// printf("Cntr+C encountered\n");
}

typedef struct Command {
	char **argu;
} Command;

void fork_pipes(Command *cmmd, int n);

void spawn_proc(int in, int out, Command *cmmd);

char ***parse(char *string)
{
	int buffer_size = 80;
	int outstream_size = 20;
	int instream_size = 20;
	char *inp;
	int pos = 0;
	inp = strtok(string, " \t\r\n\a\v\f");
	static char **arg[3];
	arg[0] = malloc(sizeof(char *) * buffer_size);
	arg[1] = malloc(sizeof(char *) * outstream_size);
	arg[2] = malloc(sizeof(char *) * instream_size);
 	while(inp != NULL && strcmp(inp, "<") != 0 && strcmp(inp, ">") != 0)
	{
		arg[0][pos++] = inp;
		if(pos >= buffer_size)
		{
			buffer_size += 80;
			arg[0] = realloc(arg[0], buffer_size * sizeof(char *));
			if(!arg[0]) {
				fprintf(stderr, "%s\n", "Memory out of limit : Could not allocate enough size.");
				exit(EXIT_FAILURE);
			}
		}
		inp = strtok(NULL, " \t\r\n\a\v\f");
	}
	arg[0][pos] = NULL;
	if(inp == NULL)
	{
		arg[1] = NULL;
		arg[2] = NULL;
	}
	else if(strcmp(inp, ">") == 0)
	{
		pos = 0;
		inp = strtok(NULL, " \t\r\n\a\v\f");
		while(inp != NULL && strcmp(inp, "<") != 0)
		{
			arg[1][pos++] = inp;
			if(pos >= outstream_size)
			{
				outstream_size += 20;
				arg[1] = realloc(arg[1], outstream_size * sizeof(char *));
			}
			if(!arg[1]) {
				fprintf(stderr, "%s\n", "Memory out of limit : Could not allocate enough size.");
				exit(EXIT_FAILURE);
			}
			inp = strtok(NULL, " \t\r\n\a\v\f");
		}
		arg[1][pos] = NULL;
		if(inp == NULL)
		{
			arg[2] = NULL;
		}
		else if(strcmp(inp, "<") == 0)
		{
			pos = 0;
			inp = strtok(NULL, " \t\r\n\a\v\f");
			while(inp != NULL)
			{
				arg[2][pos++] = inp;
			if(pos >= instream_size)
			{
				instream_size += 20;
				arg[2] = realloc(arg[2], instream_size * sizeof(char *));
			}
			if(!arg[2]) {
				fprintf(stderr, "%s\n", "Memory out of limit : Could not allocate enough size.");
				exit(EXIT_FAILURE);
			}
			inp = strtok(NULL, " \t\r\n\a\v\f");
			}
			arg[2][pos] = NULL;
		}
	}
	else if(strcmp(inp, "<") == 0)
	{
		pos = 0;
		inp = strtok(NULL, " \t\r\n\a\v\f");
		while(inp != NULL && strcmp(inp, ">") != 0)
		{
			arg[2][pos++] = inp;
			if(pos >= instream_size)
			{
				instream_size += 20;
				arg[2] = realloc(arg[2], instream_size * sizeof(char *));
			}
			if(!arg[2]) {
				fprintf(stderr, "%s\n", "Memory out of limit : Could not allocate enough size.");
				exit(EXIT_FAILURE);
			}
			inp = strtok(NULL, " \t\r\n\a\v\f");
		}
		arg[2][pos] = NULL;
		if(inp == NULL)
		{
			arg[1] = NULL;
		}
		else if(strcmp(inp, ">") == 0)
		{
			pos = 0;
			inp = strtok(NULL, " \t\r\n\a\v\f");
			while(inp != NULL)
			{
				arg[1][pos++] = inp;
				if(pos >= outstream_size)
				{
					outstream_size += 20;
					arg[1] = realloc(arg[1], outstream_size * sizeof(char *));
				}
				if(!arg[1]) {
					fprintf(stderr, "%s\n", "Memory out of limit : Could not allocate enough size.");
					exit(EXIT_FAILURE);
				}
				inp = strtok(NULL, " \t\r\n\a\v\f");
			}
			arg[1][pos] = NULL;
		}
	}
	return arg;
}

char **parse2(char *string)
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

// char **commands[20];

char *make_string(char **command)
{
	size_t total_length = 0;
	int i;
	for(i=0; command[i] != NULL; i++)
	{
		total_length += strlen(command[i]);
	}
	total_length++;
	char *res = (char *)malloc(total_length);

	res[0] = '\0';
	int len;
	for(i=0; command[i] != NULL; i++)
	{
		strcat(res, command[i]);
		strcat(res, " ");
	}
	return res;
}

int main()
{

	signal(SIGINT, inthandler);
	char *cmd = NULL;
	ssize_t buff = 0;
	int save_in, save_out;
	while(1)
	{
		printf("$ ");
		getline(&cmd, &buff, stdin);
		int infile = 0;
		int outfile = 1;
		char ***arg;
		arg = parse(cmd);
		if(arg[0][0] == NULL){
			printf("%s\n", "hello");
			continue;
		}

		if(arg[1] != NULL)
		{
			printf("%s\n", arg[1][0]);
			save_out = dup(STDOUT_FILENO);
			close(1);
			open(arg[1][0], O_WRONLY | O_CREAT);
		}

		if(arg[2] != NULL)
		{
			save_in = dup(STDIN_FILENO);
			close(0);
			open(arg[2][0], O_RDONLY, 0);
		}

		char *tot = make_string(arg[0]);

		int ind = 0;
		char *inp = strtok(tot, "|");
		char **temp = malloc(80 * sizeof(char *));
		while(inp != NULL)
		{
			temp[ind++] = inp;
			inp = strtok(NULL, "|");
		}
		Command *cmmd = (Command *)malloc(sizeof(Command) * ind);
		int i, j;
		for(i=0; i<ind; i++)
		{
			cmmd[i].argu = parse2(temp[i]);
		}
		printf("%s\n", "CMD - > ");
		for(int i=0; i<ind; i++)
			{
				for(int j=0; cmmd[i].argu[j]!=NULL; j++)
				{
					printf("%s||| ", cmmd[i].argu[j]);
				}
				printf("\n");
			}
	
		if(strcmp(cmmd[0].argu[0], "exit") == 0)
		{
			exit(EXIT_SUCCESS);
			return 0;
		}

		if(fork() == 0){
			if(strcpy(cmmd[0].argu[0], "exit") == 0)
			{
				exit(EXIT_SUCCESS);
				return 0;
			}
			fork_pipes(cmmd, ind);
		}
		else if (fork() < 0){
			fprintf(stderr, "%s\n", "Error in fork");
			exit(EXIT_FAILURE);
			return 1;
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

void spawn_proc(int in, int out, Command *cmmd)
{
	pid_t pid;

	pid = fork();
	if(pid == 0)
	{
		if(in != 0)
		{
			dup2(in, 0);
			close(in);
		}

		if(out != 1)
		{
			dup2(out, 1);
			close(1);
		}

		execvp (cmmd->argu[0], cmmd->argu);
		exit(EXIT_FAILURE);
	}
	else if(pid < 0)
	{
		exit(EXIT_FAILURE);
	}
}

void fork_pipes(Command *cmmd, int n)
{
	int i;
	pid_t pid;
	int fd[2];
	int in = 0;
	for(i=0; i < n - 1; i++)
	{
		pipe(fd);

		spawn_proc(in, fd[1], cmmd+i);

		close(fd[1]);

		in = fd[0];
	}
	if(in != 0)
		dup2(in, 0);

	execvp((cmmd + i)->argu[0], (cmmd+i)->argu);
	exit(EXIT_FAILURE);
}
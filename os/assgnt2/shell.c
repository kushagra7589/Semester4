#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int compare(char *a, char *b)
{
	int i = 0;
	if (strlen(a) != strlen(b)) {
		return 1;
	} else {
		while (i < strlen(a)) {
			if (a[i] == b[i])
				i++;
		}
		if (i == strlen(a))
			return 0;
		else
			return 1;
	}
}

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
	char *string = NULL, *d;
	ssize_t buff = 0;
	char **arg;
	while (1) {
		printf("shell> ");
		getline(&string, &buff, stdin);
		arg = parse(string);
		
		if (compare(arg[0], "exit") == 0) {
			return 0;
		}
		if (fork() == 0) {
			if (compare(arg[0], "exit") == 0) {
				return 0;
			}
			if (execvp(arg[0], arg) == -1) {
				printf("Error");
				return 1;
			}
			exit(EXIT_FAILURE);
		} else if (fork() < 0) {
			printf("Error");
			return 1;
		} else {
			wait(NULL);
		}
		fflush(stdin);
	}
	return 0;
}

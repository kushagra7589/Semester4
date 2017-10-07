#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main() {
  int link[2];
  pid_t pid;
  char foo[4096];

  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {

    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    char *myargs[3];
    myargs[0] = strdup("wc"); // program: "wc" (word count)
    myargs[1] = strdup("shell_gui.c"); // argument: file to count
    myargs[2] = NULL; // marks end of array
    execvp(myargs[0], myargs);
    die("execl");

  } else {

    close(link[1]);
    int nbytes = read(link[0], foo, sizeof(foo));
    printf("Output: (%.*s)\n", nbytes, foo);
    wait(NULL);

  }
  return 0;
}
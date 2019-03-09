#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

#define COMMAND_LENGHT 1024
#define PARAMETER_LENGHT 64

char* get_line();
char** parse(char* comm);
int execute(char** params);
void push_to_bg(void);

bool bg = false;

int main(void)
{
    printf("%s", "=== Welcome to basic-shell 0.1.0 ===\n\n");
    chdir(getenv("HOME"));
    char* line;
    char** params;
    while (1)
    {
        printf("[%s@%s] $ ", getenv("USER"), "basic-shell");
	line = get_line();
	params = parse(line);
	int status = execute(params);
    }
    free(line);
    free(params);
    return 0;
}

char* get_line()
{
    char* line = (char*)malloc(sizeof(char) * COMMAND_LENGHT);

    if (!line)
    {
	fprintf(stderr, "failed to allocate space for line input: %s", strerror(errno));
	exit(errno);
    }

    /* Exits the shell either through terminating or Ctrl + D */
    if (fgets(line, sizeof(char) * COMMAND_LENGHT, stdin) == NULL)
    {
	exit(0);
    }

    /* Removes newline char if there is one */
    if (line[strlen(line) - 1] == '\n')
    {
	line[strlen(line) - 1] = '\0';
    }

    return line;
}

char** parse(char* comm)
{
    char** params = (char**)malloc(sizeof(char) * PARAMETER_LENGHT);

    if (!params)
    {
	fprintf(stderr, "failed to allocate space for parameters: %s", strerror(errno));
	exit(errno);
    }

    /* Ensures to pass through all of the params */
    for (int i = 0; i < COMMAND_LENGHT; i++)
    {
	char* temp_param = strsep(&comm, " ");

	if (temp_param == NULL)
	{
	    break;
	}
	/* Checks if the process is to be run in the background */
	if (*temp_param == '&')
	{
	    bg = true;
	}
	else
	{
	    params[i] = temp_param;
	}
    }
    return params;
}

int execute(char** params)
{
    /* Do not execute if the passed command is an empty character */
    if (*params[0] == '\0')
    {
	return 0;
    }

    if (getppid() == 1)
    {
	/* Already a daemon */
	return 0;
    }

    pid_t pid = fork();
    if (pid < 0)
    {
	/* fork error */
        fprintf(stderr, "fork error: %s\n", strerror(errno));
	return 1;
    }
    else if (pid == 0)
    {
	/* child created */
        execvp(params[0], params);
        fprintf(stderr, "basic-shell: %s: %s\n", params[0], strerror(errno));
	return 1;
    }

    if (bg)
    {
	push_to_bg();
	return 0;
    }
    int childStatus;
    waitpid(pid, &childStatus, 0);
    return 0;
}

void push_to_bg(void)
{
    bg = false;

    setsid(); /* Obtain a new process group */

    int i = open("/dev/null",O_RDWR); /* Handle standart I/O */
    dup(i);
    dup(i);

    signal(SIGCHLD,SIG_IGN); /* Ignore child */
    signal(SIGTSTP,SIG_IGN); /* Ignore tty signals */
}

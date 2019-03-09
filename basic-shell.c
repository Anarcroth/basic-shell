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
int exec_seq(char** params);
int exec_daemon(char** params);

bool bg = false;

int main(int argc, char* argv[])
{
    char* line;
    char** params;
    while (1)
    {
        printf("[%s@%s] ~$ ", getenv("USER"), "basic-shell");
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
	fprintf(stderr, "Failed to allocate space for line input");
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
	fprintf(stderr, "Failed to allocate space for parameters");
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

    if (bg)
    {
	return exec_daemon(params);
    }
    return exec_seq(params);
}

int exec_seq(char** params)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        printf("fork: %s\n", strerror(errno));
	return 1;
    }
    else if (pid == 0)
    {
        execvp(params[0], params);
        printf("shell: %s: %s\n", params[0], strerror(errno));
	return 1;
    }
    else
    {
        int childStatus;
        waitpid(pid, &childStatus, 0);
	return 0;
    }
}

int exec_daemon(char** params)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "fork: %s\n", strerror(errno));
	return 1;
    }
    else if (pid == 0)
    {
        execvp(params[0], params);
        printf("basic-shell: %s: %s\n", params[0], strerror(errno));
	return 1;
    }
    setsid();
}

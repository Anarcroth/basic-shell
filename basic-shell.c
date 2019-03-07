#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define COMMAND_LENGHT 1024
#define PARAMETER_LENGHT 64

char* get_line();
char** parse(char* comm);
int execute(char** params);

int main(int argc, char* argv[])
{
    char* line;
    char** params;
    while (1)
    {
        printf("[%s@basic-shell] ~$ ", getenv("USER"));
	line = get_line();
	params = parse(line);
	int status = execute(params);
	if (status == 1)
	{
	    return 1;
	}
    }
    free(line);
    free(params);
    return 0;
}

char* get_line()
{
    char* line = (char*)malloc(sizeof(char) * COMMAND_LENGHT);

    if (!line) exit(0);

    if (fgets(line, sizeof(char) * COMMAND_LENGHT, stdin) == NULL) exit(0);

    if (line[strlen(line) - 1] == '\n')
	line[strlen(line) - 1] = '\0';

    return line;
}

char** parse(char* comm)
{
    char** params = (char**)malloc(sizeof(char) * PARAMETER_LENGHT);
    // ensures to pass through all of the params
    for (int i = 0; i < COMMAND_LENGHT; i++)
    {
	params[i] = strsep(&comm, " ");
	if (params[i] == NULL) break;
    }
    return params;
}

int execute(char** params)
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

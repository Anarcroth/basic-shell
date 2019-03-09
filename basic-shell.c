#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LENGHT 1024
#define PARAMETER_LENGHT 64
#define BUILTIN_COMMS 2

char *get_line(void);
char **parse(char *comm);
void execute(char **params);
bool is_builtin(char *comm);
void exec_builtin(char **params);
void exec_std(char **params);
void cd(char **params);
void push_to_bg(void);

bool bg = false;

char *builtin_commands[] = { "cd", "exit" };

int main(void)
{
	printf("%s", "=== Welcome to basic-shell 0.1.0 ===\n\n");
	chdir(getenv("HOME"));
	char *line;
	char **params;

	while (1) {
		printf("[%s@%s] $ ", getenv("USER"), "basic-shell");
		line = get_line();
		params = parse(line);
		execute(params);
	}

	free(line);
	free(params);
	return 0;
}

char *get_line(void)
{
	char *line = (char*)malloc(sizeof(char) * COMMAND_LENGHT);

	if (!line) {
		fprintf(stderr, "failed input allocation: %s", strerror(errno));
		exit(errno);
	}

	/* Exits the shell either through terminating or Ctrl + D */
	if (fgets(line, sizeof(char) * COMMAND_LENGHT, stdin) == NULL)
		exit(0);

	if (line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';

	return line;
}

char **parse(char *comm)
{
	char **params = (char**)malloc(sizeof(char) * PARAMETER_LENGHT);

	if (!params) {
		fprintf(stderr, "failed parameters allocation: %s", strerror(errno));
		exit(errno);
	}

	for (int i = 0; i < COMMAND_LENGHT; i++) {
		char *tmp = strsep(&comm, " ");

		if (tmp == NULL)
			break;

		if (*tmp == '&')
			bg = true;
		else
			params[i] = tmp;
	}
	return params;
}

void execute(char **params)
{
	if (*params[0] == '\0') /* Empty command */
		return;

	if (getppid() == 1)     /* Already a daemon */
		return;

	if (is_builtin(params[0]))
		exec_builtin(params);
	else
		exec_std(params);
}

bool is_builtin(char *comm)
{
	for (int i = 0; i < BUILTIN_COMMS; i++)
		if (strcmp(builtin_commands[i], comm) == 0)
			return true;

	return false;
}

void exec_builtin(char **params)
{
	if (strcmp(params[0], "cd") == 0)
		cd(params);
	else if (strcmp(params[0], "exit") == 0)
		exit(0);
}

void exec_std(char **params)
{
	pid_t pid = fork();
	if (pid < 0) {
		fprintf(stderr, "fork error: %s\n", strerror(errno));
		return;
	}
	else if (pid == 0) {
		execvp(params[0], params);
		fprintf(stderr, "%s: %s\n", params[0], strerror(errno));
		return;
	}

	if (bg) {
		push_to_bg();
		return;
	}
	else {
		int childStatus;
		waitpid(pid, &childStatus, 0);
		return;
	}
}

void cd(char **params)
{
	if (params[1] == NULL || chdir(params[1]) != 0)
		fprintf(stderr, "%s: %s\n", params[0], strerror(errno));
}

void push_to_bg(void)
{
	bg = false;

	setsid(); /* Obtain a new process group */

	int i = open("/dev/null", O_RDWR); /* Handle standart I/O */
	dup(i);
	dup(i);

	signal(SIGCHLD, SIG_IGN); /* Ignore child */
	signal(SIGTSTP, SIG_IGN); /* Ignore tty signals */
}

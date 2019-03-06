#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_LENGHT 100
#define PARAMETER_LENGHT 10

void get_command(char* comm);
void parse(char* comm, char** params);
void execute(char* comm, char** params);

int main(int argc, char* argv[])
{
    char comm[COMMAND_LENGHT + 1];
    char* params[PARAMETER_LENGHT + 1];
    while (1)
    {
        printf("[%s@basic-shell] ~$ ", getenv("USER"));
	get_command(comm);
	parse(comm, params);
	execute(comm, params);
	printf("%s\n", comm);
    }
    return 0;
}

void get_command(char* comm)
{
    if (fgets(comm, sizeof(comm), stdin) == NULL) comm = NULL;
    // Remove trailing neommand character
    if (comm[strlen(comm) - 1] == '\n') {
	comm[strlen(comm) - 1] = '\0';
    }
}

void parse(char* comm, char** params)
{
    for (int i = 0; i < PARAMETER_LENGHT; i++)
    {
	params[i] = strsep(&comm, " ");
	if (params[i] == NULL) break;
    }
}

void execute(char* comm, char** params)
{

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND_LENGHT 100
#define PARAMETER_LENGHT 10

char* get_command(char* line);

int main(int argc, char* argv[])
{
    char line[100];
    while (1)
    {
        printf("[%s@basic-shell] ~$ ", getenv("USER"));
	get_command(line);
	printf("%s\n", line);
    }
    return 0;
}

char* get_command(char* line)
{
    if (fgets(line, sizeof(line), stdin) == NULL) return "";
    // Remove trailing neommand character
    if (line[strlen(line) - 1] == '\n') {
	line[strlen(line) - 1] = '\0';
    }
    return line;
}

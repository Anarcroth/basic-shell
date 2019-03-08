# Basic-shell

## General

This is a bare bones shell, written in C, with the ability to both execute foreground and background processes. The shell initializes very little resources and is capable of handling only one command with its argument list. If specified with the `&` symbol, the command will respectively be executed in the background.

## Implementation

The general program loop goes like this:

``` pseudocode
while TRUE:
	get()
	parse()
	evaluate()
	check_status()
```

Although this is a very basic approach, it yields great results!

Looking at the code closer, we can see that there are two main data-structures that are used.

``` c
char* line;
char** params;
```

The `line` variable is used in order to capture the users input. This would represent the whole line of the input, together with all of the additional arguments in the form of a single string, hence the `char *`. The `params` variable is representative of the actual command with any additional arguments passed to it. It is a 2D array, containing strings.

In order to get the input line and parse it, we use the `char* get_line()` function.

``` c
char* get_line()
{
    char* line = (char*)malloc(sizeof(char) * COMMAND_LENGHT);

    if (!line) exit(0);

    if (fgets(line, sizeof(char) * COMMAND_LENGHT, stdin) == NULL) exit(0);

    if (line[strlen(line) - 1] == '\n')
	line[strlen(line) - 1] = '\0';

    return line;
}
```

First, we allocate space for the whole line, where the constant `COMMAND_LENGHT` is defined like so: `#define COMMAND_LENGHT 1024`. Since `malloc` can fail, we do a check for that on the first `if` statement. Then we use the builtin `fgets` function from the `string.h` header file, in order to get any user input. This builtin function saves a lot of manual work for us, but we do have to make sure that we end the input of the user with a terminating character. That is what we do with the last `if`. We check if the last character of the `line` array is a new line. If it is, we just replace it and return the whole line for further processing.

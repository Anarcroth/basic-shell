#include <stdio.h>

int main()
{
    char* input;
    while (1)
    {
	printf("%s", "[basic-shell@root] ~$ ");
	scanf("%s", input);
    }
    return 0;
}

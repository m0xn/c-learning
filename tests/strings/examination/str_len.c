#include <stdio.h>
#include <string.h>

int main()
{
	const char *msg = "This is another test message";
	printf("The lenght of the message \"%s\" is of %zu characters\n", msg, strlen(msg));
	return 0;
}

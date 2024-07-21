#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char msg[] = "This message has been duplicated.";
	char *buffer = strdup(msg);

	printf("\"%s\"\n", buffer);
	free(buffer);
	return 0;
}

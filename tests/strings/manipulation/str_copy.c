#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 64

int main()
{
	char msg[] = "This is my message";
	char *buffer = (char*)calloc(BUFF_SIZE, sizeof(char));
	strcpy(buffer, msg);
	printf("The original message \"%s\" was copied into `buffer` resulting in: %s\n", msg, buffer);

	free(buffer);
	return 0;
}

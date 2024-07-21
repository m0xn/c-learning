#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 64

int main()
{
	char *buff1 = (char*)calloc(BUFF_SIZE, sizeof(char));
	char *buff2 = (char*)calloc(BUFF_SIZE, sizeof(char));

	strcpy(buff1, "Here is a message to copy into the buffer.");

	memcpy(buff2, buff1, strlen(buff1));
	printf("The copied content of the second buffer is: %s\n", buff2);

	free(buff1);
	free(buff2);
	return 0;
}

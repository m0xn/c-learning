#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 64

int main()
{
	char *buff1 = (char*)calloc(BUFF_SIZE, sizeof(char));
	char *buff2 = (char*)calloc(BUFF_SIZE, sizeof(char));

	strcpy(buff1, "This message has been passed to the second buffer!");
	memmove(buff2, buff1, strlen(buff1));

	printf("The following message has been moved to `buff2`: \"%s\"\n", buff2);
	return 0;
}

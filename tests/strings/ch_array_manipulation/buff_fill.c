#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_CAP 32

int main()
{
	char *buffer = (char*)calloc(BUFF_CAP, sizeof(char));
	char ch = 'o';
	memset(buffer, ch, BUFF_CAP);
	printf("The resulting buffer is: %s\n", buffer);
	return 0;
}

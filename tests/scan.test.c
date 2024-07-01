#include <stdio.h>

int main()
{
	char str[64];
	char *ptr = str;
	printf("> ");
	scanf("%s", ptr);
	printf("%s\n", str);
}

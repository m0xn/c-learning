#include <stdio.h>
#include <string.h>

int main()
{
	const char *original = "Here something";
	const char *substring = "something";

	char *occurence = strstr(original, substring);
	printf("First substring occurence is \"%s\"\n", occurence);
	return 0;
}

#include <stdio.h>
#include <string.h>

int main()
{
	char str1[64] = "Here is something";
	char str2[] = "I want to concatenate";

	strcat(str1, " ");
	strcat(str1, str2);

	printf("%s\n", str1);
}

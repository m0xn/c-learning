#include <stdio.h>
#include <string.h>

int main()
{
	char list[] = "eggs, banana, chicken, milk";
	const char *delim = ", ";
	char *token = strtok(list, delim);

	do {
		printf("Following token found: %s\n", token);
		token = strtok(NULL, delim); // Subsequent call to the method, continues parsing the string
	} while(token);
	return 0;
}

#include <stdio.h>
#include <string.h>

int main()
{
	char *directions = "10->20<-30->40->50->->20";
	const char *br_set = "-><-";
	int instruction_count = 0;

	printf("Parsing: %s...\n", directions);
	
	do {
		directions = strpbrk(directions, br_set);
		if (directions)
			directions += strspn(directions, br_set);
		instruction_count++;
	} while (directions);

	printf("Found %d instructions.\n", instruction_count);

	return 0;
}

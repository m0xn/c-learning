#include <stdio.h>

/* Write to a file given an input */
int main()
{
	char *name;
	const char *fname = "name.txt";
	FILE *fptr = fopen(fname, "w");

	printf("Introduce un nombre: ");
	scanf("%s", name);

	fprintf(fptr, "%s", name);
	fclose(fptr);

	return 0;
}

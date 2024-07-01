#include <stdio.h>

int main()
{
	char *name;
	const char *fname = "test.txt";
	FILE *fptr = fopen(fname, "r");

	if (fptr == NULL) {
		printf("No se ha encontrado el archivo \"%s\"\n", fname);
		return 1;
	}

	fscanf(fptr, "%s", name);
	printf("El nombre extraído es: %s\n", name);
	fclose(fptr);
	
	return 0;
}

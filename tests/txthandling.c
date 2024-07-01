#include <stdio.h>

int main()
{
	char *s;
	const char *text = "Estamos aprendiendo C.\n";
	const char *nexttext = "Hemos sobreescrito el archivo\n";
	const char *fname = "text.txt";
	FILE *fptr = fopen(fname, "r+");

	if (fptr == NULL) {
		printf("No se pudo abrir el archivo \"%s\"", fname);
		return 1;
	}

	printf("%s\n", s);

	return 0;
}

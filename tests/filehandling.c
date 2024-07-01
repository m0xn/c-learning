#include <stdio.h>

int main()
{
	float n;
	float PI = 3.14;
	const char *fname = "floats.bin";
	FILE *fptr = fopen(fname, "wb+");
	
	if (fptr == NULL) {
		printf("El archivo \"%s\"", fname);
		return 1;
	}

	for (int i = 0; i < 5; ++i) {
		n = PI + (float) i;
		fwrite(&n, sizeof(float), 1, fptr);
		printf("Valor de n: %.2f\tPosición: %d\n", n, ftell(fptr));
	}

	rewind(fptr);
	printf("------------------------\n");

	while (fread(&n, sizeof(float), 1, fptr) != 0)
		printf("Valor de n: %.2f\tPosición: %d\n", n, ftell(fptr));

	fclose(fptr);

	return 0;
}

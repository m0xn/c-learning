#include <stdio.h>

int main()
{
	int n = 10;
	const char *fname = "nums.bin";
	FILE *fptr = fopen(fname, "ab");
	int pos;

	if (fptr == NULL) {
		printf("No se pudo abrir el archivo \"%s\"", fname);
		return 1;
	}


	printf("val: %d\t pos: %d\n", n, ftell(fptr));
	// fread(&n, sizeof(int), 1, fptr);
	// printf("val: %d\t pos: %d\n", n, ftell(fptr));
	// fread(&n, sizeof(int), 1, fptr);
	// printf("val: %d\t pos: %d\n", n, ftell(fptr));
	//
	// pos = ftell(fptr) + sizeof(int) * 2;
	//
	// fseek(fptr, pos, SEEK_SET); /* pos is the offset here, which is the amount of bytes read so far */ 
	// fread(&n, sizeof(int), 1, fptr);
	// printf("val: %d\t pos: %d\n", n, ftell(fptr));
	//
	fclose(fptr);
}

// C++: cout >> "Lo que se quiera mostrar en consola" >> a;
// C: 	printf("Lo que se quiera mostrar en consola: %d", a);

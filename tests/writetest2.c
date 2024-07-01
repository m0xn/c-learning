#include <stdio.h>
#include <stdarg.h>

#define CAP 5

int file_read(const char *restrict filename, const char *restrict format, int* arr);

int main()
{
	const char *fname = "num.txt";
	int num_arr[CAP];
	FILE *wptr = fopen("num.txt", "w");

	fprintf(wptr, "%d ", 42);
	fprintf(wptr, "%d ", 30);
	fprintf(wptr, "%d ", 25);
	fprintf(wptr, "%d ", 70);
	fprintf(wptr, "%d ", 80);
	fclose(wptr);
	
	file_read(fname, "%d", num_arr);

	for (int i = 0; i < CAP; ++i)
		printf("%d\n", num_arr[i]);

	return 0;
}

int file_read(const char *restrict filename, const char *restrict format, int* arr)
{
	FILE *fptr = fopen(filename, "r");

	if (fptr == NULL) {
		printf("No se ha podido abrir el archivo \"%s\"", filename);
		return -1;
	}

	for (int i = 0; i < CAP; ++i) {
		fscanf(fptr, format, &arr[i]);
	}
}

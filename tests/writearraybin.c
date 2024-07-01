#include <stdio.h>

#define CAP 5

int main() {
	int nums[CAP] = { 2, 8, 3, 7, 5 };
	int n;

	const char *fname = "arraytest.txt";
	FILE *fptr = fopen(fname, "wb+");

	if (fptr == NULL) {
		printf("No se ha podido abrir el archivo: %s", fname);
		return 1;
	}

	for (int i = 0; i < CAP; ++i) {
		fprintf(fptr, "%d", nums[i]);
	}
	// fwrite(nums, sizeof(int) * CAP, CAP, fptr);
	rewind(fptr);

	for (int i = 0; i < CAP; ++i) {
		// fread(&n, sizeof(int), 1, fptr);
		fscanf(ftpr, "%d", &n);
		printf("Valor de n:%d\tPos: %d\n", n, ftell(fptr));
	}

	fclose(fptr);

	return 0;
}

#include <stdio.h>

int main()
{
	char fname[32], newname[32];
	printf("Introduce el nombre del archivo que quieras cambiar de nombre: ");
	scanf("%s", fname);

	FILE *fptr = fopen(fname, "r");

	if (fptr == NULL) {
		printf("No existe el archivo\n");
		return 1;
	}
	
	printf("Introduce el nuevo nombre para el archivo: ");
	scanf("%s", newname);

	rename(fname, newname);
	return 0;
}

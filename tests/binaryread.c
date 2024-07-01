#include <stdio.h>

typedef struct Person {
	char name[32];
	int age;
	float salary;
} Person;

int main()
{
	Person ps[5];
	Person p;

	const char *fname = "person.bin";
	FILE *fptr = fopen(fname, "rb");

	if (fptr == NULL) {
		printf("El archivo \"%s\" no existe\n", fname);
		return 1;
	}

	for (int i = 0; i < 5; ++i) {
		fread(&p, sizeof(Person), 1, fptr);
		printf("name: %s\tage: %d\tsalary: %.2f\n", p.name, p.age, p.salary);
	}
	fclose(fptr);

	return 0;
}

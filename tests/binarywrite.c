#include <stdio.h>

typedef struct Person {
	char name[32];
	int age;
	float salary;
} Person;

int main()
{
	Person ps[5] = {
		{
			.name = "Robert",
			.age = 23,
			.salary = 1500.0
		},
		{
			.name = "Juan",
			.age = 30,
			.salary = 2000.0
		},
		{
			.name = "Carol",
			.age = 22,
			.salary = 1200.0
		},
		{
			.name = "Carlos",
			.age = 25,
			.salary = 1100.0
		},
		{
			.name = "Michelle",
			.age = 25,
			.salary = 2300.0
		}
	};

	const char *fname = "person.bin";
	FILE *fptr = fopen(fname, "wb");
	
	fwrite(&ps, sizeof(Person), 5, fptr);
	printf("Se ha escrito al archivo \"%s\" exitosamente.\n", fname);
	fclose(fptr);

	return 0;
}

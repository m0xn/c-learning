#include <stdio.h>

typedef struct CompressedPerson {
	char *name;
	int age;
	float salary;
} CompressedPerson;

typedef struct Person {
	char name[32];
	int age;
	float salary;
} Person;

int main()
{
	Person p = {
		.name = "lkasdflkjasdflkjasdflkasdf",
		.age = 20,
		.salary = 1000
	}; /* 34 bytes */

	CompressedPerson pzip = {
		.name = "lkjasdflkjasdflkljasdfkjasdflkjasdflkj",
		.age = 20,
		.salary = 1000
	}; /* 46 bytes */

	printf("%d, %d\n", sizeof(CompressedPerson), sizeof(Person));
	printf("%d, %d\n", sizeof(pzip.name), sizeof(p.name));
}

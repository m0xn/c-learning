#include <stdio.h>
#include <string.h>

typedef int s;

typedef struct Point {
	int x;
	int y;
} Point;

union uPerson {
	char name[16];
	int age;
	float salary;
} up;

struct sPerson {
	char name[16];
	int age;
	float salary;
} sp;

void print_coordinates(Point pt) {
	printf("(%d, %d)\n", pt.x, pt.y);
}

void mod_coordinates(Point *pt, int x, int y) {
	pt->x += x;
	pt->y += y;
}

int main()
{
	// Point pt = {10, 20};
	//
	// printf("%d, %d", sizeof(up), sizeof(sp));

	s x = 10;
	printf("%d\n", x);

	// printf("name: %s\tage: %d\tsalary: %.2f\n", p.name, p.age, p.salary);
	// strcpy(p.name, "Juan");
	// printf("name: %s\tage: %d\tsalary: %.2f\n", p.name, p.age, p.salary);
	//
	// p.age = 20;
	// printf("name: %s\tage: %d\tsalary: %.2f\n", p.name, p.age, p.salary);
	//
	// p.salary = 2500.99;
	// printf("name: %s\tage: %d\tsalary: %.2f\n", p.name, p.age, p.salary);

	// print_coordinates(pt);
	// mod_coordinates(&pt, 10, -20);
	// print_coordinates(pt);
}

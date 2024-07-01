#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct RobotProperties {
	int agility; /* Un valor del 0 al 5 para marcar su habilidad */ 
	int vision;
} RobotProperties;

typedef struct Robot {
	char name[32];
	int serialnum;
	float price;
	RobotProperties abilities;
} Robot;

int main() {
	Robot rb;
	RobotProperties rb_props;

	rb_props.agility = 4;
	rb_props.vision = 2;

	const char *robotname = "Robert";
	strcpy(rb.name, robotname);
	rb.serialnum = 123;
	rb.price = 2340.99;
	rb.abilities = rb_props;

	printf("El nombre del robot es: %s\nEl número de serie es: %d\nEl precio es:%.2f\n"
		   "Sus abilidades son: agilidad: %d\tvision: %d\n", 
			rb.name, 
			rb.serialnum, 
			rb.price,
			rb.abilities.agility, 
			rb.abilities.vision);
}

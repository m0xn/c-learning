#include <stdio.h>
#include "bar.h"
#include "foo.h"

int main()
{
	Point p = {
		.x = 10,
		.y = 20
	};

	printf("x:%d, y:%d\n", p.x, p.y);
}

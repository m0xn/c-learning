#include <stdio.h>

int main()
{
	int sets[3][2][2] = {{{1, 2}, {3, 2}}, {{2, 3}, {5, 2}}, {{3, 1}, {5, 7}}};
	int (*setptr)[2][2] = sets;
	int (*setptrptr)[2] = *setptr;
	int *setptrptrptr = *setptrptr;

	for (int i = 0; i < 3; ++i) {
		setptr++;
		for (int j = 0; j < 2; ++j) {
			setptrptr++;
			for (int k = 0; k < 2; ++k) {
				printf("%d\n", *setptrptrptr++);
			}
		}
	}
}

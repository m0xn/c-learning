#include <stdio.h>
#include <stdint.h>

typedef enum : uint8_t {
	BOLD = 1,
	ITALIC = 2,
	HIGHLIGHT = 4,
	UNDERLINE = 8,
	STRIKETHROUGH = 16
} Style;

void printBinary(uint8_t num) { int i; for (i = 5 - 1; i >= 0; i--) { printf("%d", (num & (1u << i)) ? 1 : 0); } }

int main()
{
	Style s = UNDERLINE;
	printf("%d\n", s);
	printBinary(s);
	printf("\n");

	printf("%d\n", s >> 2);
	printBinary(s >> 2);
	printf("\n");
}

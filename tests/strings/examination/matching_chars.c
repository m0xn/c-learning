#include <stdio.h>
#include <string.h>

int main()
{
	const char *number_set = "1234567890";
	const char *symbol_set = "~%)#@=";
	const char *sample_set = "2134125123512354123~#)%";

	size_t max_matching_length = strspn(sample_set, number_set);
	size_t max_non_matching_length = strcspn(sample_set, symbol_set);
	printf("Skipping numeric chars by getting the max length of matching numeric"
			"characters from %s in %s to get: %s\n", number_set, sample_set, sample_set + max_matching_length);
	printf("Skipping numeric chars by getting the max length of non-matching"
			"symbol characters from %s in %s to get: %s\n", symbol_set, sample_set, sample_set + max_non_matching_length);
	return 0;
}

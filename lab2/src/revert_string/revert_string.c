#include "revert_string.h"

void RevertString(char *str)
{
	size_t n = strlen(str);
	char* new_str = (char*)malloc(n + 1);
	for (size_t i = 0; i < n; ++i) {
		new_str[i] = str[n - i - 1];
	}
	strcpy(str, new_str);
	free(new_str);
}

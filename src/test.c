#include "vec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, const char **argv)
{
	struct vec vec;
	int *num, i;

	vec_init(&vec, sizeof(int), 10);

	for (i = 1; i < argc; i++)
		VEC_PUSH(&vec, int, atoi(argv[i]));

	for (i = 0; i < vec_len(&vec); i++)
		printf("%i\n", VEC_AT(&vec, int, i));

	vec_deinit(&vec);
}

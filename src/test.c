#include "vec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, const char **argv)
{
	struct vec vec;
	int i, *val;

	vec_init(&vec, sizeof(int), 10);

	for (i = 1; i < argc; i++) {
		val = vec_alloc_slot(&vec);
		*val = atoi(argv[i]);
	}

	for (i = 0; i < vec.len; i++)
		printf("%i\n", *(int *)vec_at(&vec, i));

	printf("vec len: %lu\n", vec.len);

	vec_deinit(&vec);
}

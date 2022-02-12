#include "vec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, const char **argv)
{
	struct vec *vec;
	int *num;
	int i, val;

	vec = vec_alloc(sizeof(int), 10);
	
	for (i = 1; i < argc; i++) {
		val = atoi(argv[i]);
		vec_push(vec, &val);
	}

	num = NULL;
	while (vec_iter(vec, &num)) {
		printf("%i\n", *num);
	}

	vec_free(vec);
}

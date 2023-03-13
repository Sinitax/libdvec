#include "vec.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIBVEC_ERR() errx(1, "test: libvec: %s", strerror(libvec_errno))

int
main(int argc, const char **argv)
{
	struct vec vec;
	int i, ret;
	int *val;

	ret = vec_init(&vec, sizeof(int), 10);
	if (ret) LIBVEC_ERR();

	for (i = 1; i < argc; i++) {
		val = vec_alloc_slot(&vec);
		if (!val) LIBVEC_ERR();
		*val = atoi(argv[i]);
	}

	for (i = 0; i < vec.len; i++)
		printf("%i\n", *(int *)vec_at(&vec, i));

	printf("vec len: %lu\n", vec.len);

	vec_deinit(&vec);
}

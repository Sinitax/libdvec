#include "vec.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIBVEC_ERR(rc) errx(1, "libvec: %s", strerror(rc))

int
main(int argc, const char **argv)
{
	struct vec vec;
	int i, rc;
	int *val;

	rc = vec_init(&vec, sizeof(int), 10);
	if (rc) LIBVEC_ERR(rc);

	for (i = 1; i < argc; i++) {
		rc = vec_alloc_slot(&vec, (void **)&val);
		if (rc) LIBVEC_ERR(rc);
		*val = atoi(argv[i]);
	}

	for (i = 0; i < vec.len; i++)
		printf("%i\n", *(int *)vec_at(&vec, i));

	printf("vec len: %lu\n", vec.len);

	vec_deinit(&vec);
}

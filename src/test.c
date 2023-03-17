#include "allocator.h"
#include "dvec.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIBDVEC_ERR(rc) errx(1, "libdvec: %s", rc < 0 ? strerror(-rc) : "???")

int
main(int argc, const char **argv)
{
	struct dvec dvec;
	int i, rc;
	int *val;

	rc = dvec_init(&dvec, sizeof(int), 10, &stdlib_heap_allocator);
	if (rc) LIBDVEC_ERR(rc);

	for (i = 1; i < argc; i++) {
		val = dvec_add_slot(&dvec, &rc);
		if (rc) LIBDVEC_ERR(rc);
		*val = atoi(argv[i]);
	}

	for (i = 0; i < dvec.len; i++)
		printf("%i\n", *(int *)dvec_at(&dvec, i));

	printf("dvec len: %lu\n", dvec.len);

	dvec_deinit(&dvec);
}

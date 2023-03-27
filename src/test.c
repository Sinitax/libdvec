#include "allocator.h"
#include "dvec.h"

#include <err.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LIBDVEC_ERR(rc) errx(1, "libdvec: %s", \
	rc < 0 ? strerror(-rc) : dvec_err[rc])

static const char *dvec_err[] = {
	DVEC_STRERR_INIT
};

int
main(int argc, const char **argv)
{
	struct dvec dvec;
	int i, rc;
	int *val;

	rc = dvec_init(&dvec, sizeof(int), 16, &stdlib_heap_allocator);
	if (rc) LIBDVEC_ERR(rc);

	for (i = 1; i < argc; i++) {
		rc = dvec_add_back(&dvec, 1);
		if (rc) LIBDVEC_ERR(rc);
		val = dvec_back(&dvec);
		*val = atoi(argv[i]);
	}

	for (DVEC_ITER(&dvec, val))
		printf("%i\n", *val);

	printf("dvec len: %lu\n", dvec.len);

	dvec_deinit(&dvec);
}

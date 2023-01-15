#include "vec.h"
#include "bitvec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, const char **argv)
{
	struct bitvec bitvec;
	struct vec vec;
	int i, *val;

	vec_init(&vec, sizeof(int), 10);
	bitvec_init(&bitvec, 10);

	for (i = 1; i < argc; i++) {
		val = vec_alloc_slot(&vec);
		*val = atoi(argv[i]);
		if (bitvec_get(&bitvec, *val))
			printf("%i -> dup!\n", *val);
		bitvec_reserve(&bitvec, *val);
		bitvec_set(&bitvec, *val);
	}

	for (i = 0; i < 10; i++)
		printf("%i", bitvec_get(&bitvec, i));
	printf("\n");

	for (i = 0; i < vec.len; i++)
		printf("%i\n", *(int *)vec_at(&vec, i));

	printf("vec len: %lu\n", vec.len);
	printf("bitvec len: %lu\n", bitvec.cap / (8 * sizeof(libvec_bitslot_t)));

	vec_deinit(&vec);
}

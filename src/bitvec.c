#include "bitvec.h"

#include <string.h>

#define SLOT_BYTES sizeof(libvec_bitslot_t)
#define SLOT_BITS (SLOT_BYTES * 8)
#define SLOT_MAX (~(libvec_bitslot_t)0)

#define CEILDIV(n, d) ((n) / (d) + ((n) % (d) == 0 ? 0 : 1))
#define BITCEIL(n) ((n) + SLOT_BITS - 1 - (SLOT_BITS - 1 + n) % SLOT_BITS)

#define SLOT(n) ((n) / SLOT_BITS)
#define SLOTCNT(n) CEILDIV(n, SLOT_BITS)
#define SLOT_BIT(n) (((libvec_bitslot_t)1) << n)

#define APPLY_MASK(x,s,m) ((s) ? ((x) | (m)) : ((x) & ~(m)))

bool
bitvec_init(struct bitvec *vec, size_t cap)
{
	LIBVEC_ASSERT(vec != NULL);

	if (cap) {
		vec->data = calloc(SLOTCNT(cap), SLOT_BYTES);
		if (!vec->data) {
			LIBVEC_HANDLE_ERR("calloc");
			return false;
		}
	} else {
		vec->data = NULL;
	}

	vec->cap = BITCEIL(cap);

	return true;
}

void
bitvec_deinit(struct bitvec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	vec->cap = 0;
	free(vec->data);
}

struct bitvec *
bitvec_alloc(size_t cap)
{
	struct bitvec *bitvec;

	bitvec = malloc(sizeof(struct bitvec));
	if (!bitvec) {
		LIBVEC_HANDLE_ERR("malloc");
		return NULL;
	}

	if (!bitvec_init(bitvec, cap)) {
		free(bitvec);
		return NULL;
	}

	return bitvec;
}

void
bitvec_free(struct bitvec *vec)
{
	bitvec_deinit(vec);
	free(vec);
}

bool
bitvec_reserve(struct bitvec *vec, size_t cnt)
{
	LIBVEC_ASSERT(vec != NULL);

	cnt = BITCEIL(cnt);
	if (vec->cap >= cnt) return true;

	vec->data = realloc(vec->data, SLOTCNT(cnt) * SLOT_BYTES);
	if (!vec->data) {
		LIBVEC_HANDLE_ERR("realloc");
		return false;
	}

	memset(vec->data + SLOT(vec->cap), 0, SLOT(cnt) - SLOT(vec->cap));
	vec->cap = cnt;

	return true;
}

bool
bitvec_shrink(struct bitvec *vec, size_t cnt)
{
	LIBVEC_ASSERT(vec != NULL);

	cnt = BITCEIL(cnt);
	if (vec->cap <= cnt) return true;

	vec->data = realloc(vec->data, SLOTCNT(cnt));
	if (!vec->data) {
		LIBVEC_HANDLE_ERR("realloc");
		return false;
	}

	vec->cap = cnt;

	return true;
}

bool
bitvec_get(struct bitvec *vec, size_t pos)
{
	LIBVEC_ASSERT(vec != NULL);

	if (pos >= vec->cap) return false;
	return !!(vec->data[pos / SLOT_BITS] & SLOT_BIT(pos % SLOT_BITS));
}

void
bitvec_set(struct bitvec *vec, size_t pos)
{
	LIBVEC_ASSERT(vec != NULL && pos < vec->cap);

	vec->data[pos / SLOT_BITS] |= SLOT_BIT(pos % SLOT_BITS);
}

void
bitvec_clear(struct bitvec *vec, size_t pos)
{
	LIBVEC_ASSERT(vec != NULL && pos < vec->cap);

	vec->data[pos / SLOT_BITS] &= ~SLOT_BIT(pos % SLOT_BITS);
}

void
bitvec_setn(struct bitvec *vec, size_t start, size_t end, bool set)
{
	libvec_bitslot_t mask;
	size_t starti, endi, i;

	LIBVEC_ASSERT(vec != NULL && end >= start && end <= vec->cap);

	if (start == end) return;

	starti = SLOT(start);
	end = SLOT(end - 1);

	if (starti == endi) {
		if (end % SLOT_BITS == 0)
			mask = SLOT_MAX - SLOT_BIT(start % SLOT_BITS);
		else
			mask = SLOT_BIT(end % SLOT_BITS) - SLOT_BIT(start % SLOT_BITS);
		vec->data[starti] = APPLY_MASK(vec->data[starti], set, mask);
	} else {
		mask = SLOT_MAX - SLOT_BIT(start % SLOT_BITS);
		vec->data[starti] = APPLY_MASK(vec->data[starti], set, mask);

		for (i = starti + 1; i <= endi - 1; i++)
			vec->data[i] = APPLY_MASK(vec->data[i], set, SLOT_MAX);

		mask = SLOT_BIT(end % SLOT_BITS) - SLOT_BIT(0);
		vec->data[endi] = APPLY_MASK(vec->data[endi], set, mask);
	}
}

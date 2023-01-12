#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LIBVEC_ASSERT_ENABLE
#define LIBVEC_ASSERT(x) libvec_assert((x), __FILE__, __LINE__, #x)
#else
#define LIBVEC_ASSERT(x)
#endif

struct vec {
	size_t dsize;
	size_t len, cap;

	void *data;
};

void vec_init(struct vec *vec, size_t dsize, size_t cap);
void vec_deinit(struct vec *vec);

struct vec *vec_alloc(size_t dsize, size_t cap);
void vec_free(struct vec *vec);

void vec_clear(struct vec *vec);
void vec_resize(struct vec *vec, size_t cap);
void vec_shrink(struct vec *vec);

void vec_reserve(struct vec *vec, size_t index, size_t count);
void vec_remove(struct vec *vec, size_t index, size_t count);
void vec_replace(struct vec *vec, size_t index, const void *data, size_t count);

bool vec_iter_fwd(struct vec *vec, void **p);
bool vec_iter_bwd(struct vec *vec, void **p);

static inline void libvec_assert(int cond,
	const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "libvec: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	abort();
}

static inline void *vec_at(struct vec *vec, size_t index)
{
	LIBVEC_ASSERT(vec != NULL && index < vec->len);

	return vec->data + index * vec->dsize;
}

static inline void *vec_front(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	return vec->data;
}

static inline void *vec_back(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL && vec->len > 0);

	return vec->data + (vec->len - 1) * vec->dsize;
}

static inline bool vec_empty(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	return !vec->len;
}

static inline void *
vec_alloc_slots(struct vec *vec, size_t count)
{
	vec_reserve(vec, vec->len, count);
	return vec->data + (vec->len - count) * vec->dsize;
}

static inline void *
vec_alloc_slot(struct vec *vec)
{
	return vec_alloc_slots(vec, 1);
}

static inline void
vec_free_slots(struct vec *vec, void *slot, size_t count)
{
	LIBVEC_ASSERT(vec != NULL && slot >= vec->data
		&& slot < vec->data + vec->len * vec->dsize
		&& (slot - vec->data) % vec->dsize == 0);

	vec_remove(vec, (slot - vec->data) / vec->dsize, count);
}

static inline void *
vec_free_slot(struct vec *vec, void *slot)
{
	return vec_free_slot(vec, slot);
}

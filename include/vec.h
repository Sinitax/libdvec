#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct vec {
	size_t dsize;
	size_t len, cap;

	void *data;
};

int vec_init(struct vec *vec, size_t dsize, size_t cap);
void vec_deinit(struct vec *vec);

int vec_alloc(struct vec **vec, size_t dsize, size_t cap);
void vec_free(struct vec *vec);

void vec_clear(struct vec *vec);
int vec_resize(struct vec *vec, size_t cap);
int vec_shrink(struct vec *vec);

int vec_reserve(struct vec *vec, size_t index, size_t count);
void vec_remove(struct vec *vec, size_t index, size_t count);
void vec_replace(struct vec *vec, size_t index, const void *data, size_t count);

bool vec_iter_fwd(struct vec *vec, void **p);
bool vec_iter_bwd(struct vec *vec, void **p);

static inline void *
vec_at(struct vec *vec, size_t index)
{
	return vec->data + index * vec->dsize;
}

static inline void *
vec_front(struct vec *vec)
{
	return vec->data;
}

static inline void *
vec_back(struct vec *vec)
{
	return vec->data + (vec->len - 1) * vec->dsize;
}

static inline bool
vec_empty(struct vec *vec)
{
	return !vec->len;
}

static inline int
vec_alloc_slots(struct vec *vec, void **out, size_t count)
{
	int ret;

	ret = vec_reserve(vec, vec->len, count);
	if (ret) return ret;

	*out = vec->data + (vec->len - count) * vec->dsize;

	return 0;
}

static inline int
vec_alloc_slot(struct vec *vec, void **out)
{
	return vec_alloc_slots(vec, out, 1);
}

static inline void
vec_free_slots(struct vec *vec, void *slot, size_t count)
{
	vec_remove(vec, (slot - vec->data) / vec->dsize, count);
}

static inline void
vec_free_slot(struct vec *vec, void *slot)
{
	vec_free_slots(vec, slot, 1);
}

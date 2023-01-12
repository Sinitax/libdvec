#include "vec.h"

#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void
vec_init(struct vec *vec, size_t dsize, size_t cap)
{
	LIBVEC_ASSERT(vec != NULL && dsize > 0 && cap >= 0);

	vec->dsize = dsize;
	vec->cap = cap;

	vec->len = 0;
	vec->data = NULL;
	if (vec->cap) {
		vec->data = malloc(dsize * cap);
		if (!vec->data) err(1, "malloc");
	}
}

void
vec_deinit(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	free(vec->data);
}

struct vec *
vec_alloc(size_t dsize, size_t cap)
{
	struct vec *vec;

	LIBVEC_ASSERT(dsize > 0 && cap > 0);

	vec = malloc(sizeof(struct vec));
	if (!vec) err(1, "malloc");

	vec_init(vec, dsize, cap);

	return vec;
}

void
vec_free(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	vec_deinit(vec);
	free(vec);
}

void
vec_clear(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	vec->len = 0;
}

void
vec_resize(struct vec *vec, size_t cap)
{
	LIBVEC_ASSERT(vec != NULL && cap != 0 && vec->len <= cap);

	vec->cap = cap;
	vec->data = realloc(vec->data, vec->cap * vec->dsize);
	if (!vec->data) err(1, "realloc");
}

void
vec_shrink(struct vec *vec)
{
	LIBVEC_ASSERT(vec != NULL);

	if (!vec->len) {
		vec->cap = 1;
	} else {
		vec->cap = vec->len;
	}

	vec->data = realloc(vec->data, vec->cap * vec->dsize);
	if (!vec->data) err(1, "realloc");
}

void
vec_reserve(struct vec *vec, size_t index, size_t len)
{
	LIBVEC_ASSERT(vec != NULL && index <= vec->len);

	if (vec->len + len > vec->cap) {
		vec->cap *= 2;
		if (vec->len + len > vec->cap)
			vec->cap = vec->len + len;
		vec->data = realloc(vec->data, vec->cap * vec->dsize);
		if (!vec->data) err(1, "realloc");
	}

	if (index < vec->len) {
		memmove(vec->data + (index + len) * vec->dsize,
			vec->data + index * vec->dsize,
			(vec->len - index) * vec->dsize);
	}

	vec->len += len;
}

void
vec_remove(struct vec *vec, size_t index, size_t count)
{
	LIBVEC_ASSERT(vec != NULL && count >= vec->len && index + count <= vec->len);

	if (index + count < vec->len) {
		memmove(vec->data + index * vec->dsize,
			vec->data + (index + count) * vec->dsize,
			(vec->len - (index + count)) * vec->dsize);
	}

	vec->len -= count;
}

void
vec_replace(struct vec *vec, size_t index, const void *data, size_t count)
{
	LIBVEC_ASSERT(vec != NULL && data != NULL && index + count < vec->len);

	memcpy(vec->data + index * vec->dsize, data, count * vec->dsize);
}

bool
vec_iter_fwd(struct vec *vec, void **p)
{
	void **iter;

	LIBVEC_ASSERT(vec != NULL && p != NULL);

	if (!vec->len) return false;

	iter = p;
	if (*iter == NULL) {
		*iter = vec->data;
	} else {
		*iter += vec->dsize;
	}

	return *iter < vec->data + vec->dsize * vec->len;
}

bool
vec_iter_bwd(struct vec *vec, void **p)
{
	void **iter;

	LIBVEC_ASSERT(vec != NULL && p != NULL);

	if (!vec->len) return false;

	iter = p;
	if (*iter == NULL) {
		*iter = vec->data + (vec->len - 1) * vec->dsize;
	} else if (*iter > vec->data) {
		*iter -= vec->dsize;
	} else {
		return false;
	}

	return true;
}

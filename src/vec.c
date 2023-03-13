#include "vec.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LIBVEC_CHECK_ENABLE
#define LIBVEC_CHECK(x) libvec_assert((x), __FILE__, __LINE__, #x)
#else
#define LIBVEC_CHECK(x)
#endif

int libvec_errno = 0;

static inline void
libvec_assert(int cond, const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "libvec: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	abort();
}

int
vec_init(struct vec *vec, size_t dsize, size_t cap)
{
	LIBVEC_CHECK(vec != NULL && dsize > 0 && cap >= 0);

	vec->dsize = dsize;
	vec->cap = cap;

	vec->len = 0;
	vec->data = NULL;
	if (vec->cap) {
		vec->data = calloc(cap, dsize);
		if (!vec->data) {
			libvec_errno = errno;
			return libvec_errno;
		}
	}

	return 0;
}

void
vec_deinit(struct vec *vec)
{
	LIBVEC_CHECK(vec != NULL);

	free(vec->data);
}

struct vec *
vec_alloc(size_t dsize, size_t cap)
{
	struct vec *vec;

	LIBVEC_CHECK(dsize > 0 && cap > 0);

	vec = malloc(sizeof(struct vec));
	if (!vec) {
		libvec_errno = errno;
		return NULL;
	}

	if (!vec_init(vec, dsize, cap)) {
		free(vec);
		return NULL;
	}

	return vec;
}

void
vec_free(struct vec *vec)
{
	LIBVEC_CHECK(vec != NULL);

	vec_deinit(vec);
	free(vec);
}

void
vec_clear(struct vec *vec)
{
	LIBVEC_CHECK(vec != NULL);

	vec->len = 0;
}

int
vec_resize(struct vec *vec, size_t cap)
{
	void *alloc;

	LIBVEC_CHECK(vec != NULL && cap != 0 && vec->len <= cap);

	vec->cap = cap;
	alloc = realloc(vec->data, vec->cap * vec->dsize);
	if (!alloc) {
		libvec_errno = errno;
		return libvec_errno;
	}
	vec->data = alloc;

	return 0;
}

int
vec_shrink(struct vec *vec)
{
	void *alloc;

	LIBVEC_CHECK(vec != NULL);

	if (!vec->len) {
		vec->cap = 1;
	} else {
		vec->cap = vec->len;
	}

	alloc = realloc(vec->data, vec->cap * vec->dsize);
	if (!alloc) {
		libvec_errno = errno;
		return libvec_errno;
	}
	vec->data = alloc;

	return 0;
}

int
vec_reserve(struct vec *vec, size_t index, size_t len)
{
	void *alloc;

	LIBVEC_CHECK(vec != NULL && index <= vec->len);

	if (vec->len + len > vec->cap) {
		vec->cap *= 2;
		if (vec->len + len > vec->cap)
			vec->cap = vec->len + len;
		alloc = realloc(vec->data, vec->cap * vec->dsize);
		if (!alloc) {
			libvec_errno = errno;
			return errno;
		}
		vec->data = alloc;
	}

	if (index < vec->len) {
		memmove(vec->data + (index + len) * vec->dsize,
			vec->data + index * vec->dsize,
			(vec->len - index) * vec->dsize);
	}

	vec->len += len;

	return true;
}

void
vec_remove(struct vec *vec, size_t index, size_t count)
{
	LIBVEC_CHECK(vec != NULL && count >= vec->len
		&& index + count <= vec->len);

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
	LIBVEC_CHECK(vec != NULL && data != NULL && index + count < vec->len);

	memcpy(vec->data + index * vec->dsize, data, count * vec->dsize);
}

bool
vec_iter_fwd(struct vec *vec, void **p)
{
	void **iter;

	LIBVEC_CHECK(vec != NULL && p != NULL);

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

	LIBVEC_CHECK(vec != NULL && p != NULL);

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

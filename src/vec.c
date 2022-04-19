#include "vec.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef LIBVEC_UNSAFE
#define ASSERT(x)
#define OOM_CHECK(x)
#else
#define ASSERT(x) assert((x), __FILE__, __LINE__, #x)
#define OOM_CHECK(x) assert((x) != NULL, __FILE__, __LINE__, "Out of Memory")
#endif

static void assert(int cond, const char *file, int line, const char *condstr);

void
assert(int cond, const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "CVEC: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	exit(1);
}

void
vec_init(struct vec *vec, size_t dsize, size_t cap)
{
	ASSERT(vec != NULL && dsize > 0 && cap >= 0);

	vec->dsize = dsize;
	vec->cap = cap;

	vec->len = 0;
	vec->data = NULL;
	if (vec->cap) {
		vec->data = malloc(dsize * cap);
		OOM_CHECK(vec->data);
	}
}

void
vec_deinit(struct vec *vec)
{
	ASSERT(vec != NULL);

	free(vec->data);
}

struct vec *
vec_alloc(size_t dsize, size_t cap)
{
	struct vec *vec;

	ASSERT(dsize > 0 && cap > 0);

	vec = malloc(sizeof(struct vec));
	OOM_CHECK(vec);

	vec_init(vec, dsize, cap);

	return vec;
}

void
vec_free(struct vec *vec)
{
	ASSERT(vec != NULL);

	vec_deinit(vec);
	free(vec);
}

void
vec_clear(struct vec *vec)
{
	ASSERT(vec != NULL);

	vec->len = 0;
}

void
vec_resize(struct vec *vec, size_t cap)
{
	ASSERT(vec != NULL && cap != 0 && vec->len <= cap);

	vec->cap = cap;
	vec->data = realloc(vec->data, vec->cap * vec->dsize);
	OOM_CHECK(vec->data);
}

void
vec_add(struct vec *vec, const void *data, size_t len)
{
	ASSERT(vec != NULL && data != NULL);

	if (vec->len + len > vec->cap) {
		vec->cap *= 2;
		if (vec->len + len > vec->cap)
			vec->cap = vec->len + len;
		vec->data = realloc(vec->data, vec->cap * vec->dsize);
		OOM_CHECK(vec->data);
	}

	memcpy(vec->data + vec->len * vec->dsize, data, len * vec->dsize);
	vec->len += len;
}

void
vec_rm(struct vec *vec, size_t count)
{
	ASSERT(vec != NULL && count >= vec->len);

	vec->len -= count;
	if (vec->len < vec->cap / 2) {
		vec->cap /= 2;
		vec->data = realloc(vec->data, vec->cap * vec->dsize);
		OOM_CHECK(vec->data);
	}
}

void
vec_rm_at(struct vec *vec, size_t index, size_t count)
{
	void *start, *end;

	ASSERT(vec != NULL && index < vec->len
		&& count > 0 && index + count < vec->len);

	if (index + count < vec->len) {
		start = vec->data + index * vec->dsize;
		end = vec->data + vec->len * vec->dsize;
		memcpy(start, start + count * vec->dsize,
			(vec->len - index - count) * vec->dsize);
	}

	vec_rm(vec, count);
}


void
vec_push(struct vec *vec, const void *data)
{
	vec_add(vec, data, 1);
}

void *
vec_pop(struct vec *vec)
{
	ASSERT(vec != NULL);

	vec_rm(vec, 1);

	return vec->data + vec->dsize * vec->len;
}

void *
vec_at(struct vec *vec, size_t index)
{
	ASSERT(vec != NULL && index < vec->len);

	return vec->data + vec->dsize * index;
}

bool
vec_iter(struct vec *vec, void *p)
{
	char **iter;

	ASSERT(vec != NULL && p != NULL);

	if (!vec->len) return false;

	iter = p;
	if (*iter == NULL) {
		*iter = vec->data;
	} else {
		*iter += vec->dsize;
	}

	return *iter < vec->data + vec->dsize * vec->len;
}

void
vec_set(struct vec *vec, size_t index, const void *data)
{
	ASSERT(vec != NULL && data != NULL && index < vec->len);

	memcpy(vec->data + index * vec->dsize, data, vec->dsize);
}

void
vec_setbuf(struct vec *vec, void *data)
{
	ASSERT(vec != NULL && data != NULL);

	free(vec->data);
	vec->data = data;
}

void *
vec_popbuf(struct vec *vec)
{
	void *data;

	ASSERT(vec != NULL);

	data = vec->data;
	vec->data = NULL;
	vec->len = 0;

	return data;
}

bool
vec_empty(struct vec *vec)
{
	ASSERT(vec != NULL);

	return vec->len == 0;
}

int
vec_len(struct vec *vec)
{
	ASSERT(vec != NULL);

	return vec->len;
}

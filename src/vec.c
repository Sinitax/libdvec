#include "vec.h"

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#define ASSERT(x) assert((x), __FILE__, __LINE__, #x)
#define OOM_CHECK(x) assert((x) != NULL, __FILE__, __LINE__, "Out of Memory")

struct vec {
	size_t dsize;
	size_t count, cap;

	char data[];
};

static void assert(int cond, const char *file, int line, const char *condstr);

static const size_t vec_data_offset = sizeof(size_t) * 3;

void
assert(int cond, const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "CVEC: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	exit(1);
}

struct vec *
vec_alloc(size_t dsize, size_t cap)
{
	struct vec *vec;

	ASSERT(dsize != 0 && cap != 0);

	vec = malloc(vec_data_offset + cap * dsize);
	OOM_CHECK(vec);
	vec->dsize = dsize;
	vec->count = 0;
	vec->cap = cap;

	return vec;
}

void
vec_free(struct vec *vec)
{
	ASSERT(vec != NULL);

	free(vec);
}

void
vec_clear(struct vec *vec)
{
	ASSERT(vec != NULL);

	vec->count = 0;
}

void
vec_resize(struct vec *vec, size_t cap)
{
	ASSERT(vec != NULL && cap != 0 && vec->count <= cap);

	vec->cap = cap;
	vec = realloc(vec, vec_data_offset + vec->cap * vec->dsize);
	OOM_CHECK(vec);
}

void
vec_push(struct vec *vec, const void *data)
{
	ASSERT(vec != NULL && data != NULL);

	if (vec->count == vec->cap) {
		vec->cap *= 2;
		vec = realloc(vec, vec_data_offset + vec->cap * vec->dsize);
		OOM_CHECK(vec);
	}
	vec->count++;
	vec_set(vec, vec->count - 1, data);
}

void *
vec_pop(struct vec *vec)
{
	ASSERT(vec != NULL);

	vec->count--;
	if (vec->count + 1 < vec->cap / 2) {
		vec->cap /= 2;
		vec = realloc(vec, vec_data_offset + vec->cap * vec->dsize);
		OOM_CHECK(vec);
	}

	return vec->data + vec->dsize * vec->count;
}

void *
vec_at(struct vec *vec, size_t index)
{
	ASSERT(vec != NULL && index < vec->count);

	return vec->data + vec->dsize * index;
}

bool
vec_iter(struct vec *vec, void *_p)
{
	char **iter;

	ASSERT(vec != NULL && vec != NULL);

	iter = _p;
	if (*iter == NULL) {
		*iter = vec->data;
	} else {
		*iter += vec->dsize;
	}

	return *iter < vec->data + vec->dsize * vec->count;
}

void
vec_set(struct vec *vec, size_t index, const void *data)
{
	ASSERT(vec != NULL && data != NULL && index < vec->count);

	memcpy(vec->data + index * vec->dsize, data, vec->dsize);
}

void
vec_rm(struct vec *vec, size_t index)
{
	void *start, *end;

	ASSERT(vec != NULL && index < vec->count);

	start = vec->data + index * vec->dsize;
	end = vec->data + vec->count * vec->dsize;
	if (start + vec->dsize < end) {
		memcpy(start, start + vec->dsize, end - start - vec->dsize);
	}

	vec->count--;

	if (vec->count < vec->cap / 4) {
		vec->cap /= 2;
		vec = realloc(vec, vec_data_offset + vec->cap * vec->dsize);
		OOM_CHECK(vec);
	}
}

int
vec_empty(struct vec *vec)
{
	ASSERT(vec != NULL);

	return vec->count == 0;
}

int
vec_len(struct vec *vec)
{
	ASSERT(vec != NULL);

	return vec->count;
}

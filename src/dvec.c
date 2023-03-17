#include "dvec.h"

#include <string.h>
#include <stdlib.h>

#ifdef LIBDVEC_CHECK_ENABLE
#include <stdio.h>
#define LIBDVEC_CHECK(x) libdvec_assert((x), __FILE__, __LINE__, #x)
#else
#define LIBDVEC_CHECK(x)
#endif

#ifdef LIBDVEC_CHECK_ENABLE
static inline void
libdvec_assert(int cond, const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "libdvec: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	abort();
}
#endif

int
dvec_init(struct dvec *dvec, size_t dsize, size_t cap,
	const struct allocator *allocator)
{
	int rc;

	LIBDVEC_CHECK(dvec != NULL && dsize > 0
		&& cap >= 0 && allocator != NULL);

	dvec->allocator = allocator;
	dvec->dsize = dsize;
	dvec->cap = cap;
	dvec->len = 0;
	dvec->data = NULL;
	if (dvec->cap) {
		rc = allocator->alloc(&dvec->data, cap * dsize);
		if (rc) return -rc;
	}

	return 0;
}

int
dvec_deinit(struct dvec *dvec)
{
	LIBDVEC_CHECK(dvec != NULL);

	return dvec->allocator->free(dvec->data);
}

int
dvec_alloc(struct dvec **dvec, size_t dsize, size_t cap,
	const struct allocator *allocator)
{
	int rc;

	LIBDVEC_CHECK(dvec != NULL && dsize > 0 && cap > 0);

	rc = allocator->alloc((void **)dvec, sizeof(struct dvec));
	if (!*dvec) return -rc;

	rc = dvec_init(*dvec, dsize, cap, allocator);
	if (rc) return rc;

	return 0;
}

int
dvec_free(struct dvec *dvec)
{
	const struct allocator *allocator;
	int rc;

	LIBDVEC_CHECK(dvec != NULL);

	allocator = dvec->allocator;

	rc = dvec_deinit(dvec);
	if (rc) return rc;

	rc = allocator->free(dvec);
	if (rc) return -rc;

	return 0;
}

int
dvec_copy(struct dvec *dst, struct dvec *src)
{
	int rc;

	dst->dsize = src->dsize;
	rc = dvec_reserve(dst, src->len);
	if (rc) return rc;

	dst->len = src->len;
	memcpy(dst->data, src->data, src->len * src->dsize);

	return 0;
}

void
dvec_swap(struct dvec *dst, struct dvec *src)
{
	struct dvec tmp;

	memcpy(&tmp, dst, sizeof(struct dvec));
	memcpy(dst, src, sizeof(struct dvec));
	memcpy(src, &tmp, sizeof(struct dvec));
}

void
dvec_clear(struct dvec *dvec)
{
	LIBDVEC_CHECK(dvec != NULL);

	dvec->len = 0;
}

int
dvec_reserve(struct dvec *dvec, size_t len)
{
	int rc;

	if (len <= dvec->cap) return 0;

	dvec->cap *= 2;
	if (len > dvec->cap) dvec->cap = len;

	rc = dvec->allocator->realloc(&dvec->data, dvec->cap * dvec->dsize);
	if (rc) return rc;

	return 0;
}

int
dvec_shrink(struct dvec *dvec)
{
	int rc;

	LIBDVEC_CHECK(dvec != NULL);

	if (!dvec->len) {
		dvec->cap = 0;
		free(dvec->data);
		return 0;
	}

	dvec->cap = dvec->len;

	rc = dvec->allocator->realloc(&dvec->data, dvec->cap * dvec->dsize);
	if (rc) return -rc;

	return 0;
}

int
dvec_add(struct dvec *dvec, size_t index, size_t len)
{
	LIBDVEC_CHECK(dvec != NULL && index <= dvec->len);

	dvec_reserve(dvec, dvec->len + len);

	if (index < dvec->len) {
		memmove(dvec->data + (index + len) * dvec->dsize,
			dvec->data + index * dvec->dsize,
			(dvec->len - index) * dvec->dsize);
	}

	dvec->len += len;

	return 0;
}

void
dvec_remove(struct dvec *dvec, size_t index, size_t count)
{
	LIBDVEC_CHECK(dvec != NULL && count >= dvec->len
		&& index + count <= dvec->len);

	if (index + count < dvec->len) {
		memmove(dvec->data + index * dvec->dsize,
			dvec->data + (index + count) * dvec->dsize,
			(dvec->len - (index + count)) * dvec->dsize);
	}

	dvec->len -= count;
}

void
dvec_replace(struct dvec *dvec, size_t index, const void *data, size_t count)
{
	LIBDVEC_CHECK(dvec != NULL && data != NULL && index + count < dvec->len);

	memcpy(dvec->data + index * dvec->dsize, data, count * dvec->dsize);
}

bool
dvec_iter_fwd(struct dvec *dvec, void **p)
{
	void **iter;

	LIBDVEC_CHECK(dvec != NULL && p != NULL);

	if (!dvec->len) return false;

	iter = p;
	if (*iter == NULL) {
		*iter = dvec->data;
	} else {
		*iter += dvec->dsize;
	}

	return *iter < dvec->data + dvec->dsize * dvec->len;
}

bool
dvec_iter_bwd(struct dvec *dvec, void **p)
{
	void **iter;

	LIBDVEC_CHECK(dvec != NULL && p != NULL);

	if (!dvec->len) return false;

	iter = p;
	if (*iter == NULL) {
		*iter = dvec->data + (dvec->len - 1) * dvec->dsize;
	} else if (*iter > dvec->data) {
		*iter -= dvec->dsize;
	} else {
		return false;
	}

	return true;
}

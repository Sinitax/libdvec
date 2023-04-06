#pragma once

#include "allocator.h"

#include <stdbool.h>
#include <stddef.h>

#define DVEC_ITER(dvec, p)     (p) = NULL; ((p) = dvec_iter_fwd((dvec), (p)));
#define DVEC_ITER_BWD(dvec, p) (p) = NULL; ((p) = dvec_iter_bwd((dvec), (p)));

#define DVEC_STRERR_INIT \
	[DVEC_OK] = "Success"

#ifdef LIBDVEC_ASSERT_ARGS
#include "stdlib.h"
#define LIBDVEC_ABORT_ON_ARGS(cond) do { if (cond) abort(); } while (0)
#else
#define LIBDVEC_ABORT_ON_ARGS(cond)
#endif

#ifdef LIBDVEC_ASSERT_ALLOC
#include "stdlib.h"
#define LIBDVEC_ABORT_ON_ALLOC(cond) do { if (cond) abort(); } while (0)
#else
#define LIBDVEC_ABORT_ON_ALLOC(cond)
#endif

enum {
	DVEC_OK = 0,
};

struct dvec {
	size_t dsize;
	size_t len, cap;

	void *data;

	const struct allocator *allocator;
};

int dvec_init(struct dvec *dvec, size_t dsize, size_t cap,
	const struct allocator *allocator);
int dvec_deinit(struct dvec *dvec);

int dvec_alloc(struct dvec **dvec, size_t dsize, size_t cap,
	const struct allocator *allocator);
int dvec_free(struct dvec *dvec);

int dvec_copy(struct dvec *dst, struct dvec *src);
void dvec_swap(struct dvec *dst, struct dvec *src);

void dvec_clear(struct dvec *dvec);
int dvec_reserve(struct dvec *dvec, size_t cap);
int dvec_shrink(struct dvec *dvec);

int dvec_add(struct dvec *dvec, size_t index, size_t count);
void dvec_remove(struct dvec *dvec, size_t index, size_t count);
void dvec_replace(struct dvec *dvec, size_t index,
	const void *data, size_t count);

void *dvec_iter_fwd(const struct dvec *dvec, const void *p);
void *dvec_iter_bwd(const struct dvec *dvec, const void *p);

static inline void *
dvec_at(const struct dvec *dvec, size_t index)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec || index >= dvec->len);

	return dvec->data + index * dvec->dsize;
}

static inline void *
dvec_at_back(const struct dvec *dvec, size_t index)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec || !index || index >= dvec->len);

	return dvec->data + (dvec->len  - 1 - index) * dvec->dsize;
}

static inline void *
dvec_front(const struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->data;
}

static inline void *
dvec_back(const struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->data + (dvec->len - 1) * dvec->dsize;
}

static inline bool
dvec_empty(const struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return !dvec->len;
}

static inline size_t
dvec_len(const struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->len;
}

static inline int
dvec_add_back(struct dvec *dvec, size_t count)
{
	return dvec_add(dvec, dvec->len, count);
}

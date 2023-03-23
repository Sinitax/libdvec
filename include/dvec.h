#pragma once

#include "allocator.h"

#include <stdbool.h>
#include <stddef.h>

#define DVEC_ITER(dvec, p) *p = NULL; dvec_iter_fwd(dvec, (void **)(p));

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

bool dvec_iter_fwd(const struct dvec *dvec, void **p);
bool dvec_iter_bwd(const struct dvec *dvec, void **p);

static inline void *
dvec_at(struct dvec *dvec, size_t index)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->data + index * dvec->dsize;
}

static inline void *
dvec_front(struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->data;
}

static inline void *
dvec_back(struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->data + (dvec->len - 1) * dvec->dsize;
}

static inline bool
dvec_empty(struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return !dvec->len;
}

static inline size_t
dvec_len(struct dvec *dvec)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec->len;
}

static inline void *
dvec_add_slots(struct dvec *dvec, int *_rc, size_t count)
{
	int rc;

	LIBDVEC_ABORT_ON_ARGS(!dvec || !count);

	rc = dvec_add(dvec, dvec->len, count);
	if (_rc) *_rc = rc;
	if (rc) return NULL;

	return dvec->data + (dvec->len - count) * dvec->dsize;
}

static inline void *
dvec_add_slot(struct dvec *dvec, int *rc)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec);

	return dvec_add_slots(dvec, rc, 1);
}

static inline void
dvec_rm_slots(struct dvec *dvec, void *slot, size_t count)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec || slot < dvec->data || !count);

	dvec_remove(dvec, ((size_t) (slot - dvec->data)) / dvec->dsize, count);
}

static inline void
dvec_rm_slot(struct dvec *dvec, void *slot)
{
	LIBDVEC_ABORT_ON_ARGS(!dvec || !slot);

	dvec_rm_slots(dvec, slot, 1);
}

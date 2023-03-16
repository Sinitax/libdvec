#pragma once

#include <stdbool.h>
#include <stdlib.h>

struct dvec {
	size_t dsize;
	size_t len, cap;

	void *data;
};

int dvec_init(struct dvec *dvec, size_t dsize, size_t cap);
void dvec_deinit(struct dvec *dvec);

int dvec_alloc(struct dvec **dvec, size_t dsize, size_t cap);
void dvec_free(struct dvec *dvec);

void dvec_clear(struct dvec *dvec);
int dvec_resize(struct dvec *dvec, size_t cap);
int dvec_shrink(struct dvec *dvec);

int dvec_reserve(struct dvec *dvec, size_t index, size_t count);
void dvec_remove(struct dvec *dvec, size_t index, size_t count);
void dvec_replace(struct dvec *dvec, size_t index, const void *data, size_t count);

bool dvec_iter_fwd(struct dvec *dvec, void **p);
bool dvec_iter_bwd(struct dvec *dvec, void **p);

static inline void *
dvec_at(struct dvec *dvec, size_t index)
{
	return dvec->data + index * dvec->dsize;
}

static inline void *
dvec_front(struct dvec *dvec)
{
	return dvec->data;
}

static inline void *
dvec_back(struct dvec *dvec)
{
	return dvec->data + (dvec->len - 1) * dvec->dsize;
}

static inline bool
dvec_empty(struct dvec *dvec)
{
	return !dvec->len;
}

static inline int
dvec_alloc_slots(struct dvec *dvec, void **out, size_t count)
{
	int ret;

	ret = dvec_reserve(dvec, dvec->len, count);
	if (ret) return ret;

	*out = dvec->data + (dvec->len - count) * dvec->dsize;

	return 0;
}

static inline int
dvec_alloc_slot(struct dvec *dvec, void **out)
{
	return dvec_alloc_slots(dvec, out, 1);
}

static inline void
dvec_free_slots(struct dvec *dvec, void *slot, size_t count)
{
	dvec_remove(dvec, (slot - dvec->data) / dvec->dsize, count);
}

static inline void
dvec_free_slot(struct dvec *dvec, void *slot)
{
	dvec_free_slots(dvec, slot, 1);
}

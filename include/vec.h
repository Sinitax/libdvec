#include <stdbool.h>
#include <stdlib.h>

#define VEC_ITER(vec, type, var) \
	var = (type*) NULL; vec_iter(vec, &var); 

#define VEC_PUSH(vec, type, val) \
	do { \
		type tmp = (val); \
		vec_push(vec, &tmp); \
	} while (0)

#define VEC_POP(vec, type, var) \
	do { \
		type *tmp = vec_pop(vec); \
		memcpy(&var, tmp, sizeof(type); \
	} while (0)

#define VEC_AT(vec, type, idx) \
		(*(type *)vec_at(vec, idx))

struct vec {
	size_t dsize;
	size_t len, cap;

	char *data;
};

void vec_init(struct vec *vec, size_t dsize, size_t cap);
void vec_deinit(struct vec *vec);

struct vec *vec_alloc(size_t dsize, size_t cap);
void vec_free(struct vec *vec);

void vec_clear(struct vec *vec);
void vec_resize(struct vec *vec, size_t cap);

void vec_add(struct vec *vec, const void *data, size_t count);
void vec_rm(struct vec *vec, size_t count);
void vec_rm_at(struct vec *vec, size_t index, size_t count);

void vec_push(struct vec *vec, const void *data);
void *vec_pop(struct vec *vec);

void *vec_at(struct vec *vec, size_t index);
bool vec_iter(struct vec *vec, void *p);

void vec_set(struct vec *vec, size_t index, const void *data);

void vec_setbuf(struct vec *vec, void *buf);
void *vec_popbuf(struct vec *vec);

bool vec_empty(struct vec *vec);
int vec_len(struct vec *vec);

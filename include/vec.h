#include <stdbool.h>
#include <stdlib.h>

struct vec;

struct vec *vec_alloc(size_t dsize, size_t cap);
void vec_free(struct vec *vec);

void vec_clear(struct vec *vec);
void vec_resize(struct vec *vec, size_t cap);

void vec_add(struct vec *vec, const void *data, size_t count);
void vec_rm(struct vec *vec, size_t count);

void vec_push(struct vec *vec, const void *data);
void *vec_pop(struct vec *vec);
void vec_pop_at(struct vec *vec, size_t index);

void *vec_at(struct vec *vec, size_t index);
bool vec_iter(struct vec *vec, void *p);

void vec_set(struct vec *vec, size_t index, const void *data);

int vec_empty(struct vec *vec);
int vec_len(struct vec *vec);

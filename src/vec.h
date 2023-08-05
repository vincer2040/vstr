#ifndef __VEC_H__

#define __VEC_H__

#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    size_t data_size;
    unsigned char data[];
} vec;

typedef void VecFreeCallBack(void* ptr);

vec* vec_new(size_t data_size);
int vec_push(vec** vec, void* data);
int vec_pop(vec* vec, void* out);
size_t vec_len(vec* vec);
void vec_free(vec* vec, VecFreeCallBack* fcb);

#endif /* __VEC_H__ */

#include "vec.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VEC_INITIAL_CAP 32

#define realloc_vec(v, cap, ins, data_size)                                    \
    {                                                                          \
        *v = realloc(*v, sizeof(vec) + (cap * data_size));                     \
        if (*v == NULL) {                                                      \
            return -1;                                                         \
        }                                                                      \
        memset((*v)->data + (ins * data_size), 0, (cap - ins) * data_size);    \
        (*v)->cap = cap;                                                       \
    }

/**
 * @brief create a new vector
 * @param data_size the size of the data held in the vector
 * @return an allocated vector or NULL if malloc returns NULL
 */
vec* vec_new(size_t data_size) {
    vec* v;
    size_t needed_size = sizeof(vec) + (VEC_INITIAL_CAP * data_size);
    v = malloc(needed_size);
    if (v == NULL) {
        return NULL;
    }
    memset(v, 0, needed_size);
    v->data_size = data_size;
    v->len = 0;
    v->cap = VEC_INITIAL_CAP;
    return v;
}

/**
 * @brief append an element to the end of a vector
 * @param v pointer to pointer of allocated vector
 * @param data the data to push back - must not be NULL
 * @return 0 on success, -1 if realloc is called and returns NULL
 */
int vec_push(vec** v, void* data) {
    size_t ins, cap, data_size;
    vec sv;
    sv = **v;
    ins = sv.len;
    cap = sv.cap;
    data_size = sv.data_size;
    if (ins == cap) {
        cap <<= 1; // multiply by two
        realloc_vec(v, cap, ins, data_size);
    }
    memcpy((*v)->data + (ins * data_size), data, data_size);
    (*v)->len++;
    return 0;
}

/**
 * @brief pop an element off the end of a vector
 * @param v pointer to allocated vector
 * @param out pointer to copy popped element to
 * @return 0 if element is popped, -1 if length is 0
 */
int vec_pop(vec* v, void* out) {
    size_t len, data_size;
    len = v->len;
    data_size = v->data_size;
    if (len == 0) {
        return -1;
    }
    len--;
    memcpy(out, v->data + (len * data_size), data_size);
    memset(v->data + (len * data_size), 0, data_size);
    v->len--;
    return 0;
}

/**
 * @brief get the length of the vector
 * @param v pointer to allocated vector
 * @return length of vector
 */
size_t vec_len(vec* v) { return v->len; }

/**
 * @brief free the vector
 * @param v pointer to allocated vector
 * @param fcb callback function to free elements in array - nullable
 */
void vec_free(vec* v, VecFreeCallBack* fcb) {
    if (fcb) {
        size_t i, len;
        len = v->len;
        for (i = 0; i < len; ++i) {
            void* ptr = v->data + i;
            fcb(ptr);
        }
    }
    free(v);
}

vec_iter vec_iter_new(vec* vec) {
    vec_iter vi = { 0 };
    vi.vec = vec;
    vi.end_idx = vec->len;
    vi.at_idx = 0;
    vec_iter_next(&vi);
    vec_iter_next(&vi);
    return vi;
}

void vec_iter_next(vec_iter* iter) {
    if (iter->at_idx >= iter->end_idx) {
        iter->cur = iter->next;
        iter->next = NULL;
        return;
    }
    iter->cur = iter->next;
    iter->next = iter->vec->data + (iter->at_idx * iter->vec->data_size);
    iter->at_idx++;
}

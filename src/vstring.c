#include "vstring.h"
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

vstring* vstring_new(void);
vstring* vstring_from(const char* cstr);
vstring* vstring_new_len(size_t initial_cap);
int vstring_push_char(vstring** vstring_obj, char c);
int vstring_push_string(vstring** vstring_obj, const char* cstr);
int vstring_set(vstring** vstring_obj, const char* cstr);
int vstring_push_string_len(vstring** vstring_obj, const char* cstr, size_t cstr_len);

#define VSTRING_INITIAL_CAP 32
#define VSTRING_OFFSET (intptr_t)(&((vstring*)NULL)->data)

#define realloc_vstr(vstring_obj, ins, cap)                                    \
    {                                                                          \
        vstring* tmp;                                                          \
        tmp = vstr_realloc(*vstring_obj, sizeof(vstring) + cap);               \
        if (tmp == NULL) {                                                     \
            free(*vstring_obj);                                                \
            return -1;                                                         \
        }                                                                      \
        *vstring_obj = tmp;                                                    \
        memset((*vstring_obj)->data + ins, 0, cap - ins);                      \
        (*vstring_obj)->hdr.cap = cap;                                         \
    }

/**
 * @brief allocate a new vstr
 * @return pointer to string, NULL if malloc failed
 */
vstr vstr_new(void) {
    vstring* vstring_obj;

    vstring_obj = vstring_new();
    if (vstring_obj == NULL) {
        return NULL;
    }

    return vstring_obj->data;
}

/**
 * @breif allocate a new vstr from a cstr
 * @param cstr the string to copy - must not be NULL
 * @return pointer to string, NULL if malloc failed
 */
vstr vstr_from(const char* cstr) {
    vstring* vstring_obj;

    vstring_obj = vstring_from(cstr);
    if (vstring_obj == NULL) {
        return NULL;
    }

    return vstring_obj->data;
}

/**
 * @brief allocate a new vstr with specified capacity
 * @param initial_cap the initial capacity
 * @returns vstr, NULL if malloc failed
 */
vstr vstr_new_len(size_t initial_cap) {
    vstring* vstring_obj = vstring_new_len(initial_cap);
    if (vstring_obj == NULL) {
        return NULL;
    }
    return vstring_obj->data;
}

/**
 * @brief duplicate a vstr
 * @param str the vstr to copy
 * @returns vstr, NULL if malloc fails
 */
vstr vstr_dup(vstr vstr) {
    vstring* vstring_obj = vstring_from(vstr);
    if (vstring_obj == NULL) {
        return NULL;
    }
    return vstring_obj->data;
}

/**
 * @brief create a vstr from a format
 * @param fmt the format string
 * @param ... the arguments
 * @returns vstr, NULL if fail
 */
vstr vstr_format(const char* fmt, ...) {
    int n;
    size_t size = 0;
    char* p = NULL;
    va_list ap;
    vstring* vstring_obj;

    va_start(ap, fmt);
    n = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (n < 0) {
        return NULL;
    }

    size = ((size_t)n) + 1;
    vstring_obj = vstring_new_len(size);
    if (vstring_obj == NULL) {
        return NULL;
    }

    va_start(ap, fmt);
    n = vsnprintf(vstring_obj->data, size, fmt, ap);
    va_end(ap);

    if (n < 0) {
        vstr_free(vstring_obj);
        return NULL;
    }

    return vstring_obj->data;
}

/**
 * @brief set contents of a vstring
 * @param str pointer to vstring
 * @param cstr the string to set vstr to - must not be NULL
 * @return pointer to vstr, NULL if realloc called and failed
 */
vstr vstr_set(vstr vstr, const char* cstr) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    int set_res = vstring_set(&vstring_obj, cstr);
    if (set_res == -1) {
        return NULL;
    }

    return vstring_obj->data;
}

/**
 * @brief append a char to the end of str
 * @param str the allocated vstr
 * @param c the char to push
 * @return pointer to string, NULL if realloc called and failed
 */
vstr vstr_push_char(vstr vstr, char c) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    int push_res = vstring_push_char(&vstring_obj, c);
    if (push_res == -1) {
        return NULL;
    }
    return vstring_obj->data;
}

/**
 * @brief append a string to the end of str
 * @param str the allocated vstr
 * @param cstr pointer to c string - must not be NULL
 * @return pointer to string, NULL if realloc called and failed
 */
vstr vstr_push_string(vstr vstr, const char* cstr) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    int push_res = vstring_push_string(&vstring_obj, cstr);
    if (push_res == -1) {
        return NULL;
    }
    return vstring_obj->data;
}

/**
 * @brief append a string to the end of str
 * @param str the allocated vstr
 * @param cstr pointer to c string - must not be NULL
 * @param cstr_len the length of cstr ot push
 * @return pointer to string, NULL if realloc called and failed
 */
vstr vstr_push_string_len(vstr vstr, const char* cstr, size_t cstr_len) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    int push_res = vstring_push_string_len(&vstring_obj, cstr, cstr_len);
    if (push_res == -1) {
        return NULL;
    }
    return vstring_obj->data;
}

/**
 * @brief get length of the vstr
 * @param str pointer to vstr
 * @return length of the string
 */
size_t vstr_len(vstr vstr) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    return vstring_obj->hdr.len;
}

/**
 * @brief get capacity of the vstr
 * @param str pointer to vstr
 * @return capacity of the vstr
 */
size_t vstr_cap(vstr vstr) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    return vstring_obj->hdr.cap - 1; // -1 to account for null terminator
}

/**
 * @brief get available space in the vstr
 * @param str pointer to vstr
 * @return available space in vstr
 */
size_t vstr_available(vstr vstr) {
    vstring* vstring_obj = ((vstring*)(vstr - VSTRING_OFFSET));
    // cap is always >= len + 1
    // we accomodate for null terminator with - 1
    return vstring_obj->hdr.cap - vstring_obj->hdr.len - 1;
}

/**
 * @brief free vstring
 * @param pointer to vstr to free
 */
void vstr_delete(vstr vstr) {
    void* ptr = vstr - VSTRING_OFFSET;
    free(ptr);
}

/**
 * @brief allocate a vstring
 * @return vstring pointer, NULL if malloc returns NULL
 */
vstring* vstring_new(void) {
    vstring* vstring_obj;
    size_t needed_len = sizeof(vstring) + VSTRING_INITIAL_CAP;

    vstring_obj = vstr_malloc(needed_len);
    if (vstring_obj == NULL) {
        return NULL;
    }

    memset(vstring_obj, 0, needed_len);
    vstring_obj->hdr.len = 0;
    vstring_obj->hdr.cap = VSTRING_INITIAL_CAP;

    return vstring_obj;
}

/**
 * @brief allocate a vstring initialized from a c string
 * @param cstr string to initialize from - should not be NULL
 * @return vstring pointer, NULL if malloc returns NULL
 */
vstring* vstring_from(const char* cstr) {
    vstring* vstring_obj;
    size_t cap = strlen(cstr);
    size_t needed_len = sizeof(vstring) + cap + 1;

    vstring_obj = vstr_malloc(needed_len);
    if (vstring_obj == NULL) {
        return NULL;
    }
    memset(vstring_obj, 0, needed_len);
    memcpy(vstring_obj->data, cstr, cap);
    vstring_obj->hdr.len = cap;
    vstring_obj->hdr.cap = cap + 1;
    return vstring_obj;
}

/**
 * @brief allocate a vstring with a specified capacity
 * @param initial_cap the initial capacity
 * @returns vstring, NULL if malloc returns NULL
 */
vstring* vstring_new_len(size_t initial_cap) {
    vstring* vstring_obj;
    size_t tmp_inital_cap = initial_cap;
    size_t needed_len = sizeof(vstring) + initial_cap + 1;

    if (needed_len < tmp_inital_cap) {
        // we have overflowed
        fprintf(stderr, "vstr failed to allocate in vstr_new_len, capacity is too large\n");
        return NULL;
    }

    if (needed_len > SIZE_MAX) {
        fprintf(stderr, "vstr cannot allocate a string larger than size max (vstr_new_len)\n");
        return NULL;
    }

    vstring_obj = vstr_malloc(needed_len);
    if (vstring_obj == NULL) {
        return NULL;
    }

    memset(vstring_obj, 0, needed_len);
    vstring_obj->hdr.len = 0;
    vstring_obj->hdr.cap = initial_cap + 1;

    return vstring_obj;
}

/**
 * @brief add a char to the end of the string
 * @param vstr a pointer to pointer of allocated vstring
 * @param c the char to push
 * @return 0 if success, -1 if realloc is called and failed
 */
int vstring_push_char(vstring** vstring_obj, char c) {
    size_t ins, cap;
    vstring vs;
    vs = **vstring_obj;
    cap = vs.hdr.cap;
    ins = vs.hdr.len;
    if (ins >= (cap - 1)) {
        size_t tmp_cap = cap;
        cap <<= 1; // multiply by two
        cap += ins;
        if ((cap < tmp_cap) || (cap > SIZE_MAX)) {
            // we have overflowed
            fprintf(stderr, "vstr failed to allocate memory, capacity is too large\n");
            vstr_free(*vstring_obj);
            return -1;
        }
        realloc_vstr(vstring_obj, ins, cap);
    }
    (*vstring_obj)->data[ins] = c;
    (*vstring_obj)->hdr.len++;
    return 0;
}

/**
 * @brief concatinate a c string to the end of the string
 * @param vstr a pointer to pointer of allocated vstring
 * @param cstr the c string to push
 * @return 0 if success, -1 if realloc is called and failed
 */
int vstring_push_string(vstring** vstring_obj, const char* cstr) {
    size_t ins, cap, cstr_len, needed;
    vstring vs;
    vs = **vstring_obj;
    ins = vs.hdr.len;
    cap = vs.hdr.cap;
    cstr_len = strlen(cstr);
    needed = ins + cstr_len;
    if (needed >= (cap - 1)) {
        size_t tmp_cap = cap;
        cap <<= 1;
        cap += needed;
        if ((cap < tmp_cap) || (cap > SIZE_MAX)) {
            // we have overflowed
            fprintf(stderr, "vstr failed to allocate memory, capacity is too large\n");
            vstr_free(*vstring_obj);
            return -1;
        }
        realloc_vstr(vstring_obj, ins, cap);
    }
    memcpy((*vstring_obj)->data + ins, cstr, cstr_len);
    (*vstring_obj)->hdr.len += cstr_len;
    return 0;
}

/**
 * @brief concatinate a c string to the end of the string
 * @param vstr a pointer to pointer of allocated vstring
 * @param cstr the c string to push
 * @param cstr_len the length to push
 * @return 0 if success, -1 if realloc is called and failed
 */
int vstring_push_string_len(vstring** vstring_obj, const char* cstr, size_t cstr_len) {
    size_t ins, cap, needed;
    vstring vs;
    vs = **vstring_obj;
    ins = vs.hdr.len;
    cap = vs.hdr.cap;
    needed = ins + cstr_len;
    if (needed >= (cap - 1)) {
        size_t tmp_cap = cap;
        cap <<= 1;
        cap += needed;
        if ((cap < tmp_cap) || (cap > SIZE_MAX)) {
            // we have overflowed
            fprintf(stderr, "vstr failed to allocate memory, capacity is too large\n");
            vstr_free(*vstring_obj);
            return -1;
        }
        realloc_vstr(vstring_obj, ins, cap);
    }
    memcpy((*vstring_obj)->data + ins, cstr, cstr_len);
    (*vstring_obj)->hdr.len += cstr_len;
    return 0;
}

/**
 * @brief clear the current data and set it with cstr
 * @param vstr pointer to pointer of allocated vstring
 * @param cstr the string to set
 * @return 0 on success, -1 if realloc called and failed
 */
int vstring_set(vstring** vstring_obj, const char* cstr) {
    size_t ins, cap, cstr_len, needed;
    vstring vs;
    vs = **vstring_obj;
    ins = vs.hdr.len;
    cap = vs.hdr.cap;
    cstr_len = strlen(cstr);
    needed = ins + cstr_len;
    if (needed >= (cap - 1)) {
        size_t tmp_cap = cap;
        cap <<= 1;
        cap += needed;
        if ((cap < tmp_cap) || (cap > SIZE_MAX)) {
            // we have overflowed
            fprintf(stderr, "vstr failed to allocate memory, capacity is too large\n");
            vstr_free(*vstring_obj);
            return -1;
        }
        realloc_vstr(vstring_obj, ins, cap);
    }
    memset((*vstring_obj)->data, 0, cap);
    memcpy((*vstring_obj)->data, cstr, cstr_len);
    (*vstring_obj)->hdr.len = cstr_len;
    return 0;
}

/**
 * @brief get the length of the string
 * @param vstr the allocated vstring
 * @return the length of the string
 */
size_t vstring_len(vstring* vstring_obj) { return vstring_obj->hdr.len; }

/**
 * @brief get the c string
 * @param vstr the allocated vstring
 * @return the c string
 */
char* vstring_get(vstring* vstring_obj) { return vstring_obj->data; }

/**
 * @brief free the vstring
 * @param vstr the vstring to free
 */
void vstring_free(vstring* vstring_obj) { vstr_free(vstring_obj); }

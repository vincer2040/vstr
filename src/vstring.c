#include "vstring.h"
#include "stdio.h"
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

vstring* vstring_new();
vstring* vstring_from(char* cstr);
vstring* vstring_new_len(size_t initial_cap);
int vstring_push_char(vstring** vstr, char c);
int vstring_push_string(vstring** vstr, char* cstr);
int vstring_set(vstring** vstr, char* cstr);

// can this be optimized depending on the allocator ?
// the only problem is that i think this would cause ifdef hell.. to deal
// with but 32 bit architectures and checking what the allocator is.
#define VSTRING_INITIAL_CAP 32
#define VSTRING_OFFSET sizeof(vstring_hdr)

#define realloc_vstr(vstr, ins, cap)                                           \
    {                                                                          \
        *vstr = vstr_realloc(*vstr, sizeof(vstring) + cap);                    \
        if (*vstr == NULL) {                                                   \
            return -1;                                                         \
        }                                                                      \
        memset((*vstr)->data + ins, 0, cap - ins);                             \
        (*vstr)->hdr.cap = cap;                                                \
    }

/**
 * @brief allocate a new vstr
 * @return pointer to string, NULL if malloc failed
 */
vstr vstr_new() {
    vstring* vstr;

    vstr = vstring_new();
    if (vstr == NULL) {
        return NULL;
    }

    return vstr->data;
}

/**
 * @breif allocate a new vstr from a cstr
 * @param cstr the string to copy - must not be NULL
 * @return pointer to string, NULL if malloc failed
 */
vstr vstr_from(char* cstr) {
    vstring* vstr;

    vstr = vstring_from(cstr);
    if (vstr == NULL) {
        return NULL;
    }

    return vstr->data;
}

/**
 * @brief allocate a new vstr with specified capacity
 * @param initial_cap the initial capacity
 * @returns vstr, NULL if malloc failed
 */
vstr vstr_new_len(size_t initial_cap) {
    vstring* vstring = vstring_new_len(initial_cap);
    if (vstring == NULL) {
        return NULL;
    }
    return vstring->data;
}

/**
 * @brief duplicate a vstr
 * @param str the vstr to copy
 * @returns vstr, NULL if malloc fails
 */
vstr vstr_dup(vstr str) {
    vstring* vs = vstring_from(str);
    if (vs == NULL) {
        return NULL;
    }
    return vs->data;
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
    vstring* vstr;

    va_start(ap, fmt);
    n = vsnprintf(p, size, fmt, ap);
    va_end(ap);

    if (n < 0) {
        return NULL;
    }

    size = ((size_t)n) + 1;
    vstr = vstring_new_len(size);
    if (vstr == NULL) {
        return NULL;
    }

    va_start(ap, fmt);
    n = vsnprintf(vstr->data, size, fmt, ap);
    va_end(ap);

    if (n < 0) {
        free(vstr);
        return NULL;
    }

    return vstr->data;
}

/**
 * @brief set contents of a vstring
 * @param str pointer to vstring
 * @param cstr the string to set vstr to - must not be NULL
 * @return pointer to vstr, NULL if realloc called and failed
 */
vstr vstr_set(vstr str, char* cstr) {
    vstring* vstr = ((vstring*)(str - VSTRING_OFFSET));
    int set_res = vstring_set(&vstr, cstr);
    if (set_res == -1) {
        return NULL;
    }

    return vstr->data;
}

/**
 * @brief append a char to the end of str
 * @param str the allocated vstr
 * @param c the char to push
 * @return pointer to string, NULL if realloc called and failed
 */
vstr vstr_push_char(vstr str, char c) {
    vstring* ptr = ((vstring*)(str - VSTRING_OFFSET));
    int push_res = vstring_push_char(&ptr, c);
    if (push_res == -1) {
        return NULL;
    }
    return ptr->data;
}

/**
 * @brief append a string to the end of str
 * @param str the allocated vstr
 * @param cstr pointer to c string - must not be NULL
 * @return pointer to string, NULL if realloc called and failed
 */
vstr vstr_push_string(vstr str, char* cstr) {
    vstring* vstr = ((vstring*)(str - VSTRING_OFFSET));
    int push_res = vstring_push_string(&vstr, cstr);
    if (push_res == -1) {
        return NULL;
    }
    return vstr->data;
}

/**
 * @brief get length of the vstr
 * @param str pointer to vstr
 * @return length of the string
 */
size_t vstr_len(vstr str) {
    vstring* vstr = ((vstring*)(str - VSTRING_OFFSET));
    return vstr->hdr.len;
}

/**
 * @brief get capacity of the vstr
 * @param str pointer to vstr
 * @return capacity of the vstr
 */
size_t vstr_cap(vstr str) {
    vstring* vstr = ((vstring*)(str - VSTRING_OFFSET));
    return vstr->hdr.cap - 1; // -1 to account for null terminator
}

/**
 * @brief get available space in the vstr
 * @param str pointer to vstr
 * @return available space in vstr
 */
size_t vstr_available(vstr str) {
    vstring* vstr = ((vstring*)(str - VSTRING_OFFSET));
    // cap is always >= len + 1
    // we accomodate for null terminator with - 1
    return vstr->hdr.cap - vstr->hdr.len - 1;
}

/**
 * @brief free vstring
 * @param pointer to vstr to free
 */
void vstr_delete(vstr str) {
    void* ptr = str - VSTRING_OFFSET;
    free(ptr);
}

/**
 * @brief allocate a vstring
 * @return vstring pointer, NULL if malloc returns NULL
 */
vstring* vstring_new() {
    vstring* vstr;
    size_t needed_len = sizeof(vstring) + VSTRING_INITIAL_CAP;

    vstr = vstr_malloc(needed_len);
    if (vstr == NULL) {
        return NULL;
    }

    memset(vstr, 0, needed_len);
    vstr->hdr.len = 0;
    vstr->hdr.cap = VSTRING_INITIAL_CAP;

    return vstr;
}

/**
 * @brief allocate a vstring initialized from a c string
 * @param cstr string to initialize from - should not be NULL
 * @return vstring pointer, NULL if malloc returns NULL
 */
vstring* vstring_from(char* cstr) {
    vstring* vstr;
    size_t cap = strlen(cstr);
    size_t needed_len = sizeof(vstring) + cap + 1;
    vstr = vstr_malloc(needed_len);
    if (vstr == NULL) {
        return NULL;
    }
    memset(vstr, 0, needed_len);
    memcpy(vstr->data, cstr, cap);
    vstr->hdr.len = cap;
    vstr->hdr.cap = cap + 1;
    return vstr;
}

/**
 * @brief allocate a vstring with a specified capacity
 * @param initial_cap the initial capacity
 * @returns vstring, NULL if malloc returns NULL
 */
vstring* vstring_new_len(size_t initial_cap) {
    vstring* vstr;
    size_t needed_len = sizeof(vstring) + initial_cap + 1;

    vstr = vstr_malloc(needed_len);
    if (vstr == NULL) {
        return NULL;
    }

    memset(vstr, 0, needed_len);
    vstr->hdr.len = 0;
    vstr->hdr.cap = initial_cap + 1;

    return vstr;
}

/**
 * @brief add a char to the end of the string
 * @param vstr a pointer to pointer of allocated vstring
 * @param c the char to push
 * @return 0 if success, 1 if realloc is called and failed
 */
int vstring_push_char(vstring** vstr, char c) {
    size_t ins, cap;
    vstring vs;
    vs = **vstr;
    cap = vs.hdr.cap;
    ins = vs.hdr.len;
    if (ins == (cap - 1)) {
        cap <<= 1; // multiply by two
        realloc_vstr(vstr, ins, cap);
    }
    (*vstr)->data[ins] = c;
    (*vstr)->hdr.len++;
    return 0;
}

/**
 * @brief concatinate a c string to the end of the string
 * @param vstr a pointer to pointer of allocated vstring
 * @param cstr the c string to push
 * @return 0 if success, 1 if realloc is called and failed
 */
int vstring_push_string(vstring** vstr, char* cstr) {
    size_t ins, cap, cstr_len, needed;
    vstring vs;
    vs = **vstr;
    ins = vs.hdr.len;
    cap = vs.hdr.cap;
    cstr_len = strlen(cstr);
    needed = ins + cstr_len;
    if (needed > (cap - 1)) {
        cap <<= 1;
        cap += needed;
        realloc_vstr(vstr, ins, cap);
    }
    memcpy((*vstr)->data + ins, cstr, cstr_len);
    (*vstr)->hdr.len += cstr_len;
    return 0;
}

/**
 * @brief clear the current data and set it with cstr
 * @param vstr pointer to pointer of allocated vstring
 * @param cstr the string to set
 * @return 0 on success, -1 if realloc called and failed
 */
int vstring_set(vstring** vstr, char* cstr) {
    size_t ins, cap, cstr_len, needed;
    vstring vs;
    vs = **vstr;
    ins = vs.hdr.len;
    cap = vs.hdr.cap;
    cstr_len = strlen(cstr);
    needed = ins + cstr_len;
    if (needed > (cap - 1)) {
        cap <<= 1;
        cap += needed;
        realloc_vstr(vstr, ins, cap);
    }
    memset((*vstr)->data, 0, cap);
    memcpy((*vstr)->data, cstr, cstr_len);
    (*vstr)->hdr.len = cstr_len;
    return 0;
}

/**
 * @brief get the length of the string
 * @param vstr the allocated vstring
 * @return the length of the string
 */
size_t vstring_len(vstring* vstr) { return vstr->hdr.len; }

/**
 * @brief get the c string
 * @param vstr the allocated vstring
 * @return the c string
 */
char* vstring_get(vstring* vstr) { return vstr->data; }

/**
 * @brief free the vstring
 * @param vstr the vstring to free
 */
void vstring_free(vstring* vstr) { vstr_free(vstr); }

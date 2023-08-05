#include "vstring.h"
#include "stdio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

vstring* vstring_new();
vstring* vstring_from(char* cstr);
int vstring_push_char(vstring** vstr, char c);
int vstring_push_string(vstring** vstr, char* cstr);

#define VSTRING_INITIAL_CAP 32
#define VSTRING_OFFSET sizeof(vstring_hdr)

#define realloc_vstr(vstr, ins, cap)                                           \
    {                                                                          \
        *vstr = realloc(*vstr, sizeof(vstring) + cap);                         \
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
 * @param cstr pointer to c string
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

size_t vstr_len(vstr str) {
    vstring* vstr = ((vstring*)(str - VSTRING_OFFSET));
    return vstr->hdr.len;
}

/**
 * @brief free vstring
 * @param pointer to vstr to free
 */
void vstr_free(vstr str) {
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

    vstr = malloc(needed_len);
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
    vstr = malloc(needed_len);
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
void vstring_free(vstring* vstr) { free(vstr); }

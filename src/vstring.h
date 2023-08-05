#ifndef __VSTRING_H__

#define __VSTRING_H__

#include <stddef.h>
#include <stdint.h>

typedef char* vstr;

typedef struct {
    size_t len;
    size_t cap;
} vstring_hdr;

typedef struct {
    vstring_hdr hdr;
    char data[];
} vstring;

#ifndef vstr_alloc

#include <stdlib.h>
#define vstr_malloc malloc
#define vstr_realloc realloc
#define vstr_free free

#else

#ifndef vstr_malloc
#error you have opted in to change the allocator by defining vstr_alloc. you must define the malloc implementation like so: #define vstr_malloc <malloc implementation>
#endif /* vstr_malloc */

#ifndef vstr_realloc
#error you have opted in to change the allocator by defining vstr_alloc. you must define the realloc implementation like so: #define vstr_realloc <realloc implementation>
#endif /* vstr_realloc */

#ifndef vstr_free
#error you have opted in to change the allocator by defining vstr_alloc. you must define the free implementation like so: #define vstr_free <free implementation>
#endif /* vstr_free */

#endif /* vstr_alloc */

vstr vstr_new();
vstr vstr_from(char* cstr);
vstr vstr_dup(vstr str);
vstr vstr_format(const char* fmt, ...);
vstr vstr_set(vstr str, char* cstr);
vstr vstr_push_char(vstr str, char c);
vstr vstr_push_string(vstr str, char* cstr);
size_t vstr_len(vstr str);
size_t vstr_cap(vstr str);
size_t vstr_available(vstr str);
void vstr_delete(vstr str);

#endif /* __VSTRING_H__ */

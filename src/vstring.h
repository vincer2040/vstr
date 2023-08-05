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

vstr vstr_new();
vstr vstr_from(char* cstr);
vstr vstr_push_char(vstr str, char c);
vstr vstr_push_string(vstr str, char* cstr);
size_t vstr_len(vstr str);
void vstr_free(vstr str);

#endif /* __VSTRING_H__ */

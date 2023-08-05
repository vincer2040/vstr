#ifndef __VSTRING_H__

#define __VSTRING_H__

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t len;
    size_t cap;
    char data[];
} vstring;

vstring* vstring_new();
vstring* vstring_from(char* cstr);
int vstring_push_char(vstring** vstr, char c);
int vstring_push_string(vstring** vstr, char* cstr);
size_t vstring_len(vstring* vstr);
char* vstring_get(vstring* vstr);
void vstring_free(vstring* vstr);

#endif /* __VSTRING_H__ */

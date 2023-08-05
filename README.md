# vstr

a safe string implementation in pure C

## Getting started

### how it works

below is the internal representation of the vstr:

```shell
+---------------------------------+
| Header | string ... | null term |
+---------------------------------+
         |
         -> pointer returned to user
```

The header contains two field: the length of the string, and the capacity of the string.

### using vstr

basic usage:

```c
vstr s = vstr_new();
s = vstr_push_char(s, 'v');
s = vstr_push_char(s, 's');
s = vstr_push_char(s, 't');
s = vstr_push_char(s, 'r');
s = vstr_push_str(s, " is a string implementation");
printf("%s\n", s); // "vstr is a string implementation"
vstr_free(s);
```

Notice that you have to reset `s` when appending chars or char*s. This is because vstr
may have to reallocate the internal data structure to accomodate the new
length.

create a vstr from a C string:

```c
vstr s = vstr_from("vstr");
printf("%s\n", s); // "vstr"
vstr_delete(s);
```

set the vstring to a different C string

```c
vstr s = vstr_from("vstr");
s = vstr_set(s, "a string implementation");
printf("%s\n", s); // "a string implementation"
vstr_delete(s);
```

get the length of a vstr:

```c
vstr s = vstr_from("vstr");
size_t len = vstr_len(s);
printf("%lu\n", len); // "4"
vstr_delete(s);
```

### specify an allocator:

```c
#define vstr_alloc
#define vstr_malloc <malloc implementation>
#define vstr_realloc <realloc implementation>
#define vstr_free <free implementation>
#include "vstr.h"
```

ensure that the allocating and deallocating implementations are defined before
including the vstr header file

## Limitations

Notice above how the user must reassign the vstr when appending to it. This is required because
vstr might have to reallocate memory for its internal data structure to accomodate the new
length. Because of this, if you have mulitple references to the same vstr, they all must be reassigned
when appending a char or string.

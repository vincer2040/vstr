# vstr

A safe string implementation in pure C. Originally created for
an interpreter following the incredible book "Writing an Interpreter In Go"
by Thorsten Ball (you can find the book [here](https://interpreterbook.com/), highly recommend)

## Getting started

The easiest way to get started is to copy the two files vstring.c and vstring.h into
your project and compile them with your binary.

You can also install the library. See [Installing](#installing)

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
All of the data for a vstr is contained in the same memory allocation, so it is good
for cache locality.

### using vstr

basic usage:

```c
vstr s = vstr_new();
s = vstr_push_char(s, 'v');
s = vstr_push_char(s, 's');
s = vstr_push_char(s, 't');
s = vstr_push_char(s, 'r');
s = vstr_push_string(s, " is a string implementation");
printf("%s\n", s); // "vstr is a string implementation"
vstr_delete(s);
```

Notice that you have to reset `s` when appending chars or char*s. This is because vstr
may have to reallocate the internal data structure to accomodate the new
length.

destroy the vstr:

```c
vstr_delete(vstr);
```

create a vstr from a C string:

```c
vstr s = vstr_from("vstr");
printf("%s\n", s); // "vstr"
```

set the vstring to a different C string

```c
vstr s = vstr_from("vstr");
s = vstr_set(s, "a string implementation");
printf("%s\n", s); // "a string implementation"
```

duplicate a vstr (creates a seperate memory allocation):

```c
vstr s1 = vstr_from("vstr");
vstr s2 = vstr_dup(s1);
assert(strcmp(s1, s2) == 0);
```

specify an initial capacity
(one will be added to ensure null terminator is accounted for):

```c
vstr s1 = vstr_new_len(32);
assert(vstr_cap(s1) == 33);
assert(vstr_available(s1) == 32);
```

create a vstr from a formatted string:

```c
vstr s = vstr_format("he was number %d", 1);
printf("%s\n", s); // "he was number 1"
```

useful utility functions:

```c
vstr s = vstr_from("vstr");
size_t len = vstr_len(s); // the length of the vstr
size_t cap = vstr_cap(s); // the capacity of the vstr
size_t avail = vstr_available(s); // the available space in the vstr
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

# Installing

## Requirements

1. gcc
2. cmake

if you want to test the library, [libcheck](https://github.com/libcheck/check) is required.

## installing vstr

1. clone this repo

```console
git clone git@github.com:vincer2040/vstr.git && cd vstr
```

2. build the library

from within the vstr directory:

```console
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

3. run tests (optional, must have [libcheck](https://github.com/libcheck/check) installed)

from within the vstr/build directory:

```console
make test
```

4. install

```console
sudo make install
```

the default install path is /usr/local/lib

# Limitations

Notice above how the user must reassign the vstr when appending to it. This is required because
vstr might have to reallocate memory for its internal data structure to accomodate the new
length. Because of this, if you have mulitple references to the same vstr, they all must be reassigned
when appending a char or string.

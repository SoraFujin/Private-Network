#ifdef MEMORY_UTILS
#define MEMORY_UTILS

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void *xmalloc(size_t size);
void *xcalloc(size_t count, size_t size);
void *xrealloc(void *ptr, size_t new_size);
void xfree(void *ptr);

#endif

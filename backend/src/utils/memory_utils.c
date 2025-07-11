#include "include/memory_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>



void *xmalloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "[FATAL] malloc failed for %zu bytes\n", size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xcalloc(size_t count, size_t size) {
    void *p = calloc(count, size);
    if (!p) {
        fprintf(stderr, "[FATAL] calloc failed for %zu x %zu bytes\n", count, size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void *xrealloc(void *ptr, size_t new_size) {
    void *p = realloc(ptr, new_size);
    if (!p) {
        fprintf(stderr, "[FATAL] realloc failed for %zu bytes\n", new_size);
        exit(EXIT_FAILURE);
    }
    return p;
}

void xfree(void *ptr) {
    free(ptr);
}

char *xstrdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *copy = (char *)xmalloc(len);
    memcpy(copy, s, len);
    return copy;
}

void *xmemdup(const void *data, size_t size) {
    if (!data || size == 0) return NULL;
    void *copy = xmalloc(size);
    memcpy(copy, data, size);
    return copy;
}


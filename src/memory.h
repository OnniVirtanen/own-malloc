#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

extern void *heap_allocate(size_t size);

extern void heap_free(void *ptr);

// Used for testing purposes
extern void reset();

extern struct {
    size_t size;
    int free;
    void *next;
} heap_block_t;

extern struct heap_block_t *free_list;

#endif

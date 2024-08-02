#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

extern void *heap_allocate(size_t size);
extern void heap_free(void *ptr);

#endif

#ifndef MEMORY_H
#define MEMORY_H

void *heap_allocate(size_t size);
void heap_free(void *ptr);

#endif
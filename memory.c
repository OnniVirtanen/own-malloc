#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>

#define HEAP_SHIFT(start) ((void *)start + sizeof(heap_t))
#define BLOCK_SHIFT(start) ((void *)start + sizeof(heap_block_t))

#define TINY_HEAP_ALLOCATION_SIZE (4 * getpagesize())
#define TINY_BLOCK_SIZE (TINY_HEAP_ALLOCATION_SIZE / 128)
#define SMALL_HEAP_ALLOCATION_SIZE (16 * getpagesize())
#define SMALL_BLOCK_SIZE (SMALL_HEAP_ALLOCATION_SIZE / 128)

typedef struct {
    struct heap_block_t *start;
    size_t available_size;
} heap_t;

typedef struct {
    size_t size;
    bool used;
    void *next;
    void *previous;
} heap_block_t ;

void *heap_allocate(size_t size) {
    if (size <= 0) {
        return NULL;
    }
    
    void *heap = (heap_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    return heap;
}


void heap_free(void *ptr) {
    if (!ptr) {
        return;
    }

    
}

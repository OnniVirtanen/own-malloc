#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/mman.h>

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
    printf("size: %ld\n", size);
    void *heap = (heap_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    return heap;
}


void heap_free(void *ptr) {
    if (!ptr) {
        return;
    }

    
}

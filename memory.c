#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>

#define FALSE 0
#define TRUE 1

typedef struct {
    size_t size;
    int free;
    void *next;
} heap_block_t;

heap_block_t *free_list = NULL;

heap_block_t *find_free_block(size_t size) {
    heap_block_t *current = free_list;
    while (current) {
        if (current->free && current->size >= size) {
	    return current;
	}
	current = current->next;
    }
    return NULL;
}

heap_block_t *request_space(size_t size) {
    size_t total_size = size + sizeof(heap_block_t);

    if (total_size % getpagesize() != 0) {
        total_size += getpagesize() - (total_size % getpagesize());
    }

    printf("size: %ld\n", size);
    heap_block_t *block = (heap_block_t *)mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    if (block == MAP_FAILED) {
        return NULL;
    }

    block->size = total_size - sizeof(heap_block_t);
    block->next = NULL;
    block->free = FALSE;


    printf("block->size: %ld\n", block->size);

    return block;
}

void *heap_allocate(size_t size) {
    if (size <= 0) {
        return NULL;
    }
    
    heap_block_t *block = find_free_block(size);
    if (block) {
        block->free = FALSE;
	return (block + 1);
    }

    block = request_space(size);
    if (!block) {
        return NULL;
    }

    if (free_list) {
        block->next = free_list;
    }
    free_list = block;

    return (block + 1);
}


void heap_free(void *ptr) {
    if (!ptr) {
        return;
    }
    heap_block_t *heap_block = (heap_block_t *)ptr - 1;
    heap_block->free = TRUE;
}

#include <stdio.h>
#include <stddef.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdbool.h>
#include <unistd.h> 

typedef struct {
    size_t size;
    bool free;
    void *next;
} heap_block_t;

heap_block_t *free_list = NULL;
heap_block_t *inuse_list = NULL;

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
#ifdef DEBUG
    printf("free_list is null at %d %d\n", __LINE__, free_list == NULL);
#endif
    size_t total_size = size + sizeof(heap_block_t);

    if (total_size % getpagesize() != 0) {
        total_size += getpagesize() - (total_size % getpagesize());
    }
#ifdef DEBUG
    printf("size: %ld\n", size);
    printf("total size: %ld\n", total_size);
#endif
    heap_block_t *block = (heap_block_t *)mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);

    if (block == MAP_FAILED) {
        return NULL;
    }

    // Append block to inuse list
    if (inuse_list == NULL) {
        inuse_list = block;
    } else {
        // increment inuse list
        heap_block_t *current = inuse_list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = block;
        block->next = NULL;
    }

    heap_block_t *free_block = NULL;

    // Make a block that has size that user asked
    bool less_memory_than_pagesize = (size + sizeof(heap_block_t)) < total_size;
    if (less_memory_than_pagesize) {
        // Make two blocks
        block->size = size + sizeof(heap_block_t);
        block->next = NULL;
        block->free = false;

        // Free block starts where the used block ends
        free_block = block + block->size;
        free_block->size = total_size - block->size;
        free_block->next = NULL;
        free_block->free = true;

        // Free list is empty
        if (free_list == NULL) {
            free_list = free_block;
        } else {
            // Loop to the end of free list
            heap_block_t *current = free_list;
            while (current->next != NULL) {
                current = current->next;
            }
            // Append free block to the free list
            current->next = free_block;
        }

    } else {
        // Make one block containing all the memory
        block->size = total_size - sizeof(heap_block_t);
        block->next = NULL;
        block->free = false;
    }
#ifdef DEBUG
    printf("block->size: %ld\n", block->size);
    if (free_block != NULL) {
        printf("free block->size: %ld\n", free_block->size);
    }

    printf("free_list %p\n", free_list);
#endif
    return block;
}

void *heap_allocate(size_t size) {
    if (size <= 0) {
        return NULL;
    }
    
    heap_block_t *block = find_free_block(size);
    if (block) {
        block->free = false;
	    return (block + 1);
    }

    block = request_space(size);
    if (!block) {
        return NULL;
    }

    return (block + 1);
}


void heap_free(void *ptr) {
    if (!ptr) {
        return;
    }
    
    heap_block_t *block = (heap_block_t *)ptr - sizeof(heap_block_t);
    heap_block_t *current = inuse_list;
    heap_block_t *previous = NULL;

    // Traverse through the list and pop the block out and update linked list
    while (current->next != NULL) {
        if (current->next == block) {
            if (block->next != NULL) {
                previous->next = current->next;
            } else {
                previous->next = NULL;
            }
            break;
        } else {
            previous = current;
            current = current->next;
        }
    }

    // Handle munmap
    munmap(block, sizeof(heap_block_t) + block->size);

    // Handle defragmentation?
}

void munmap_list_of_memory(heap_block_t *list_of_memory) {
    if (list_of_memory == NULL) {
#ifdef DEBUG
    printf("list_of_memory is null at line %d\n", __LINE__);
#endif
        return;
    }
    // unmap memory of every block in the list
    while (list_of_memory->next != NULL) {
        munmap(list_of_memory, sizeof(heap_block_t) + list_of_memory->size);
        list_of_memory = list_of_memory->next;
    }
#ifdef DEBUG
    assert(list_of_memory != NULL);
    assert(list_of_memory->next == NULL);
#endif
}

void reset() {
    munmap_list_of_memory(free_list);
    munmap_list_of_memory(inuse_list);
    free_list = NULL;
    inuse_list = NULL;
}
#include <stdio.h>
#include <stdbool.h>

struct heap_meta {
    struct heap_chunk_meta *start;
    size_t available_size;
};

struct heap_chunk_meta {
    size_t size;
    bool used;
    void *next;
};

void *heap_allocate(size_t size) {
    if (size <= 0) {
        return NULL;
    }


}


void heap_free(void *ptr) {
    if (!ptr) {
        return;
    }

    
}
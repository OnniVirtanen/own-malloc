#include <stdio.h>
#include <assert.h>
#include "../src/memory.h"
#include "test.h"
#include <sys/mman.h>

void test_pointer_is_created();
void test_less_than_pagesize_free_list_assigned();
void test_more_requested_than_pagesize_free_list_null();

int main() {
    run_test(*test_pointer_is_created, "pointer_is_created");
    run_test(*test_less_than_pagesize_free_list_assigned, "less_than_pagesize_free_list_assigned");
    run_test(*test_more_requested_than_pagesize_free_list_null, "more_requested_than_pagesize_free_list_null");
    return 0;
}

void test_pointer_is_created() {
    int *p_int = (int *)heap_allocate(sizeof(int));
    assert(p_int != NULL);
    heap_free(p_int);
    reset();
}

void test_less_than_pagesize_free_list_assigned() {
    int *p_int = (int *)heap_allocate(sizeof(int));
    assert(free_list != NULL);
    heap_free(p_int);
    reset();
}

void test_more_requested_than_pagesize_free_list_null() {
    int *p_int = (int *)heap_allocate(getpagesize() - sizeof(heap_block_t));
    assert(free_list == NULL);
    heap_free(p_int);
    reset();
}
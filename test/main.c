#include <stdio.h>
#include <assert.h>
#include "../src/memory.h"

void run_test(int (*function)(void), char *name);
int test_pointer_is_created();

int main() {
    run_test(*test_pointer_is_created, "test_pointer_is_created");

    return 0;
}

void run_test(int (*function)(void), char *name) {
    printf("-- Start of test %s --\n", name);
    function();
    printf("-- End of test %s --\n", name);
}

int test_pointer_is_created() {
    int *p_int = (int *)heap_allocate(sizeof(int));
    
    assert(p_int != NULL);
    heap_free(p_int);

    return 0;
}
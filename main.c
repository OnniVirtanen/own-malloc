#include <stdio.h>
#include "memory.h"

int main() {
    int *p_int = (int *)heap_allocate(4);
    *p_int = 42;
    printf("dereferenced p_int: %d\n", *p_int);
    printf("pointer p_int: %p\n", p_int);
    printf("size of p_int: %ld\n", sizeof(p_int));
    return 0;
}

#include <stdio.h>

void run_test(void (*function)(void), char *name) {
    printf("\n-- Start of test %s --\n", name);
    function();
    printf("-- Test run successfully --\n");
}
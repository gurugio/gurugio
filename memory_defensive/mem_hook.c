#include <stdio.h>
#include <stdlib.h>
#include <mcheck.h> // for mtrace()


void test_mtrace(void)
{
    printf("HERE! %s:%d\n", __func__, __LINE__);
    malloc(10);
}

int main(void)
{
    mtrace();

    void *p = malloc(10);
    
    printf("HERE! %s:%d\n", __func__, __LINE__);
    malloc(20);
    free(p);

    test_mtrace();
    muntrace();
    
    printf("set environment variable \"MALLOC_TRACE\"\n");
    printf("and executes \"mtrace mem_hook $MALLOC_TRACE\"\n");

    return 0;
}

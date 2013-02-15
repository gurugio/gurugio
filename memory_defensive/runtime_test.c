#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    int data;
    int *p;

    /* twice free
    p = malloc(100);
    if (p != NULL)
        free(p);
    free(p);
    */

    /* invalid pointer, middle of allocated memory
    free(p+50);
    */

    /* invalid pointer, stack or static data
    p = &data;
    free(p);
    */

    return 0;
}

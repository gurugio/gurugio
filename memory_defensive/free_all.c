#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *myname = NULL;

/* cleanup_fns section can be existed
 * only if compiled with -g option
 */
asm (".section cleanup_fns, \"a\"");
static void free_myname(void);
static void (*const fn_free_myname)(void) __attribute((unused,section("cleanup_fns"))) = &free_myname;

static void free_myname(void)
{
    printf("FREE <%s>\n", myname);
    free(myname);
    myname = NULL;
}


/* Magic symbols, GNU-linker declares these pointers */
extern void (*const __start_cleanup_fns)(void);
extern void (*const __stop_cleanup_fns)(void);

void run_cleanup(void)
{
    void (*const *f)(void) = &__start_cleanup_fns;
    printf("__start_cleanup_fns->%p\n", &__start_cleanup_fns);
    printf("__stop_cleanup_fns->%p\n", &__stop_cleanup_fns);
    while (f < &__stop_cleanup_fns)
    {
        printf("f->%p\n", f);
        (*f++)();
    }
}

int main(void)
{
    myname = malloc(100);
    strncpy(myname, "blabla", 6);
    myname[6] = '\0';

    printf("fn_free_myname->%p\n", fn_free_myname);
    run_cleanup();

    printf("PROGRAM ENDS\n");
    return 0;
}

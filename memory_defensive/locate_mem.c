
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * refer to "Defensive Programming for Red Hat Enterprise Linux,
 * Ulrich Drepper.
 */

#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

#define debug_malloc(sz)                                           \
  ({ char __line[strlen(__FILE__) + 6 * sizeof(size_t) + 3]; \
     size_t __sz = sz; \
     int __n = sprintf(__line, "%c%s:%d:%d", \
                       '\0', __FILE__, __LINE__, __sz); \
     size_t __pre = roundup(__n, 2 * sizeof(size_t)); \
     char *__p = malloc(__sz + __pre + __n); \
     printf("n=%d pre=%d\n", __n, __pre); \
     printf("MALLOC->%p\n", __p); \
     printf("LINE=%s\n", &__line[1]); \
     if (__p != NULL) \
       { \
         memset(memcpy(__p, __line, __n)+__n, ' ', __pre - __n); \
         __p[__pre - 1] = '\0'; \
         memcpy(__p + __pre + __sz, __line, __n); \
       } \
     (void *) (__p + __pre); })

#define debug_free(p) \
  (void) ({ char *__p = (char *) (p) - 1; \
            while (*--__p != '\0'); \
            printf("FREE->%p\n", __p); \
            free(__p); })

#define get_preinfo(p) \
    ({ char *__p = (char *) (p) - 1;            \
        while (*--__p != '\0');                 \
        (__p + 1); })

#define get_postinfo(p) \
    ({ char *__p = (char *)(p);                 \
        while (*__p++ != '\0');                 \
        (__p); })

int main(void)
{
    char data[] = "THIS-IS-DATA";
    char *p = debug_malloc(strlen(data));
    char *q;
    int i;

    memcpy(p, data, strlen(data));

    printf("USER DATA:%p-->%s\n", p, p);

    printf("PRE DATA:\n");
    q = p - 1;
    printf("%p-->%X\n", q, *q);
    while (*--q != '\0')
    {
        printf("%p-->%X(%c)\n", q, *q, *q);
    }
    printf("%p-->%X\n", q, *q);

    printf("POST DATA:\n");
    q = p + strlen(p);
    while (*++q != '\0')
    {
        printf("%p-->%X(%c)\n", q, *q, *q);
    }

    printf("MEMORY's INFO:%s\n", get_preinfo(p));
    printf("MEMORY's INFO:%s\n", get_postinfo(p));
    
    debug_free(p);

    return 0;
}


#include <stdio.h>
#include <assert.h>


struct descriptor
{
    struct descriptor *free;
    struct descriptor *link;
    const void *ptr;
    long size;
    const char *file;
    int line;
} *htab[2048];


struct descriptor freelist = { &freelist };

#define hash(p,t) (((unsigned long)(p)>>3) & (sizeof(t) / sizeof((t)[0])-1))
#define NDESCRIPTORS 512
#define NALLOC (((4096 + sizeof(union align) - 1) / (sizeof(union align))) * (sizeof(union align)))


union align
{
    int i;
    long l;
    long *lp;
    void *p;
    void (*fp)(void);
    float f;
    double d;
    long double ld;
};


struct descriptor *find(const void *ptr)
{
    struct descriptor *bp = htab[hash(ptr, htab)];

    while (bp && bp->ptr != ptr)
        bp = bp->link;
    return bp;
}


void mem_free(void *ptr, const char *file, int line)
{
    if (ptr)
    {
        struct descriptor *bp;

        if (((unsigned long)ptr) % (sizeof(union align)) != 0 ||
            (bp = find(ptr)) == NULL ||
            bp->free)
        {
            printf("ERROR TO FREE at %s:%d\n", file, line);
            exit(1);
        }
        bp->free = freelist.free;
        freelist.free = bp;
    }
}

struct descriptor *dalloc(void *ptr, long size, const char *file, int line)
{
    static struct descriptor *avail;
    static int nleft;

    if (nleft <= 0)
    {
        avail = malloc(NDESCRIPTORS*sizeof(*avail));
        if (avail == NULL) return NULL;
        nleft = NDESCRIPTORS;
    }
    avail->ptr = ptr;
    avail->size = size;
    avail->file = file;
    avail->line = line;
    avail->free = avail->link = NULL;
    nleft--;
    return avail++;
}

void *mem_alloc(long nbytes, const char *file, int line)
{
    struct descriptor *bp;
    void *ptr;

    assert(nbytes > 0);

    nbytes = ((nbytes + sizeof(union align) - 1)/(sizeof(union align)))*sizeof(union align);

    for (bp = freelist.free; bp; bp = bp->free)
    {
        if (bp->size > nbytes)
        {
            bp->size -= nbytes;
            ptr = (char *)bp->ptr + bp->size;
            if ((bp = dalloc(ptr, nbytes, file, line)) != NULL)
            {
                unsigned h = hash(ptr, htab);
                bp->link = htab[h];
                htab[h] = bp;
                return ptr;
            }
            else
            {
                printf("FAIL OF ALLOC #1 at %s:%d\n", file, line);
                exit(2);
            }
        }
        if (bp == &freelist)
        {
            struct descriptor *newptr;
            if ((ptr = malloc(nbytes + NALLOC)) == NULL ||
                (newptr = dalloc(ptr, nbytes + NALLOC, __FILE__, __LINE__)) == NULL)
            {
                printf("FAIL OF ALLOC #! at %s:%d\n", file, line);
                exit(3);
            }
            newptr->free = freelist.free;
            freelist.free = newptr;
        }
    }
    assert(0);
    return NULL;
}


int main(void)
{

    /* add tests */

    return 0;
}

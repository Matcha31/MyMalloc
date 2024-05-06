#include "./tools.h"

#define _GNU_SOURCE

size_t align(size_t size)
{
    int s = sizeof(long double);
    if (!(size % s))
        return size;
    size_t j;
    if (__builtin_add_overflow(size, s, &j))
        return 0;
    return (j / s) * s;
}

size_t page_size_is(void)
{
    static size_t page_size = 0;
    if (!page_size)
        page_size = sysconf(_SC_PAGE_SIZE);
    return page_size;
}

size_t pow_of_two(size_t n)
{
    if (n <= 16)
        return 16;
    size_t p = 1;
    while (p <= n)
        p <<= 1;
    return p;
}

void *add_to_vp(void *ptr, size_t i)
{
    char *added = ptr;
    added += i;
    return added;
}

void *page_begin(void *ptr, size_t page_size)
{
    if (!page_size || !ptr)
        return NULL;
    size_t add = (size_t)ptr;
    size_t k = add - (add % page_size);
    size_t offset = add - k;
    char *page_start = ptr;
    return page_start - offset;
}

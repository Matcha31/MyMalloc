#include "./my_malloc.h"

#include <stdio.h>

#include "buckets/bucket.h"
#include "tools.h"

struct bucket *g_head = NULL;

size_t num_block(struct bucket *bucket, void *ptr)
{
    unsigned char *start = bucket->free_list + bucket->free_size;
    unsigned char *p = ptr;
    return (p - start) / bucket->block_size;
}

void unmap_if_free(struct bucket *bucket)
{
    for (size_t i = 0; i < bucket->free_size; i++)
    {
        if (bucket->free_list[i] != 255)
        {
            return;
        }
    }
    if (bucket == g_head)
    {
        g_head = bucket->next;
    }
    else
    {
        struct bucket *prev = g_head;
        while (prev->next != bucket)
            prev = prev->next;
        prev->next = bucket->next;
    }
    size_t page_size = page_size_is();
    if (munmap(bucket, page_size))
        errx(1, "could not munmap\n");
}

void my_free(void *ptr)
{
    if (!ptr)
        return;
    struct bucket *bucket = page_begin(ptr, page_size_is());
    size_t n = num_block(bucket, ptr);
    size_t j = 1 << (7 - (n % 8));
    bucket->free_list[n / 8] += j;
    unmap_if_free(bucket);
    return;
}

void *my_malloc(size_t size)
{
    if (!size)
        return NULL;
    if (size > (page_size_is() - align(sizeof(struct bucket) + 1)))
    {
        struct bucket *bucket = greater_then_page_size(size);
        return alloc_bucket(bucket);
    }
    size_t block_size = pow_of_two(size);
    struct bucket *bucket = find_bucket(block_size);
    if (!bucket)
    {
        bucket = create_bucket(block_size);
    }
    return alloc_bucket(bucket);
}

void *my_realloc(void *ptr, size_t size)
{
    if (!ptr)
        return my_malloc(size);
    if (!size)
    {
        my_free(ptr);
        return NULL;
    }
    struct bucket *b = page_begin(ptr, page_size_is());
    if (b->block_size >= size)
        return ptr;
    void *tmp = my_malloc(size);
    memcpy(tmp, ptr, size);
    my_free(ptr);
    return tmp;
}

void *my_calloc(size_t nmemb, size_t size)
{
    size_t total = nmemb * size;
    struct bucket *old = g_head;
    void *ptr = my_malloc(total);
    if (g_head == old)
        memset(ptr, 0, nmemb * size);
    return ptr;
}

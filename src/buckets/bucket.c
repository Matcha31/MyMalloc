#include "bucket.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "../tools.h"

extern struct bucket *g_head;

size_t sum_padd(size_t padding)
{
    if (!padding)
        return 0;
    size_t n = 1;
    for (size_t i = 0; i < padding - 1; i++)
    {
        n <<= 1;
        n += 1;
    }
    return n;
}

struct bucket *greater_then_page_size(size_t size)
{
    size_t page_size = page_size_is();
    while (page_size < (size + 16 + sizeof(struct bucket)))
        page_size += page_size_is();
    struct bucket *bucket = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    bucket->block_size = size;
    bucket->free_size = 16;
    bucket->nb_block = 1;
    memset(bucket->free_list, 255, 16);
    struct bucket *tmp = g_head;
    g_head = bucket;
    bucket->next = tmp;
    return bucket;
}

size_t get_nb_block(size_t size)
{
    int space = page_size_is();
    space -= sizeof(struct bucket);
    size_t n = 0;
    while (space >= 0)
    {
        if (!(n % 8))
            space--;
        space -= size;
        n++;
    }
    return n;
}

size_t get_free_size(size_t nb_block)
{
    size_t n = sizeof(long double);
    while (n * 8 < nb_block)
        n += sizeof(long double);
    return n;
}

struct bucket *create_bucket(size_t size)
{
    if (!size)
        return NULL;
    size_t page_size = page_size_is();
    struct bucket *bucket = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    bucket->block_size = size;
    bucket->nb_block = get_nb_block(size);
    bucket->free_size = get_free_size(bucket->nb_block);
    for (size_t i = 0; i < bucket->free_size; i++)
    {
        bucket->free_list[i] = 255;
    }
    struct bucket *tmp = g_head;
    g_head = bucket;
    bucket->next = tmp;
    return bucket;
}

size_t num_free_list(struct bucket *bucket)
{
    size_t i = 0;
    while (!bucket->free_list[i])
    {
        i++;
    }
    size_t j = 0;
    char c = bucket->free_list[i];
    while (!(c & 128))
    {
        c <<= 1;
        j++;
    }
    bucket->free_list[i] -= (1 << (7 - j));
    return i * 8 + j;
}

void *alloc_bucket(struct bucket *bucket)
{
    void *tmp = bucket->free_list + bucket->free_size;
    size_t nth_block = num_free_list(bucket);
    void *ptr = add_to_vp(tmp, nth_block * bucket->block_size);
    return ptr;
}

int is_full(struct bucket *bucket)
{
    for (size_t i = 0; i < bucket->nb_block / 8; i++)
    {
        if (bucket->free_list[i] != 0)
        {
            return 0;
        }
    }
    if (bucket->nb_block % 8)
    {
        size_t p = 255;
        for (size_t i = 0; i < (bucket->nb_block % 8); i++)
            p >>= 1; // correct
        return bucket->free_list[bucket->nb_block / 8] == p;
    }
    return 1;
}

struct bucket *find_bucket(size_t size)
{
    if (size >= page_size_is())
        return NULL;
    struct bucket *current = g_head;
    while (current)
    {
        if (current->block_size == size && !is_full(current))
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

#ifndef BUCKET_H
#define BUCKET_H

#include "../header.h"

struct bucket
{
    struct bucket *next;
    size_t block_size;
    size_t nb_block;
    size_t free_size;
    unsigned char free_list[];
};

struct bucket *create_bucket(size_t size);
struct bucket *greater_then_page_size(size_t size);
void *alloc_bucket(struct bucket *bucket);
struct bucket *find_bucket(size_t size);
size_t sum_padd(size_t padding);

#endif /* !BUCKET_H */

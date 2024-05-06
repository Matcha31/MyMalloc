#ifndef MY_MALLOC_H
#define MY_MALLOC_H

#include "./header.h"
#include "./tools.h"

size_t num_block(struct bucket *bucket, void *ptr);
void unmap_if_free(struct bucket *bucket);

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_realloc(void *ptr, size_t size);
void *my_calloc(size_t nmemb, size_t size);

#endif /* !MY_MALLOC_H */

#ifndef TOOLS_H
#define TOOLS_H

#include "./buckets/bucket.h"
#include "./header.h"

size_t align(size_t size);
size_t page_size_is(void);
size_t pow_of_two(size_t n);
void *add_to_vp(void *ptr, size_t i);
void *page_begin(void *ptr, size_t page_size);

#endif /* ! TOOLS_H */

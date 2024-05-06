// #include <stdio.h>

#include "../src/buckets/bucket.h"
#include "../src/my_malloc.h"

extern struct bucket *g_head;

int main(void)
{
    void *ptr1 = my_malloc(16);
    // printf("1 malloced %p\n\n", ptr1);
    void *ptr2 = my_malloc(1000);
    // printf("2 malloced %p\n\n", ptr2);
    void *ptr3 = my_malloc(1000);
    // printf("3 malloced %p\n\n", ptr3);
    void *ptr4 = my_malloc(1000);
    // printf("4 malloced %p\n\n", ptr4);
    void *ptr5 = my_malloc(1000);
    // printf("5 malloced %p\n\n", ptr5);
    void *ptr6 = my_malloc(1000);
    // printf("6 malloced %p\n\n", ptr6);
    void *ptr7 = my_malloc(1000);
    // printf("7 malloced %p\n\n", ptr7);
    my_free(ptr1);
    my_free(ptr2);
    my_free(ptr3);
    my_free(ptr4);
    my_free(ptr5);
    my_free(ptr6);
    my_free(ptr7);
}

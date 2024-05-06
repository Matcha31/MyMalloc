#include <criterion/criterion.h>

#include "../src/my_malloc.h"

TestSuite(MymallocTest, .timeout = 3);

Test(MymallocTest, Malloc)
{
    void *ptr1 = my_malloc(56);
    my_free(ptr1);
}

/*
 * Test code for binary heap. See binary_heap.c for more info.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "binary_heap.h"

/*
 * Print a heap element.
 */
static void bheap_test_print (bheap *h)
{
    bheap_idx idx;

    assert(h->printer);

    printf("(%-2u in use):", h->in_use);

    for (idx = 0; idx < h->in_use; idx++) {
        (h->printer)(&h->data[idx]);
    }

    printf("\n");
}

/*
 * Compare two keys for equality.
 */
static unsigned int bheap_compare_sort_keys (const bheap_data *a, 
                                             const bheap_data *b)
{
    return (a->sort_key <= b->sort_key);
}

/*
 * Print the contents of the key for debugging.
 */
static void bheap_print_sort_key (const bheap_data *a)
{
    printf("%-2u ", a->sort_key);
}

/*
 * Some random data we want to store with each key.
 */
static unsigned int bheap_test_user_data = 42;

/*
 * Fill the bheap with data.
 */
static bheap *bheap_test_fill (bheap *h, bheap_idx nelements)
{
    bheap_data data;

    while (nelements--) {
        memset(&data, 0, sizeof(data));

        data.sort_key = rand() % 100;
        data.user_data = bheap_test_user_data + data.sort_key;
        data.user_junk = bheap_test_user_data;

        h = bheap_insert(h, &data);

        /*
         * Print the bheap.
         */
        printf("inserted ");
        bheap_print_sort_key(&data);
        bheap_test_print(h);
    }

    return (h);
}

/*
 * Empty the bheap in sorted order.
 */
static void bheap_test_empty (bheap *h)
{
    unsigned int last_value = 0;
    bheap_data data;

    while (!bheap_empty(h)) {
        data = bheap_pop(h);

        /*
         * Print the bheap.
         */
        printf("popped   ");
        bheap_print_sort_key(&data);
        bheap_test_print(h);

        /*
         * Check what we popped is sorted and matches our user data.
         */
        assert(data.sort_key >= last_value);
        assert(data.user_data == bheap_test_user_data + data.sort_key);
        assert(data.user_junk == bheap_test_user_data);
        last_value = data.sort_key;
    }
}

int main (int argc, char *argv[])
{
    bheap *h;

    h = bheap_malloc(10 /* elements */, 
                    bheap_compare_sort_keys, 
                    bheap_print_sort_key);

    h = bheap_test_fill(h, 20);

    bheap_test_empty(h);

    bheap_free(h);

    return (0);
}

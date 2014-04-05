/*
 * Test code for binary heap. See binary_heap.c for more info.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "binary_heap.h"

static void heap_test_print (heap *h)
{
    heap_idx idx;

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
static unsigned int heap_compare_sort_keys (const heap_data *a, 
                                            const heap_data *b)
{
    return (a->sort_key <= b->sort_key);
}

/*
 * Print the contents of the key for debugging.
 */
static void heap_print_sort_key (const heap_data *a)
{
    printf("%-2u ", a->sort_key);
}

/*
 * Some random data we want to store with each key.
 */
static unsigned int heap_test_user_data = 42;

/*
 * Fill the heap with data.
 */
static void heap_test_fill (heap *h, heap_idx nelements)
{
    heap_data data;

    while (nelements--) {
        memset(&data, 0, sizeof(data));

        data.sort_key = rand() % 100;
        data.user_data = heap_test_user_data + data.sort_key;
        data.user_junk = heap_test_user_data;

        heap_insert(h, &data);

        /*
         * Print the heap.
         */
        printf("inserted ");
        heap_print_sort_key(&data);
        heap_test_print(h);
    }
}

/*
 * Empty the heap in sorted order.
 */
static void heap_test_empty (heap *h)
{
    unsigned int last_value = 0;
    heap_data data;

    while (!heap_empty(h)) {
        data = heap_pop(h);

        /*
         * Print the heap.
         */
        printf("popped   ");
        heap_print_sort_key(&data);
        heap_test_print(h);

        /*
         * Check what we popped is sorted and matches our user data.
         */
        assert(data.sort_key >= last_value);
        assert(data.user_data == heap_test_user_data + data.sort_key);
        assert(data.user_junk == heap_test_user_data);
        last_value = data.sort_key;
    }
}

int main (int argc, char *argv[])
{
    heap *h;

    h = heap_malloc(10 /* elements */, 
                    heap_compare_sort_keys, 
                    heap_print_sort_key);

    heap_test_fill(h, 20);

    heap_test_empty(h);

    heap_free(h);

    return (0);
}

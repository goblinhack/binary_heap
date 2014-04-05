/*
 * Example implementation of a binary heap - useful in A * star searches
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef unsigned int heap_idx;

typedef struct {
    unsigned int sort_key;
    unsigned int user_data;
    unsigned char user_junk;
} heap_data;

typedef unsigned int (*heap_less_than_func)(const heap_data *, 
                                            const heap_data *);

typedef void (*heap_print_func)(const heap_data *);

typedef struct heap_ {
    /*
     * Contiguous array of data.
     */
    heap_data *data;
    /*
     * How to compare heap elements.
     */
    heap_less_than_func less_than;
    /*
     * How to print elements.
     */
    heap_print_func printer;
    /*
     * Size of malloced array.
     */
    heap_idx max_size;
    /*
     * Number of elements in use.
     */
    heap_idx in_use;
} heap;

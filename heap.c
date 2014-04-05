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

static heap *heap_malloc (const heap_idx max_size,
                          heap_less_than_func less_than,
                          heap_print_func printer)
{
    heap *h = malloc(sizeof(*h));
    if (!h) {
        return (0);
    }

    h->data = malloc(sizeof(heap_data) * max_size);
    h->max_size = max_size;
    h->in_use = 0;
    h->less_than = less_than;
    h->printer = printer;

    return (h);
}

static void heap_free (heap *h)
{
    free(h->data);
    free(h);
}

static unsigned int heap_insert (heap *h, const heap_data *insert_data)
{
    /*
     * Resize the heap if needed.
     */
    if (h->in_use == h->max_size) {
        h->max_size += (h->max_size + 1) / 2;

        heap_data *new_data = realloc(h->data, sizeof(heap_data) * h->max_size);
        if (!new_data) {
            return (0);
        }

        h->data = new_data;
    }

    heap_idx idx = h->in_use++;
    heap_data *current = &h->data[idx];

    /*
     * Insert the new element. If there are existing elements, we need to
     * walk backwards from the end of the heap, swapping elements as we
     * go until the heap is reordered.
     */
    if (idx) {
        heap_idx parent_idx;
        heap_data *parent;

        /*
         * Get the parent idx, which is half the current idx.
         */
        parent_idx = (idx - 1) / 2;
        parent = &h->data[parent_idx];

        heap_less_than_func less_than = h->less_than;
        do {
            /*
             * If the parent is less than the current value, then we are 
             * ordered.
             */
            if ((less_than)(parent, insert_data)) {
                break;
            }

            /*
             * "Swap" the parent data with the current. The parent data we
             * do not set yet in case there is a further swap.
             */
            *current = *parent;

            /*
             * Parent is now the current idx.
             */
            idx = parent_idx;
            current = &h->data[idx];

            /*
             * Get the parent idx, which is half the current idx.
             */
            parent_idx = (idx - 1) / 2;
            parent = &h->data[parent_idx];

        } while (idx);
    }

    *current = *insert_data;

    return (1);
}

static unsigned int heap_empty (heap *h)
{
    return (!h->in_use);
}

static heap_data heap_pop (heap *h)
{
    assert(!heap_empty(h));

    /*
     * The head element is what we will return.
     */
    heap_data head = h->data[0];

    /*
     * First, we remove the item in slot #1, which is now empty. Then we take 
     * the last item in the heap, and move it up to slot #1.
     */
    heap_data temp = h->data[--h->in_use];
    if (!h->in_use) {
        return (head);
    }

    heap_less_than_func less_than = h->less_than;
    
    heap_idx idx = 0;

    for (;;) {
        heap_idx lowest_child_idx;
        heap_idx other_child_idx;
        heap_idx child_idx;

        /*
         * Next we compare the item to each of its two children, which are at 
         * locations (current position * 2) and  (current position * 2 + 1).  
         *
         * If it has a lower score than both of its two children, it stays 
         * where it is. If not, we swap it with the lower of the two children.  
         */
        child_idx = (idx * 2) + 1;
        if (child_idx >= h->in_use) {
            /*
             * If we are past the end of the heap then it is ordered again.
             */
            break;
        }

        other_child_idx = child_idx + 1;

        const heap_data *child = &h->data[child_idx];
        const heap_data *lowest_child;

        /*
         * Find the lowest of the two children if there are two children.
         */
        if (other_child_idx < h->in_use) {
            const heap_data *other_child = &h->data[other_child_idx];

            if ((less_than)(child, other_child)) {
                lowest_child = child;
                lowest_child_idx = child_idx;
            } else {
                lowest_child = other_child;
                lowest_child_idx = other_child_idx;
            }
        } else {
            /*
             * We only have one child? It must be the lowest.
             */
            lowest_child = child;
            lowest_child_idx = child_idx;
        }

        /*
         * If our value is lower than both children, then we are ordered.
         */
        if ((less_than)(&temp, lowest_child)) {
            break; 
        }

        h->data[idx] = *lowest_child;
        idx = lowest_child_idx;
    }

    /*
     * Re-insert the popped value now.
     */
    h->data[idx] = temp;

    return (head);
}

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

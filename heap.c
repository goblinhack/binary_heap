/*
 * Example implementation of a binary heap - useful in A * star searches
 * 
 * In a sorted list or tree, every element in the tree is in the correct order.
 * However this is not needed for A* where we just want to pull the lowest cost
 * key out of the tree. We don't care if the whole tree is sorted; the rest of
 * the list can be a jumble as long as when we pull items off the list they are
 * sorted.
 * 
 * A binary heap is bunch of items where either the lowest or highest value 
 * item is at the top of the heap. In this demo, I use the lowest value 
 * ordering. Each element in
 * the heap has two children and it must have a value lower than those two children.
 * 
 * e.g. this linear array (the heap):
 * 
 * <pre>
 *     3  7 42 9 12 65 44
 * </pre>
 * 
 * can be viewed as a tree
 * 
 * <pre>
 *                                 ___ 3 ___
 *                                /         \
 *                               7           42 
 *                             /  \         /  \
 *                            9   12       65   44
 * 
 * </pre>
 * 
 * When insterting elements, we add them to the end of the array and then compare
 * with the parent node ((index - 1) / 2) and swap if the parent is lower.
 * 
 * When popping from the head, we take the head element out and swap it with the
 * element at the end of the array. Then we compare with each child node (index * 2) + 1
 * 
 * So each thread of the tree will always be sorted, but the whole tree itself may not
 * be; which is a lot quicker for A* where once we fill the tree, we just want to drain
 * it, in sort order.
 * 
 * The example code, heap.c and binary, binary_heap should produce results like the 
 * following when ran.
 * 
 * <pre>
 * inserted 7  (1  in use):7  
 * inserted 49 (2  in use):7  49 
 * inserted 73 (3  in use):7  49 73 
 * inserted 58 (4  in use):7  49 73 58 
 * inserted 30 (5  in use):7  30 73 58 49 
 * inserted 72 (6  in use):7  30 72 58 49 73 
 * inserted 44 (7  in use):7  30 44 58 49 73 72 
 * inserted 78 (8  in use):7  30 44 58 49 73 72 78 
 * inserted 23 (9  in use):7  23 44 30 49 73 72 78 58 
 * inserted 9  (10 in use):7  9  44 30 23 73 72 78 58 49 
 * inserted 40 (11 in use):7  9  44 30 23 73 72 78 58 49 40 
 * inserted 65 (12 in use):7  9  44 30 23 65 72 78 58 49 40 73 
 * inserted 92 (13 in use):7  9  44 30 23 65 72 78 58 49 40 73 92 
 * inserted 42 (14 in use):7  9  42 30 23 65 44 78 58 49 40 73 92 72 
 * inserted 87 (15 in use):7  9  42 30 23 65 44 78 58 49 40 73 92 72 87 
 * inserted 3  (16 in use):3  7  42 9  23 65 44 30 58 49 40 73 92 72 87 78 
 * inserted 27 (17 in use):3  7  42 9  23 65 44 27 58 49 40 73 92 72 87 78 30 
 * inserted 29 (18 in use):3  7  42 9  23 65 44 27 29 49 40 73 92 72 87 78 30 58 
 * inserted 40 (19 in use):3  7  42 9  23 65 44 27 29 49 40 73 92 72 87 78 30 58 40 
 * inserted 12 (20 in use):3  7  42 9  12 65 44 27 29 23 40 73 92 72 87 78 30 58 40 49 
 * popped   3  (19 in use):7  9  42 27 12 65 44 30 29 23 40 73 92 72 87 78 49 58 40 
 * popped   7  (18 in use):9  12 42 27 23 65 44 30 29 40 40 73 92 72 87 78 49 58 
 * popped   9  (17 in use):12 23 42 27 40 65 44 30 29 58 40 73 92 72 87 78 49 
 * popped   12 (16 in use):23 27 42 29 40 65 44 30 49 58 40 73 92 72 87 78 
 * popped   23 (15 in use):27 29 42 30 40 65 44 78 49 58 40 73 92 72 87 
 * popped   27 (14 in use):29 30 42 49 40 65 44 78 87 58 40 73 92 72 
 * popped   29 (13 in use):30 40 42 49 40 65 44 78 87 58 72 73 92 
 * popped   30 (12 in use):40 40 42 49 58 65 44 78 87 92 72 73 
 * popped   40 (11 in use):40 49 42 73 58 65 44 78 87 92 72 
 * popped   40 (10 in use):42 49 44 73 58 65 72 78 87 92 
 * popped   42 (9  in use):44 49 65 73 58 92 72 78 87 
 * popped   44 (8  in use):49 58 65 73 87 92 72 78 
 * popped   49 (7  in use):58 73 65 78 87 92 72 
 * popped   58 (6  in use):65 73 72 78 87 92 
 * popped   65 (5  in use):72 73 92 78 87 
 * popped   72 (4  in use):73 78 92 87 
 * popped   73 (3  in use):78 87 92 
 * popped   78 (2  in use):87 92 
 * popped   87 (1  in use):92 
 * popped   92 (0  in use):
 * </pre>
 * 
 * Plese let me know if this is useful to anyone. I wrote this as examples I 
 * found did not quite work, and/or had no demo of it working.
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

binary_heap
===========

Example implementation of a binary heap - useful in A * star searches

In a sorted list or tree, every element in the tree is in the correct order.
However this is not needed for A* where we just want to pull the lowest cost
key out of the tree. We don't care if the whole tree is sorted; the rest of
the list can be a jumble as long as when we pull items off the list they are
sorted.


In a sorted list, every item on the list is in its proper order, lowest-to-highest or highest-to-lowest. This is helpful, but is actually more than we really need. We don’t actually care if item number 127 is lower than number 128 on the list. All we really want is the lowest F cost item to be easily accessible at the top of the list. The rest of the list can be a jumble, for all we care. Keeping the rest of the list properly sorted is just extra unneeded work until, of course, we need the next lowest F cost item.

A binary heap is bunch of items where either the lowest or highest value item is at 
the top of the heap. In this demo, I use the lowest value ordering. Each element in
the heap has two children and it must have a value lower than those two children.

e.g. this linear array (the heap):

<pre>
    3  7 42 9 12 65 44
</pre>

can be viewed as a tree

<pre>
                                ___ 3 ___
                               /         \
                              7           42 
                            /  \         /  \
                           9   12       65   44

</pre>

When insterting elements, we add them to the end of the array and then compare
with the parent node ((index - 1) / 2) and swap if the parent is lower.

When popping from the head, we take the head element out and swap it with the
element at the end of the array. Then we compare with each child node (index * 2) + 1

So each thread of the tree will always be sorted, but the whole tree itself may not
be; which is a lot quicker for A* where once we fill the tree, we just want to drain
it, in sort order.

The example code, heap.c and binary, binary_heap should produce results like the 
following when ran.

<pre>
inserted 7  (1  in use):7  
inserted 49 (2  in use):7  49 
inserted 73 (3  in use):7  49 73 
inserted 58 (4  in use):7  49 73 58 
inserted 30 (5  in use):7  30 73 58 49 
inserted 72 (6  in use):7  30 72 58 49 73 
inserted 44 (7  in use):7  30 44 58 49 73 72 
inserted 78 (8  in use):7  30 44 58 49 73 72 78 
inserted 23 (9  in use):7  23 44 30 49 73 72 78 58 
inserted 9  (10 in use):7  9  44 30 23 73 72 78 58 49 
inserted 40 (11 in use):7  9  44 30 23 73 72 78 58 49 40 
inserted 65 (12 in use):7  9  44 30 23 65 72 78 58 49 40 73 
inserted 92 (13 in use):7  9  44 30 23 65 72 78 58 49 40 73 92 
inserted 42 (14 in use):7  9  42 30 23 65 44 78 58 49 40 73 92 72 
inserted 87 (15 in use):7  9  42 30 23 65 44 78 58 49 40 73 92 72 87 
inserted 3  (16 in use):3  7  42 9  23 65 44 30 58 49 40 73 92 72 87 78 
inserted 27 (17 in use):3  7  42 9  23 65 44 27 58 49 40 73 92 72 87 78 30 
inserted 29 (18 in use):3  7  42 9  23 65 44 27 29 49 40 73 92 72 87 78 30 58 
inserted 40 (19 in use):3  7  42 9  23 65 44 27 29 49 40 73 92 72 87 78 30 58 40 
inserted 12 (20 in use):3  7  42 9  12 65 44 27 29 23 40 73 92 72 87 78 30 58 40 49 
popped   3  (19 in use):7  9  42 27 12 65 44 30 29 23 40 73 92 72 87 78 49 58 40 
popped   7  (18 in use):9  12 42 27 23 65 44 30 29 40 40 73 92 72 87 78 49 58 
popped   9  (17 in use):12 23 42 27 40 65 44 30 29 58 40 73 92 72 87 78 49 
popped   12 (16 in use):23 27 42 29 40 65 44 30 49 58 40 73 92 72 87 78 
popped   23 (15 in use):27 29 42 30 40 65 44 78 49 58 40 73 92 72 87 
popped   27 (14 in use):29 30 42 49 40 65 44 78 87 58 40 73 92 72 
popped   29 (13 in use):30 40 42 49 40 65 44 78 87 58 72 73 92 
popped   30 (12 in use):40 40 42 49 58 65 44 78 87 92 72 73 
popped   40 (11 in use):40 49 42 73 58 65 44 78 87 92 72 
popped   40 (10 in use):42 49 44 73 58 65 72 78 87 92 
popped   42 (9  in use):44 49 65 73 58 92 72 78 87 
popped   44 (8  in use):49 58 65 73 87 92 72 78 
popped   49 (7  in use):58 73 65 78 87 92 72 
popped   58 (6  in use):65 73 72 78 87 92 
popped   65 (5  in use):72 73 92 78 87 
popped   72 (4  in use):73 78 92 87 
popped   73 (3  in use):78 87 92 
popped   78 (2  in use):87 92 
popped   87 (1  in use):92 
popped   92 (0  in use):
</pre>

Plese let me know if this is useful to anyone. I wrote this as examples I found did
not quite work, and/or had no demo of it working.

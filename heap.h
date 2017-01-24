// heap.h
/*
Heap for storage of generic elements, stores void pointers.
By Anton Babkin.
*/

#ifndef __HEAP_H
#define __HEAP_H

#include <stdbool.h>
#include <stddef.h>


typedef struct heap *heap_ptr;

/* Return pointer to newly created heap. By passing different cmp functions, it can be max or min-heap. */
heap_ptr heap_create(size_t max_size, bool (*cmp)(void *a, void *b));

/* Free allocated memory. */
void heap_destroy(heap_ptr h);

bool heap_is_empty(heap_ptr h);

void heap_insert(heap_ptr h, void *x);

/* Return item from top of heap. */
void *heap_top(heap_ptr h);

/* Return and remove item from top of heap. */
void *heap_pop(heap_ptr h);

void heap_unit_test(void);

#endif /* __HEAP_H */

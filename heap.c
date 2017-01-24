// heap.c


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "heap.h"

struct heap
{
    size_t nmax;                      /* maximum size of heap */
    size_t n;                         /* current size of heap */
    void **q;                         /* dynamic array of void pointers, stored in heap. 1-indexing */
    bool (*higher)(void *a, void *b); /* pointer to comparison function, returns true if a must be higher than b in heap */
};

/* default comparison function: for min heap that stores pointers to int elements */
bool heap_min_int_higher(void *a, void *b)
{
    int a_num = *(int *)a;
    int b_num = *(int *)b;
    return (bool)(a_num < b_num);
}

/* convenience function: calls comparison on elements with indexes i, j */
static bool heap_idx_higher(heap_ptr h, size_t i, size_t j)
{
    return h->higher(h->q[i], h->q[j]);
}

/* returns pointer to newly created heap. by passing different cmp functions, it can be max or min-heap */
heap_ptr heap_create(size_t max_size, bool (*cmp)(void *a, void *b))
{
    heap_ptr h = malloc(sizeof *h);
    h->nmax = max_size;
    h->n = 0;
    h->q = malloc((max_size + 1) * (sizeof *h->q)); /* 1-indexing */
    h->higher = (cmp == NULL ? heap_min_int_higher : cmp);
    return h;
}

void heap_destroy(heap_ptr h)
{
    free(h->q);
    free(h);
}

bool heap_is_empty(heap_ptr h)
{
    return h->n == 0;
}

/* index of i's parent */
static size_t heap_parent(size_t i)
{
    if (i == 1)
    { /* root */
        return -1;
    }
    else
    {
        return i / 2; /* implicit floor(i/2) */
    }
}

/* index of left child of i */
static size_t heap_young_child(size_t i)
{
    return 2 * i;
}

/* swap two elements in heap */
static void heap_swap(heap_ptr h, size_t i, size_t j)
{
    void *tmp = h->q[i];
    h->q[i] = h->q[j];
    h->q[j] = tmp;
}

/* restore heap property by bubbling i's element up */
static void heap_bubble_up(heap_ptr h, size_t i)
{
    size_t j = heap_parent(i);
    if (j == -1)
    { /* root, no parent */
        return;
    }

    if (heap_idx_higher(h, i, j))
    {
        /* i must be above j */
        heap_swap(h, i, j);
        heap_bubble_up(h, j);
    }
}

/* add item to heap */
void heap_insert(heap_ptr h, void *x)
{
    if (h->n >= h->nmax)
    {
        printf("Warning: heap overflow insert\n");
    }
    else
    {
        h->n++;
        h->q[h->n] = x;
        heap_bubble_up(h, h->n);
    }
}


/* restore heap property by bubbling i's element down */
static void heap_bubble_down(heap_ptr h, size_t i)
{
    size_t j; /* counter */
    size_t c = heap_young_child(i);
    size_t i_top = i; /* index of element that must be closer to top */

    for (j = 0; j <= 1; j++)
    {
        if (c + j <= h->n)
        {
            if (heap_idx_higher(h, c + j, i_top))
            {
                /* c+j must be above previous i_top */
                i_top = c + j;
            }
        }
    }

    if (i_top != i)
    {
        heap_swap(h, i, i_top);
        heap_bubble_down(h, i_top);
    }
}

/* show item from top of heap */
void *heap_top(heap_ptr h)
{
    void *top;
    if (h->n == 0)
    {
        printf("Warning: looking up top of empty heap\n");
        top = NULL;
    }
    else
    {
        top = h->q[1];
    }
    return top;
}

/* extract item from top of heap */
void *heap_pop(heap_ptr h)
{
    void *top; /* returned value */

    if (h->n == 0)
    {
        printf("Warning: extracting from empty heap\n");
        top = NULL;
    }
    else
    {
        top = h->q[1];
        h->q[1] = h->q[h->n];
        h->n--;
        heap_bubble_down(h, 1);
    }

    return top;
}

/* restore heap property after i's key value changed */
void heap_heapify(heap_ptr h, size_t i)
{
    size_t ip = heap_parent(i);
    size_t ic = heap_young_child(i);
    size_t j;

    if (ip != -1)
    {
        if (heap_idx_higher(h, i, ip))
        {
            /* i must be higher than it's current parrent ip */
            heap_bubble_up(h, i);
        }
    }
    else
    {
        for (j = 0; j <= 1; j++)
        {
            if (ic + j <= h->n)
            {
                if (heap_idx_higher(h, ic + j, i))
                {
                    /* i must be lower than it's child c+j */
                    heap_bubble_down(h, i);
                    break;
                }
            }
        }
    }
}

void heap_unit_test(void)
{
    srand(time(0)); //use current time as seed for random generator

    const int N = 10;
    printf("Min heap\n");
    heap_ptr h = heap_create(N, NULL);
    int *a = malloc(N * sizeof(int));

    printf("Inserting elements... ");
    for (int i = 0; i < N; i++)
    {
        a[i] = rand() % 100;
        printf("%d ", a[i]);
        heap_insert(h, a + i);
    }
    printf("\n");

    printf("Popping elements... ");
    while (!heap_is_empty(h))
    {
        printf("%d ", *(int *)heap_pop(h));
    };
    printf("\n");

    heap_destroy(h);
}
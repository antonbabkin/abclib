// hashmap.c
/*
Every bin contains a linked list - chaining to resolve collisions.
By Anton Babkin.
*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "hashmap.h"

struct hashmap
{
    int n;               /* number of bins */
    hashmap_node_ptr *q; /* dynamic array of pointers to linked list nodes */
};

/* hash function */
static int hash(hashmap_ptr h, int key)
{
    return key % h->n;
}

hashmap_ptr hashmap_create(int size)
{
    hashmap_ptr h = malloc(sizeof *h);
    h->n = size;
    h->q = calloc(size, sizeof *h->q); /* initialize all bins to NULL */
    return h;
}

void hashmap_destroy(hashmap_ptr h)
{
    int i;
    hashmap_node_ptr node;
    hashmap_node_ptr prev;
    for (i = 0; i < h->n; i++)
    {
        node = h->q[i];
        while (node != NULL)
        {
            prev = node;
            node = node->next;
            free(prev);
        }
    }
    free(h->q);
    free(h);
}

void hashmap_insert(hashmap_ptr h, int key, int val)
{
    int hkey = hash(h, key);
    hashmap_node_ptr node = malloc(sizeof *node);
    node->key = key;
    node->val = val;
    node->next = h->q[hkey];
    h->q[hkey] = node;
}

hashmap_node_ptr hashmap_lookup(hashmap_ptr h, int key)
{
    int hkey = hash(h, key);
    hashmap_node_ptr node;
    for (node = h->q[hkey]; node != NULL; node = node->next)
    {
        if (node->key == key)
        {
            break;
        }
    }
    return node;
}

void hashmap_unit_test(void)
{
    const int N = 11;
    hashmap_ptr h = hashmap_create(N);

    hashmap_node_ptr node;

    /* lookup non-existing elements */
    assert(hashmap_lookup(h, 6) == NULL);
    assert(hashmap_lookup(h, 100) == NULL);

    /* insert and lookup elements without collisions */
    hashmap_insert(h, 6, -6);
    assert(hashmap_lookup(h, 6)->val == -6);
    hashmap_insert(h, 100, -100);
    assert(hashmap_lookup(h, 100)->val == -100);

    /* insert and lookup with collision */
    hashmap_insert(h, 6 + N * 5, -600);
    assert(hashmap_lookup(h, 6 + N * 5)->val == -600);

    /* lookup non-existing keys at existing hash keys */
    assert(hashmap_lookup(h, 6 + N) == NULL);
    assert(hashmap_lookup(h, 100 + N) == NULL);

    hashmap_destroy(h);
    printf("Hash map tests completed successfully.\n");
}

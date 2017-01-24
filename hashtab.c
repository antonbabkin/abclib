/*
Open addressing to resolve collisions.

By Anton Babkin.
*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "hashtab.h"

struct hashtab
{
    size_type n;  /* number of bins */
    item_type *q; /* dynamic array of stored items */
    bool *p;      /* dynamic array: if item is present */
};

/* hash function */
static size_type hash(hashtab_ptr ht, item_type x)
{
    size_type y;
    y = x % ht->n;
    return y < 0 ? y + ht->n : y;
}

hashtab_ptr hashtab_create(size_type n)
{
    hashtab_ptr ht = malloc(sizeof *ht);
    ht->n = n;
    ht->q = malloc(n * sizeof(item_type));
    ht->p = calloc(n, sizeof(bool));
    return ht;
}

void hashtab_destroy(hashtab_ptr ht)
{
    free(ht->q);
    free(ht->p);
    free(ht);
}

void hashtab_insert(hashtab_ptr ht, item_type x)
{
    size_type ih = hash(ht, x), i, j;
    size_type n = ht->n;
    /* find empty bin, starting from ih and iterating forward by 1 */
    for (i = 0, j = ih; i < n && ht->p[j]; i++, j = (ih + i) % n)
        ;
    ht->p[j] = true;
    ht->q[j] = x;
}

bool hashtab_lookup(hashtab_ptr ht, item_type x)
{
    size_type ih = hash(ht, x), i, j;
    size_type n = ht->n;
    /* find empty bin, starting from ih and iterating forward by 1 */
    for (i = 0; i < n; i++)
    {
        j = (ih + i) % n;
        if (ht->p[j])
        {
            if (ht->q[j] == x)
            {
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}


void hashtab_unit_test(void) {
    const int N = 11;
    hashtab_ptr h = hashtab_create(N);

    /* lookup non-existing elements */
    assert(!hashtab_lookup(h, 6));
    assert(!hashtab_lookup(h, 100));

    /* insert and lookup elements without collisions */
    hashtab_insert(h, 6);
    assert(hashtab_lookup(h, 6));
    hashtab_insert(h, 100);
    assert(hashtab_lookup(h, 100));

    /* insert and lookup with collision */
    hashtab_insert(h, 6 + N * 5);
    assert(hashtab_lookup(h, 6 + N * 5));

    /* lookup non-existing keys at existing hash keys */
    assert(!hashtab_lookup(h, 6 + N));
    assert(!hashtab_lookup(h, 100 + N));

    hashtab_destroy(h);
    printf("Hash table tests completed successfully.\n");
}

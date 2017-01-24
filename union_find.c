// union_find.h
/* 
Implementation of union-find data structure with lazy union and path compression.
By Anton Babkin
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "union_find.h"

struct uf
{
    uf_int n;  /* total number of elements */
    uf_int nc; /* number of connected components */
    uf_int *p; /* dynamic array: p[i] is parent of element i */
    uf_int *r; /* dynamic array: r[i] is rank of element i */
};

uf_ptr uf_create(uf_int size)
{
    uf_ptr u = malloc(sizeof *u);
    u->n = size;
    u->nc = size;
    u->p = malloc(size * sizeof(*(u->p)));
    for (uf_int i = 0; i < size; i++)
    {
        u->p[i] = i; /* every element is in it's own group, has itself as parent */
    }
    u->r = calloc(size, sizeof(*(u->p))); /* implicit = 0, initial rank */
    return u;
}

void uf_destroy(uf_ptr u)
{
    free(u->p);
    free(u->r);
    free(u);
}

uf_int uf_size(uf_ptr u)
{
    return u->nc;
}

/* find root of x */
uf_int uf_find(uf_ptr u, uf_int x)
{
    uf_int root = x;
    /* go up the tree until root */
    while (root != u->p[root])
    {
        root = u->p[root];
    }

    /* path compression: go up the tree again, making all elements point to root */
    uf_int parent = u->p[x];
    while (parent != root)
    {
        u->p[x] = root;
        x = parent;
        parent = u->p[x];
    }

    return root;
}

bool uf_connected(uf_ptr u, uf_int x, uf_int y)
{
    return uf_find(u, x) == uf_find(u, y);
}

void uf_union(uf_ptr u, uf_int x, uf_int y)
{
    uf_int xroot = uf_find(u, x);
    uf_int yroot = uf_find(u, y);
    if (xroot == yroot)
    {
        printf("Warning: union of already connected elements: %d, %d", x, y);
        return;
    }
    uf_int xrank = u->r[xroot];
    uf_int yrank = u->r[yroot];
    if (xrank > yrank)
    {
        u->p[yroot] = xroot;
    }
    else
    {
        u->p[xroot] = yroot;
        if (xrank == yrank)
        {
            u->r[yroot]++;
        }
    }
    u->nc--;
}

void uf_unit_test(void)
{
    const int N = 100;
    uf_ptr u = uf_create(N);

    /* initially all elements only connected to self */
    assert(uf_size(u) == N);
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
        {
            if (x == y)
                assert(uf_connected(u, x, y));
            else
                assert(!uf_connected(u, x, y));
        }
    }

    /* connect 0+1, 1+2, 2+3, etc */
    for (int z = 1; z < N; z++)
    {
        uf_union(u, z - 1, z);
        assert(uf_size(u) == N - z);
        for (int x = 0; x < N; x++)
        {
            for (int y = 0; y < N; y++)
            {
                if ((x <= z && y <= z) || x == y)
                    assert(uf_connected(u, x, y));
                else
                    assert(!uf_connected(u, x, y));
            }
        }
    }

    uf_destroy(u);

    printf("Union-find tests completed successfully.\n");
}

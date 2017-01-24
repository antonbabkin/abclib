// union_find.h
/*
Union-find data structure (disjoint set).
By Anton Babkin.
*/

#ifndef __UNION_FIND_H
#define __UNION_FIND_H

#include <stdbool.h>

typedef int uf_int;

typedef struct uf *uf_ptr;

/* Create new union-find of given size. */
uf_ptr uf_create(uf_int size);

/* Free allocated memory. */
void uf_destroy(uf_ptr u);

/* Return current number of connected components. */
uf_int uf_size(uf_ptr u);

/* Return the root (leader) of given element x. */
uf_int uf_find(uf_ptr u, uf_int x);

/* True if two elements x and y are connected. */
bool uf_connected(uf_ptr u, uf_int x, uf_int y);

/* Join two elements x and y. */
void uf_union(uf_ptr u, uf_int x, uf_int y);

void uf_unit_test(void);

#endif

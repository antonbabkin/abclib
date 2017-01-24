/*
Hash map.
Stores {key, value} pairs, creating hash of the key.

By Anton Babkin.
*/

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include <stdbool.h>

typedef struct hashmap *hashmap_ptr;

typedef struct hashmap_node *hashmap_node_ptr;

struct hashmap_node
{
    int key, val;
    hashmap_node_ptr next;
};

/* Return pointer to newly created hash map. */
hashmap_ptr hashmap_create(int size);

/* Free allocated memory. */
void hashmap_destroy(hashmap_ptr h);

/* Insert key-value pair. */
void hashmap_insert(hashmap_ptr h, int key, int val);

/* Return pointer to node with given key. Returns NULL hash map does not contain such key. */
hashmap_node_ptr hashmap_lookup(hashmap_ptr h, int key);

void hashmap_unit_test(void);

#endif /* __HASHMAP_H */

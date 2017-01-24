// hashtab.h
/*
Hash table: add keys and check if key is present. Deletion is not implemented.
By Anton Babkin.
*/

#ifndef __HASHTAB_H
#define __HASHTAB_H

#include <stdbool.h>


typedef long long item_type;
typedef long size_type;

typedef struct hashtab *hashtab_ptr;

/* Return pointer to newly created hash table. */
hashtab_ptr hashtab_create(size_type n);

/* Free allocated memory. */
void hashtab_destroy(hashtab_ptr ht);

/* Insert key x into hash table. */
void hashtab_insert(hashtab_ptr ht, item_type x);

/* Return true if key x is in hash table. */
bool hashtab_lookup(hashtab_ptr ht, item_type x);

void hashtab_unit_test(void);

#endif /* __HASHTAB_H */

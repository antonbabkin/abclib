/*
Sorting functions.


By Anton Babkin
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Compare numbers. */
typedef long num_t
int numcmp(const void *xp, const void *yp) {
  num_t x = *(num_t *)xp;
  num_t y = *(num_t *)yp;
  if (x < y) {
    return -1;
  } else if (x > y) {
    return 1;
  } else {
    return 0;
  }
}

/* Compare numbers by their pointers */
int numpcmp(const void *xp, const void *yp) {
  return numcmp(*(num_t **)xp, *(num_t **)yp);
}



/* Merge sort. Generic function that can sort objects of any type. */
void mergesort(void *ap, const size_t n, const size_t size, int (*cmp)(const void *, const void *)) {

  /* base case */
  if (n < 2) {
    return;
  }

  /* convert into pointer to 1-byte sized objects: can use pointer arithmetic */
  char *p = (char *)ap;

  /* create new arrays for left and right halves */
  size_t nl = n / 2;
  size_t nr = n - nl;
  char *l = malloc(size * nl);
  char *r = malloc(size * nr);  

  /* copy into left and right */
  memcpy(l, p, size * nl);
  memcpy(r, p + size*nl, size * nr);

  /* recursive call to sort left and right halves */
  mergesort(l, nl, size, cmp);
  mergesort(r, nr, size, cmp);

  /* merge sorted halves into original array */
  size_t i;
  size_t jl = 0;
  size_t jr = 0;
  for (i = 0; i < n; i++) {
    if (jl >= nl) { /* left half already fully copied, copy from right */
      memcpy(p+i*size, r+jr*size, (nr-jr)*size);
      break;
    } else if (jr >= nr) { /* right half already fully copied, copy from left */
      memcpy(p+i*size, l+jl*size, (nl-jl)*size);
      break;
    } else {
      if (cmp(l + jl*size, r + jr*size) == 1) { /* x_left > x_right: copy from right */
        memcpy(p+i*size, r+jr*size, size);
        jr++;
      } else { /* copy from left */
        memcpy(p+i*size, l+jl*size, size);
        jl++;
      }
    }
  }

  free(l);
  free(r);
}



/* Swap two long's in an array */
void swap(long a[], size_t i, size_t j) {
  long tmp = a[i];
  a[i] = a[j];
  a[j] = tmp;
}

/* 
Insertion sort. 
*/
void inssort(long a[], size_t n) {
  size_t i, j;
  for (i=1; i<n; i++) {
    for (j=i; j>0 && a[j]<a[j-1]; j--) {
      swap(a, j, j-1);
    }
  }
}

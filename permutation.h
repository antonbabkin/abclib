// permutation.h
/*
Combinatoric routines. 
By Anton Babkin.
*/

#ifndef __PERMUTATION_H
#define __PERMUTATION_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*
Takes array of ints and constructs next permutation in lexicographical order. Input array is modified in place.
Example: [1,2,3] -> [1,3,2] -> [2,1,3] -> [2,3,1] -> [3,1,2] -> [3,2,1]
Returns 0 on success and 1 if array is already the largest permutation ([3,2,1] in the example).
*/
int nextPermutationIntArr(int arr[], int n);

/*
Takes unsigned int and returns next permutation of bits in lexicographical order.
Example: 0011 -> 0101 -> 0110 -> 1001 -> 1010 -> 1100 -> ...
*/
unsigned int nextPermutationBitInt(unsigned int num);

void permutationUnitTest(void);

#endif /* __PERMUTATION_H */
// permutation.c
#include "permutation.h"
#include "bit.h"

/* Swap two elements of integer array. */
void swapInt(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/*
Algorithm is described here: https://www.nayuki.io/page/next-lexicographical-permutation-algorithm
*/
int nextPermutationIntArr(int arr[], int n) {
    /* Find pivot element with the longest weakly decreasing suffix [pivot+1, n). */
    int pivot;
    for (pivot = n-2; pivot >= 0 && arr[pivot] >= arr[pivot+1]; pivot--)
        ;
    if (pivot == -1) {
        /* Array is weakly decreasing and is already the last possible permutation. */
        return 1;
    }

    /* Find smallest element in suffix that is greater than pivot. */
    int newPivot;
    for (newPivot = n-1; arr[newPivot] <= arr[pivot]; newPivot--)
        ;
    
    /* Swap pivots. */
    swapInt(arr, pivot, newPivot);

    /* Make suffix as small as possible by reverting and making it weakly increasing. */
    int i, j;
    for (i = pivot + 1, j = n-1; i < j; i++, j--) {
        swapInt(arr, i, j);
    }

    return 0;
}

/* Compare arrays a and b in lexicographical sense. Numbers are read from left to right. Arrays must have same length n.
Example: [1,2,3] < [1,2,4] < [1,3,1]
*/
int cmpIntArr(int a[], int b[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (a[i] > b[i]) return 1;
        else if (a[i] < b[i]) return -1;
    }
    return 0;
}

/* Assuming that input number has MSB at least 2 positions smaller than max width, so that next permutation always exists. */
unsigned int nextPermutationBitInt(unsigned int num) {
    /* Most significant bit. */
    int msb;
    unsigned int tmp;
    tmp = num;
    msb = -1;
    while (tmp != 0) {
        msb++;
        tmp >>= 1;
    }

    /* Find pivot. (Pivot bit will always be "0".) */
    unsigned int pivot;
    for (pivot = 1; pivot <= msb && bitGet(num, pivot) >= bitGet(num, pivot - 1); pivot++)
        ;

    /* Find smallest element in suffix that is greater than pivot. (Always the last "1".) */
    int newPivot;
    for (newPivot = 0; bitGet(num, newPivot) <= bitGet(num, pivot); newPivot++)
        ;
    
    /* Swap pivots. */
    num = bitSwap(num, pivot, newPivot);

    /* Make suffix as small as possible by reverting and making it weakly increasing. */
    int i, j;
    for (i = pivot - 1, j = 0; i > j; i--, j++) {
        num = bitSwap(num, i, j);
    }
    return num;
}



void permutationUnitTest(void) {
    /* a < b < c */
    int aa[] = {1,2,3}, bb[] = {1,2,4}, cc[] = {1,3,1};
    assert(cmpIntArr(aa, bb, 3) == -1);
    assert(cmpIntArr(cc, bb, 3) == 1);
    assert(cmpIntArr(aa, cc, 3) == -1);
    assert(cmpIntArr(bb, bb, 3) == 0);

    int n = 4;
    int i;
    int *a = malloc(n * sizeof(int));
    int *b = malloc(n * sizeof(int));
    int nPerm = 1; /* Number of possible permutations = n! */
    for (i = 0; i < n; i++) {
        a[i] = i;
        b[i] = i;
        nPerm *= i + 1;
    }

    /* Permutations should be in increasing order. */
    while (nextPermutationIntArr(a, n) == 0) {
        nPerm--;
        assert(cmpIntArr(a, b, n) == 1);
        for (i = 0; i < n; i++) {
            b[i] = a[i];
        }
    }
    /* If there are m permutations, nPerm must be decremented m-1 times. */
    assert(nPerm == 1);

    /* Bit permutation. */
    {
        int n = 7; /* 0111 */
        do {
            bitFPrint(n, stdout);
        } while ((n = nextPermutationBitInt(n)) < (1 << 7));
    }

    free(a);
    free(b);

    printf("Permutation tests completed successfully.\n");
}
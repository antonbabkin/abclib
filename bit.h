// bit.h
/*
Bit manipulation. 
By Anton Babkin
*/


#ifndef __BIT_H
#define __BIT_H

#include <stdbool.h>
#include <stdio.h>

/* Return number represented by a string of bits. */
int bitNumFromBitStr(char *s);

/* Return true if bit i is set in num. */
bool bitGet(int num, int i);

/* Return num with bit i set. */
int bitSet(int num, int i);

/* Return num with bit i cleared. */
int bitClear(int num, int i);

/* Return num with bit i set to val. */
int bitUpdate(int num, int i, bool val);

/* Return num with bit i swapped with bit j. */
int bitSwap(int num, int i, int j);

/* Print binary representation of num into outStream. */
void bitFPrint(int num, FILE *outStream);

/* Print binary representation of num into string buf and return pointer to this string.
Buffer is assumed to big large enough to fit all bits of the number. */
char *bitSPrint(int num, char *buf);

/* Unit tests. */
void bitUnitTest(void);

#endif /* __BIT_H */
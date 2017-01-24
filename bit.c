// bit.c

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "bit.h"



int bitNumFromBitStr(char *s) {
    int num = 0;
    int n = strlen(s);
    for (int i = 0; i < n; i++) {
        int bit = n - 1 - i;
        num = bitUpdate(num, bit, s[i] == '1');
    }
    return num;
}

bool bitGet(int num, int i) {
    return (num & (1 << i)) != 0;
}

int bitSet(int num, int i) {
    return num | (1 << i);
}

int bitClear(int num, int i) {
    return num & (~(1 << i));
}

int bitUpdate(int num, int i, bool val) {
    if (val) {
        return bitSet(num, i);
    } else {
        return bitClear(num, i);
    }
}

int bitSwap(int num, int i, int j) {
    bool tmp = bitGet(num, i);
    num = bitUpdate(num, i, bitGet(num, j));
    num = bitUpdate(num, j, tmp);
    return num;
}


void bitFPrint(int num, FILE *outStream) {
    int n = 8 * (sizeof num);
    char *buf = malloc((n + 1) * (sizeof *buf));
    bitSPrint(num, buf);
    fprintf(outStream, "%s\n", buf);
    free(buf);
}

char *bitSPrint(int num, char *buf) {
    int n = 8 * (sizeof num);
    for (int i = 0; i < n; i++) {
        int b = n - i - 1;
        buf[i] = bitGet(num, b) ? '1' : '0';
    }
    buf[n] = '\0';
    return buf;
}



void bitUnitTest(void) {

    assert(sizeof(int) == 4);
    int w = 32;

    int pos = 0xb; // 00001011
    int neg = -7; // 11111001;

    assert(bitGet(pos, 0));
    assert(bitGet(pos, 1));
    assert(!bitGet(pos, 2));
    assert(bitGet(pos, 3));
    for (int i = 4; i < (sizeof pos); i++) {
        assert(!bitGet(pos, i));
    }


    assert(bitGet(neg, 0));
    assert(!bitGet(neg, 1));
    assert(!bitGet(neg, 2));
    assert(bitGet(neg, 3));
    for (int i = 4; i < (sizeof neg); i++) {
        assert(bitGet(neg, i));
    }

    assert(bitSet(pos, 0) == pos);
    assert(bitSet(pos, 4) == 0x1b);
    assert(bitSet(neg, 10) == neg);
    assert(bitSet(neg, 1) == -5);


    printf("%d\n", bitNumFromBitStr("1011"));
    assert(bitNumFromBitStr("1011") == pos);
    assert(bitNumFromBitStr("001011") == pos);
    assert(bitNumFromBitStr("1001011") != pos);
    assert(-bitNumFromBitStr("111") == neg);
    assert(bitNumFromBitStr("11111111111111111111111111111001") == neg);
    

    assert(bitClear(pos, 0) == bitNumFromBitStr("1010"));
    assert(bitClear(neg, 4) == bitNumFromBitStr("11111111111111111111111111101001"));
    assert(bitClear(-1, w - 1) == 0x7fffffff);

    assert(bitUpdate(pos, 5, true) == 0x2b); // 00101011
    assert(bitUpdate(neg, 5, false) == bitNumFromBitStr("11111111111111111111111111011001"));
    

    char *buf = malloc((sizeof *buf) * (sizeof(int) + 1));
    // int x = bitClear(-1, w - 1);
    // printf("%d: %s\n", x, bitSPrint(x, buf));
    for (int i = -16; i <= 15; i++) {
        printf("%d: ", i);
        bitFPrint(i, stdout);
    }

    free(buf);
}
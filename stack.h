// stack.h
/*
Stack - LIFO container.
By Anton Babkin.
*/

#ifndef __STACK_H
#define __STACK_H

#include <stdbool.h>

typedef struct Stack *StackPtr;

/* Initialize empty stack. */
StackPtr stackCreate(void);

/* Deallocate stack memory. */
void stackDestroy(StackPtr);

bool stackIsEmpty(StackPtr s);

/* Returns element on top of the stack. */
int stackPeek(StackPtr s);

/* Put element on top. */
void stackPush(StackPtr s, int item);

/* Remove and return top of the stack. */
int stackPop(StackPtr s);

void stackUnitTest(void);

#endif /* __STACK_H */
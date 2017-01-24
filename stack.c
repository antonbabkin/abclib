// stack.c
/* 
Linked list implementation.
By Anton Babkin.
*/

#include <stdlib.h>
#include <assert.h> /* for unit test */
#include <stdio.h>  /* for unit test */

#include "stack.h"

typedef struct StackNode *StackNodePtr;

struct StackNode
{
    int item;
    StackNodePtr next;
};

struct Stack
{
    StackNodePtr top;
    int size;
};

StackPtr stackCreate(void)
{
    StackPtr s = malloc(sizeof *s);
    s->size = 0;
    s->top = NULL;
    return s;
}

void stackDestroy(StackPtr s)
{
    StackNodePtr node, nextNode;
    node = s->top;
    while (node != NULL)
    {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
    free(s);
}

bool stackIsEmpty(StackPtr s)
{
    return s->size == 0;
}

int stackPeek(StackPtr s)
{
    if (stackIsEmpty(s))
    {
        perror("Peek empty stack - aborted");
        return 0;
    }
    else
    {
        return s->top->item;
    }
}

void stackPush(StackPtr s, int item)
{
    StackNodePtr node = malloc(sizeof *node);
    node->item = item;
    node->next = s->top;
    s->top = node;
    s->size++;
}

int stackPop(StackPtr s)
{
    if (stackIsEmpty(s))
    {
        perror("Pop empty stack - aborted");
        return 0;
    }
    else
    {
        int item = s->top->item;
        StackNodePtr node = s->top;
        s->top = node->next;
        free(node);
        s->size--;
        return item;
    }
}

void stackUnitTest(void)
{
    StackPtr s = stackCreate();
    assert(s != NULL);

    /* Reading empty stack: errors. */
    assert(stackPeek(s) == 0);
    assert(stackPop(s) == 0);

    assert(stackIsEmpty(s));

    int const N = 10;
    for (int i = 0; i < N; i++)
    {
        stackPush(s, i);
        assert(stackPeek(s) == i);
    }

    assert(!stackIsEmpty(s));

    for (int i = N - 1; i >= 0; i--)
    {
        assert(stackPop(s) == i);
    }

    assert(stackIsEmpty(s));

    stackDestroy(s);
    printf("Stack unit test completed successfully.\n");
}
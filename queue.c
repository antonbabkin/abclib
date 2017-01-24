// queue.c
/* Linked list implementation. */


#include <stdlib.h>
#include <assert.h> /* for unit test */
#include <stdio.h> /* for unit test */

#include "queue.h"

typedef struct QueueNode *QueueNodePtr;
struct QueueNode {
    int item;
    QueueNodePtr next;
};

struct Queue {
    QueueNodePtr first, last;
    int size;
};

QueuePtr queueCreate(void) {
    QueuePtr q = malloc(sizeof *q);
    q->size = 0;
    q->first = NULL;
    q->last = NULL;
    return q;
}

void queueDestroy(QueuePtr q) {
    QueueNodePtr node, nextNode;
    node = q->first;
    while (node != NULL) {
        nextNode = node->next;
        free(node);
        node = nextNode;
    }
    free(q);
}

bool queueIsEmpty(QueuePtr q) {
    return q->size == 0;
}

int queuePeek(QueuePtr q) {
    if (queueIsEmpty(q)) {
        perror("Peek empty queue - aborted");
        return 0;
    } else {
        return q->first->item;
    }
}

void queueAdd(QueuePtr q, int item) {
    QueueNodePtr node = malloc(sizeof *node);
    node->item = item;
    node->next = NULL;
    if (queueIsEmpty(q)) {
        q->first = node;
        q->last = node;
    } else {
        q->last->next = node;
        q->last = node;
    }
    q->size++;
}

int queueRemove(QueuePtr q) {
    if (queueIsEmpty(q)) {
        perror("Pop empty queue - aborted");
        return 0;
    } else {
        int item = q->first->item;
        QueueNodePtr node = q->first;
        if (q->size == 1) {
            q->first = NULL;
            q->last = NULL;
        } else {
            q->first = node->next;
        }
        free(node);
        q->size--;
        return item;
    }
}


void queueUnitTest(void) {
    QueuePtr q = queueCreate();
    assert(q != NULL);

    /* Reading empty queue: errors. */
    assert(queuePeek(q) == 0);
    assert(queueRemove(q) == 0);

    assert(queueIsEmpty(q));

    int const N = 10;
    for (int i = 0; i < N; i++) {
        queueAdd(q, i);
        assert(queuePeek(q) == 0);
    }

    assert(!queueIsEmpty(q));

    for (int i = 0; i < N; i++) {
        assert(queueRemove(q) == i);
    }

    assert(queueIsEmpty(q));

    queueDestroy(q);
    printf("Queue unit test completed successfully.\n");
}
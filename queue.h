// queue.h
/*
Queue - FIFO container.
By Anton Babkin
*/

#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdbool.h>

typedef struct Queue *QueuePtr;

/* Initialize empty queue. */
QueuePtr queueCreate(void);

/* Deallocate queue memory. */
void queueDestroy(QueuePtr);

bool queueIsEmpty(QueuePtr s);

/* Return element from front of the queue. */
int queuePeek(QueuePtr s);

/* Add element to end of the queue. */
void queueAdd(QueuePtr s, int item);

/* Remove and return element from front of the queue. */
int queueRemove(QueuePtr s);

void queueUnitTest(void);

#endif /* __QUEUE_H */
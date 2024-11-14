#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX_LEN 1000

typedef struct Queue
{
    int *arr[MAX_LEN];
    int front, back;
} Queue;

Queue init()
{
    Queue q;
    q.front = 0;
    q.back = MAX_LEN - 1;
    return q;
}

bool isEmpty(Queue q)
{
    return q.front == (q.back + 1) % MAX_LEN;
}

bool isFull(Queue q)
{
    return q.front == (q.back + 2) % MAX_LEN;
}

int *front(Queue q)
{
    if (isEmpty(q))
    {
        printf("front: empty Queue");
        return NULL;
    }
    return q.arr[q.front];
}

Queue enqueue(Queue q, int *key)
{
    if (isFull(q))
    {
        printf("enqueue: Queue is full\n");
        return q;
    }
    q.back = (++q.back) % MAX_LEN;
    q.arr[q.back] = key;
    return q;
}

Queue dequeue(Queue q)
{
    if (isEmpty(q))
    {
        printf("dequeue: empty Queue\n");
        return q;
    }
    q.front = (++q.front) % MAX_LEN;
    return q;
}

/*
 * ./linkedlist.c
 * Defines linked list and queue methods.
 * Created: 2020-03-04 20:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "linkedlist.h"
#include <stdio.h>
#include <string.h>

/*
 * Creates a new singly linked list node that holds the given data.
 */
Node
*node_new(void *data)
{
	Node *node = malloc(sizeof(Node));	/* create the node */
	node->data = data;	/* place the data in the node */
	node->next = NULL;	/* initially, the next node will be NULL */
	return node;	/* return the result */
} /* end *node_new(void*) */

/**
 * Creates a new FIFO queue.
 */
Queue
*queue_new()
{
	Queue *queue = malloc(sizeof(Queue));	/* create the queue */
	Node *phead = node_new(NULL);	/* create the head pointer */
	queue->phead = phead;	/* set the head pointer */
	queue->tail = phead;	/* the tail is also the head pointer */
} /* end *queue_new() */

/**
 * Adds a new element at the end of the queue.
 * @params
 *   *queue       : Queue = the queue whereto to add
 *   *new_element : Node  = the node for the new element to add
 */
void
queue_enqueue(Queue *queue, Node *new_element)
{
	queue->tail->next = new_element;	/* set the next node of the current tail node */
	queue->tail = new_element;	/* set the new tail node */
} /* end *queue_enqueue(Queue*, Node*) */

/**
 * Removes the element at the head of the queue.
 * @params
 *   *queue : Queue = the queue wherefrom to remove
 * @returns the node dequeued
 */
Node
*queue_dequeue(Queue *queue)
{
	/* copy the element to dequeue */
	Node *dequeued = queue->phead->next;
	/* shift the head pointer to the next node */
	queue->phead->next = dequeued->next;
	return dequeued;	/* return the node of the element dequeued */
} /* end *queue_dequeue(Queue*) */

/**
 * @returns if the given queue is empty.
 */
bool
queue_is_empty(Queue *queue)
{
	return (NULL == queue->phead->next);
}

/**
 * @returns if the given queue has a next element.
 */
bool
queue_has_next(Queue *queue)
{
	return (!queue_is_empty(queue));
}

/**
 * Tests the various linked list and queue functions.
 */
void
main(int argc, char **argv)
{
	char *samples[] = { "0", "1", "4", "9", "16", "25", NULL };
	char **psample;
	Queue *queue = queue_new();
	char *string;
	Node *dequeued;

	for (psample = samples; *psample; ++psample) {
		printf("%lu\n", (long unsigned)(*psample));
		queue_enqueue(queue, node_new(*psample));
	}
	printf("\n");

	int k = 0;
	while (queue_has_next(queue)) {
		dequeued = queue_dequeue(queue);
		string = dequeued->data;
		free(dequeued);
		printf("%d\t%s\n", k++, string);
	}
} /* end main(int, char**) */


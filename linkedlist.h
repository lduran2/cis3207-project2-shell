/*
 * ./linkedlist.h
 * Defines linked list nodes and queues, and declares their functions.
 * Created: 2020-03-04 20:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include <stdlib.h>
#include <stdbool.h>

/**
 * Structure for basic singly linked list node.
 */
typedef struct Node {
	/* data contained in the node */
	void *data;
	/* the succeeding node */
	struct Node *next;
} Node;

/*
 * Creates a new singly linked list node that holds the given data.
 */
Node *node_new(void *data);

/**
 * Structure for FIFO queue.
 */
typedef struct Queue {
	/* pointer to the head */
	Node *phead;
	/* the tail node */
	Node *tail;
} Queue;

/**
 * Creates a new FIFO queue.
 */
Queue *queue_new();

/**
 * Adds a new element at the end of the queue.
 * @params
 *   *queue       : Queue = the queue whereto to add
 *   *new_element : Node  = the node for the new element to add
 */
void queue_enqueue(Queue *queue, Node *new_element);

/**
 * Removes the element at the head of the queue.
 * @params
 *   *queue : Queue = the queue wherefrom to remove
 * @returns the node dequeued
 */
Node *queue_dequeue(Queue *queue);

/**
 * @returns if the given queue is empty.
 */
bool queue_is_empty(Queue *queue);

/**
 * @returns if the given queue has a next element.
 */
bool queue_has_next(Queue *queue);


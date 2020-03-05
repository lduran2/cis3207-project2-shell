/*
 * ./test-linkedlist.c
 * Tests linked list queues.
 * Created: 2020-03-04 23:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "linkedlist.h"
#include <stdio.h>
#include <string.h>

/**
 * Tests the various linked list and queue functions.
 */
void
main(int argc, char **argv)
{
	/* array and pointer to samples */
	char *samples[] = { "0", "1", "4", "9", "16", "25", NULL };
	char **psample;

	/* the queue */
	Queue *queue = queue_new();
	/* the dequeued node */
	Node *dequeued;
	/* the dequeued string */
	char *string;

	/* a counter */
	int k;

	/* array copy of queue */
	char **array;

	/* test enqueue */
	printf("enqueue:\n");
	for (psample = samples; *psample; ++psample) {
		printf("%lu\n", (long unsigned)(*psample));
		queue_enqueue(queue, node_new(*psample));
	}
	printf("\tlength: %lu\n", queue_length(queue));
	printf("\n");

	/* test to array */
	queue_to_array(queue, (void***)&array, sizeof(char*));

	/* test dequeue */
	printf("dequeue:\n");
	k = 0;
	while (queue_has_next(queue)) {
		dequeued = queue_dequeue(queue);
		string = dequeued->data;
		free(dequeued);
		printf("%d\t%s\n", k++, string);
	}
	printf("\tlength: %lu\n", queue_length(queue));
	printf("\n");

	/* print the array afterward */
	printf("to array:\n");
	k = 0;
	for (; *array; ++array) {
		printf("%d\t%s\n", k++, (char*)*array);
	}
	printf("\n");
} /* end main(int, char**) */


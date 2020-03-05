/*
 * ./parser.c
 * Parses the command line.
 * Created: 2020-03-04 23:50
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"

bool
in_class(char *needle, char **haystack)
{
	int len;	/* stores the haystack length */
	/* loop through the class characters */
	for (; *haystack; ++haystack) {
		/* find length of the current class character */
		len = strlen(*haystack);
		/* compare to the current needle character */
		if (0 == strncmp(*haystack, needle, len)) {
			/* if found, return try */
			return true;
		}
	} /* end for (; *haystack; ) */
	/* otherwise, it was not found, so return false */
	return false;
} /* end in_class(char*, char**) */

void
parse(char *haystack, int *argc, char ***argv)
{
	/* queue to store the unknown number of arguments temporarily */
	Queue *nargv = queue_new();

	/* the comment class */
	char *comment[] = { "#", NULL };

	/* for creating new tokens */
	char *offset = haystack;	/* offset within haystack */
	int len;	/* length of the new token */
	char *new_token = NULL;	/* the new token */

	/* loop until end of haystack or first comment character */
	for (; *haystack && !in_class(haystack, comment); ++haystack) {
	}

	/* copy the new token */
	len = haystack - offset - 1;
	new_token = malloc(len * sizeof(char));
	strncpy(new_token, offset, len);
	/* enqueue the new token */
	queue_enqueue(nargv, node_new(new_token));

	/* set argc and argv */
	*argc = (int)queue_length(nargv);
	/* argv is an array copy of nargv */
	queue_to_array(nargv, (void***)argv, sizeof(char*));
} /* end parse(char *haystack, int *argc, char ***argv) */

void
main(int argc, char** argv)
{
	char *haystack = "hello, world! #how are you?";
	int pargc;
	char **pargv;

	parse(haystack, &pargc, &pargv);
	printf("%d:%s\n", pargc, pargv[0]);
}


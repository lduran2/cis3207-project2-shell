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

char
*subclass_of(char *needle, char **haystack)
{
	int len;	/* stores the haystack length */
	/* loop through the class strings */
	for (; *haystack; ++haystack) {
		/* find length of the current class string */
		len = strlen(*haystack);
		/* compare to the current needle string*/
		if (0 == strncmp(*haystack, needle, len)) {
			/* if found, return the string in haystack */
			return *haystack;
		}
	} /* end for (; *haystack; ) */
	/* otherwise, it was not found, so return NULL */
	return NULL;
} /* end *subclass_of(char*, char**) */

bool
in_class(char *needle, char **haystack)
{
	/* the needle is in the haystack class */
	/* if its equivalent subclass is not NULL */
	return (NULL != subclass_of(needle, haystack));
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

	/* loop until end of haystack */
	for (; *haystack; ++haystack) {
		/* break on first comment string */
		if (in_class(haystack, comment)) {
			break;
		} /* end if (in_class(haystack, comment)) */
	} /* end for (; *haystack; ) */

	/* copy the new token */
	len = haystack - offset;
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
	char *haystacks[] = {
		"hello, world! #how are you?",
		"hello, world!#how are you?",
		"hello, world!",
		"#how are you?",
		NULL
	};
	char **phaystack;
	int pargc;	/* parsed argc */
	char **pargv;	/* parsed argv */

	for (phaystack = haystacks; *phaystack; ++phaystack) {
		parse(*phaystack, &pargc, &pargv);
		printf("%d:\"%s\"\n", pargc, pargv[0]);
	}
}


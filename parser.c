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
push_next_token(Queue *queue, char *offset, char *end)
{
	int len;	/* length of the new token */
	char *new_token = NULL;	/* the new token */

	/* calculate and check the length */
	len = end - offset;
	/* if the length is 0, then don't push */
	if (0 == len) return;

	/* copy the new token */
	new_token = malloc(len * sizeof(char));
	strncpy(new_token, offset, len);
	/* enqueue the new token */
	queue_enqueue(queue, node_new(new_token));
}

void
parse(char *haystack, int *argc, char ***argv)
{
	/* queue to store the unknown number of arguments temporarily */
	Queue *nargv = queue_new();

	/* the escape prefix class */
	char *escape[] = { "\\", NULL };
	/* the quotation delimiter class */
	char *delims[] = { "\"", "'", NULL };
	/* the comment prefix class */
	char *comment[] = { "#", NULL };
	char *separators[] = { " ", "\t", "\r", "\n", NULL };

	/* subclass of the delimiters that the current haystack is in */
	char *delim_subclass;
	/* whether the state is in the separator class */
	bool in_separator = false;

	/* for creating new tokens */
	char *offset = haystack;	/* offset within haystack */

	/* loop until end of haystack */
	while (*haystack) {
		if (!in_separator) {
			/* if the current character is in the */
			/* delimiter class */
			delim_subclass = subclass_of(haystack, delims);
			if (NULL != delim_subclass) {
				/* seek past the next instance */
				/* of the delimiter */
				haystack = strstr(haystack + 1,
					delim_subclass) + 1;
				continue;
			} /* end if (NULL != delim_subclass) */

			/* break on first comment string */
			if (in_class(haystack, comment)) {
				break;
			} /* end if (in_class(haystack, comment)) */

			if (in_class(haystack, separators)) {
				push_next_token(nargv,
					offset, haystack);
				in_separator = true;
			}

			++haystack;
		}
		else {
			if (in_class(haystack, separators)) {
				++haystack;
			}
			else {
				offset = haystack;
				in_separator = false;
			}
		} /* end switch ((int)in_separator) */
	} /* end while (*haystack) */

	push_next_token(nargv, offset, haystack);

	/* set argc and argv */
	*argc = (int)queue_length(nargv);
	/* argv is an array copy of nargv */
	queue_to_array(nargv, (void***)argv, sizeof(char*));
} /* end parse(char *haystack, int *argc, char ***argv) */

void
main(int argc, char** argv)
{
	int k;

	char *haystacks[] = {
		"hello, world! #how are you?",
		"hello, world!#how are you?",
		"hello, world!",
		"#how are you?",
		"hello 'world, #how are you?'",
		"hello \"world, #how are you?\"",
		"hello \\'world, #how are you?\\'",
		"hello \\\"world, #how are you?\\\"",
		NULL
	};
	char **phaystack;	/* pointer to haystacks */
	int pargc;	/* parsed argc */
	char **pargv;	/* parsed argv */

	for (phaystack = haystacks; *phaystack; ++phaystack) {
		parse(*phaystack, &pargc, &pargv);
		printf("%d:\"%s\"\n", pargc, *phaystack);
		for (k = 0; k < pargc; ++k) {
			printf("\t\"%s\"\n", pargv[k]);
		}
	}
}


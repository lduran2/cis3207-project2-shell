/*
 * ./parser.c
 * Parses the command line.
 * Created: 2020-03-04 23:50
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "parser.h"

/**
 * @returns the subclass that the *needle is a member of the
 * superset **haystack.
 * @params
 *   *needle    : char = the string to classify
 *   **haystack : char = the superset representing the class
 */
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

/**
 * @returns true if the *needle is a member of the superclass
 * **haystack.
 * @params
 *   *needle    : char = the string to classify
 *   **haystack : char = the superset representing the class
 */
bool
in_class(char *needle, char **haystack)
{
	/* the needle is in the haystack class */
	/* if its equivalent subclass is not NULL */
	return (NULL != subclass_of(needle, haystack));
} /* end in_class(char*, char**) */

/**
 * Enqueues the string between *offset inclusive and *end exclusive.
 * @params
 *   *queue  : Queue = the queue whereto, to add
 *   *offset : char  = the beginning of the subtring (inclusive)
 *   *end    : char  = the end of the substring (exclusive)
 */
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
} /* end push_next_token(Queue*, char*, char*) */

/**
 * Parses the string *haystack and stores the number of tokens in
 * *pargc and the tokens themselves in ***pargv.
 * @params
 *   *haystack : char = the string to search
 *   *pargc    : int  = pointer to the argument count
 *   ***pargv  : char = pointer to the argument values
 *                      (NULL terminated)
 * @returns true on success, false on failure.
 */
bool
parse(char *haystack, int *pargc, char ***pargv)
{
	/* queue to store the unknown number of arguments temporarily */
	Queue *qargv = queue_new();

	/* the quotation delimiter class */
	/* substrings in quotation delimiters are treated as single */
	/* characters */
	char *delims[] = { "\"", "'", NULL };
	/* the comment prefix class */
	/* these characters make the end of a line */
	char *comment[] = { "#", NULL };
	/* the white space separator class */
	/* these strings split tokens up */
	char *separators[] = { " ", "\t", "\r", "\n", NULL };
	/* the self token class */
	/* these strings are tokens onto themselves */
	char *tokens[] = { "<", "|", ">>", ">", "&", NULL };

	/* subclass of the delimiters that the current haystack is in */
	char *delim_subclass;
	/* whether the state is in the separator class */
	bool in_separator = false;
	/* subclass of tokens that the current haystack is in */
	char *token_subclass;

	/* for creating new tokens */
	char *offset = haystack;	/* offset within haystack */

	/* loop until end of haystack */
	while (*haystack) {
		/* state: in regular (non-separator) text */
		if (!in_separator) {
			/* if the current string is in the */
			/* delimiter class */
			delim_subclass = subclass_of(haystack, delims);
			if (NULL != delim_subclass) {
				/* seek past the next instance */
				/* of the delimiter */
				haystack = strstr(haystack + 1,
					delim_subclass);
				/* if the next instance is not found*/
				if (NULL == haystack) {
					/* print an error */
					fprintf(stderr, "Unmatched %s.\n", delim_subclass);
					/* return failure */
					return false;
				};
				++haystack;
				continue;
			} /* end if (NULL != delim_subclass) */

			/* break on first comment string */
			if (in_class(haystack, comment)) {
				break;
			} /* end if (in_class(haystack, comment)) */

			/* if the current string is in the */
			/* separator class  */
			if (in_class(haystack, separators)) {
				/* push the string so far into the stack */
				push_next_token(qargv, offset, haystack);
				/* update the state */
				in_separator = true;
			} /* end if (in_class(haystack, separators)) */

			/* if the current string is in the */
			/* token class */
			token_subclass = subclass_of(haystack, tokens);
			if (NULL != token_subclass) {
				/* push the string so far onto stack */
				push_next_token(qargv, offset, haystack);
				/* push the token too */
				queue_enqueue(qargv, node_new(token_subclass));
				/* seek after the token */
				haystack += strlen(token_subclass);
				/* update the offset */
				offset = haystack;
				/* stay on the new character */
				continue;
			} /* end if (NULL != token_subclass) */
		} /* end if (!in_separator) */
		/* state: in the separator class */
		else {
			/* if the current string is STILL in the */
			/* separator class */
			if (in_class(haystack, separators)) {
				++haystack;
			} /* end if (in_class(haystack, separators)) */
			/* otherwise */
			else {
				/* exit the in_separator state */
				in_separator = false;
				/* update the offset */
				offset = haystack;
				/* stay on same character */
				continue;
			} /* end if (!in_class(haystack, separators)) */
		} /* end if (in_separator) */

		/* priority logic: continue to next character */
		++haystack;
	} /* end while (*haystack) */

	/* push the last token onto the stack */
	push_next_token(qargv, offset, haystack);

	/* set pargc and pargv */
	*pargc = (int)queue_length(qargv);
	/* pargv is an array copy of qargv */
	queue_to_array(qargv, (void***)pargv, sizeof(char*));

	/* return success */
	return true;
} /* end parse(char *haystack, int *pargc, char ***pargv) */

/**
 * Splits an argument list by the redirection tokens.  Each new element
 * starts with a redirection token, except the first which starts with
 * an empty string.
 * @params
 *   **argv : char = the argument list
 * @returns the queue of redirection processes.
 */
Queue
*process_splitter(char **argv) {
	/* the redirection tokens */
	char *tokens[] = { "<", "|", ">>", ">", "&", NULL };

	char **curr;	/* the current argument */

	/* arrays were not playing nice here, so I went with queues */
	/* also, access will be sequential either way */
	Queue *processes = queue_new();	/* all processes */
	Queue *process = queue_new();	/* each process */
	/* arguments will always start at 1 */
	queue_enqueue(process, node_new(""));

	/* for each argument */
	for (curr = argv; *curr; ++curr) {
		/* if the argument is a redirection token: */
		if (in_class(*curr, tokens)) {
			/* enqueue the process */
			queue_enqueue(processes, node_new(process));
			/* start a new one */
			process = queue_new();
		} /* end if (in_class(*curr, tokens)) */
		queue_enqueue(process, node_new(*curr));
	} /* end for (curr = argv; *curr; ++curr) */
	/* enqueue the final process */
	queue_enqueue(processes, node_new(process));

	return processes;

} /* end *process_splitter(char**) */


/*
 * ./shell.c
 * The shell program is an interactive buffer between the user and the system.
 * Created: 2020-02-26 15:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Wraps the getline method by giving the user a prompt first.
 * @params
 *   *prompt   : char    = displayed to the user before accepting input
 *   **lineptr : char    = buffer for input string
 *   *n        : size_t  = size of the input string
 *   *instream : FILE    = the input stream 
 */
bool
promptline(char* prompt, char **lineptr, size_t *n, FILE *instream)
{
	fprintf(stderr, "%s", prompt);	/* write the prompt */
	size_t out = getline(lineptr, n, stdin);	/* accept a new line */
	/* continue conditions */
	return (
		(*lineptr[0] != '\0')	/* user did not press Ctrl+D */
		/* user did not type exit */
		&& (0 != strcmp(*lineptr, "exit\n"))
		/* user did not type quit */
		&& (0 != strcmp(*lineptr, "quit\n"))
	);
} /* end promptline(char*, char**, size_t*, FILE*) */

void
main(int argc, char **argv)
{
	/* let getline allocate the line buffer */
	char *line = NULL;	/* line read in */
	size_t line_len = 0;	/* line length */
	FILE* in = stdin;	/* input stream */

	/* while prompting the user */
	while (promptline("$ ", &line, &line_len, in)) {
		/* echo input */
		printf("%lu\n", line_len);
	} /* while (promptline) */
} /* end main(int, char**) */


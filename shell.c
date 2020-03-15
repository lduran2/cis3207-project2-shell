/*
 * ./shell.c
 * The shell program is an interactive buffer between the user and the system.
 * Created: 2020-02-26 15:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "parser.h"

/**
 * Wraps the getline method by giving the user a prompt first.
 * @params
 *   *prompt   : char    = displayed to the user before accepting input
 *   **lineptr : char    = buffer for input string
 *   *n        : size_t  = size of the input string
 *   *instream : FILE    = the input stream 
 */
bool
promptline(char* prompt, int *pargc, char ***pargv, FILE *instream)
{
	char *line = NULL;	/* line read in */
	size_t n = 0;	/* line length */

	/* prompt and read line */
	fprintf(stderr, "%s", prompt);	/* write the prompt */
	size_t out = getline(&line, &n, stdin);	/* accept a new line */
	/* Ctrl+D: shortcut for exit */
	if (*line == '\0') { /* the user pressed Ctrl+D */
		printf("exit\n");
		return false;

	} /* if (*line == '\0') */

	/* get the length */
	n = strlen(line);
	/* parse the line */
	parse(line, pargc, pargv);

	/* continue conditions */
	return (
		/* user did typed the exit command */
		(0 != strcmp(**pargv, "exit"))
		/* user did typed the quit command */
		&& (0 != strcmp(**pargv, "quit"))
	);
} /* end promptline(char*, char**, size_t*, FILE*) */

int
main(int margc, char **margv)
{
	/* let getline allocate the line buffer */
	char **argv;	/* argument values */
	int argc;	/* argument count */
	FILE* in = stdin;	/* input stream */

	/* while prompting the user */
	while (promptline("$ ", &argc, &argv, in)) {
	} /* while (promptline) */
} /* end main(int, char**) */


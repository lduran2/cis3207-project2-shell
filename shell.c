/*
 * ./shell.c
 * The shell program is an interactive buffer between the user and the system.
 * Created: 2020-02-26 15:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "parser.h"
#include "locator.h"
#include <sys/wait.h>

/**
 * Wraps the getline method by giving the user a prompt first.
 * @params
 *   *prompt   : char = displayed to the user before accepting input
 *   *pargc    : int  = pointer to argument count
 *   ***pargv  : char = pointer to parsed argument values
 *                         (NULL terminated)
 *   *success  : bool = whether parsing was successful
 *   *instream : FILE = the input stream
 * @returns true if no exit code; false on exit code.
 */
bool
promptline(
	char* prompt, int *pargc, char ***pargv, bool *success,
	FILE *instream)
{
	char *line = NULL;	/* line read in */
	char *raw_line = NULL;	/* line read in, raw from getline */
	size_t n = 0;	/* line length */

	/* prompt and read line */
	fprintf(stderr, "%s", prompt);	/* write the prompt */
	/* accept a new raw line */
	size_t out = getline(&raw_line, &n, stdin);

	/* get the length removing the line feed '\n' */
	n = (strlen(raw_line) - 1);
	/* Ctrl+D: shortcut for exit */
	if (0 == n) { /* the user pressed Ctrl+D */
		*success = true;	/* report success */
		printf("exit\n");	/* show exit */
		return false;	/* and exit */
	} /* if (0 == n) */

	/* properly terminate the line */
	line = malloc((n + 1) * sizeof(char));
	strncpy(line, raw_line, n);
	strcat(line, "\0");

	/* parse the line, but on error: */
	*success = parse(line, pargc, pargv); /* report if success */
	if (!*success) {
		return true;	/* do NOT exit on error though */
	} /* if (!*success) */

	/* continue conditions */
	return (
		/* user did typed the exit command */
		(0 != strcmp(**pargv, "exit"))
		/* user did typed the quit command */
		&& (0 != strcmp(**pargv, "quit"))
	);
} /* end promptline(char*, char**, size_t*, bool*, FILE*) */

int
main(int margc, char **margv)
{
	char **argv;	/* argument values */
	int argc;	/* argument count */
	bool success;	/* whether parsing was successful */
	FILE* in = stdin;	/* input stream */
	char *absolute_path;	/* absolute path of command */
	bool execute;	/* whether command is executable */

	int status;	/* child exit status */

	/* while prompting the user */
	while (promptline("$ ", &argc, &argv, &success, in)) {
		/* if not successful parsing, just skip to next prompt */
		if (!success) continue;
		/* test executability */
		execute = locate(*argv, &absolute_path);
		/* if the absolute path is not executable, */ 
		/* skip to next prompt */
		if (!execute) continue;

		printf("%s\t%d\n", absolute_path, argc);
		for (char **parg = argv; *parg; ++parg) {
			printf("\t%p:%s:\n", parg, *parg);
		}
		continue;

		/* fork: */
		if (fork() == 0) { /* child */
			execvp(absolute_path, argv);
		}
		else { /* parent */
			status = 0;
			wait(&status);
			printf("Child exited with status of %d.\n", status);
		}
	} /* while (promptline) */
} /* end main(int, char**) */


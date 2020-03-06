/*
 * ./test-parser.c
 * Tests the parser against some test strings.
 * Created: 2020-03-05 19:30
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "parser.h"

void
main(int argc, char** argv)
{
	int k;	/* the argument printer counter */

	/* the test strings to parse */
	char *haystacks[] = {
		"hello, world! #how are you?",
		"hello, world!#how are you?",
		"hello, world!",
		"#how are you?",
		"hello 'world, #how are you?'",
		"hello \"world, #how are you?\"",
		"hello \\'world, #how are you?\\'",
		"hello \\\"world, #how are you?\\\"",
		"hello > world!",
		"hello >> world!",
		"hello < world!",
		"hello | world!",
		"hello & world",
		"hello & world \"",
		NULL
	}; /* end char *haystacks[] */

	char **phaystack;	/* pointer to haystacks */
	int pargc;	/* parsed argc */
	char **pargv;	/* parsed argv */

	/* for each of the haystacks strings */
	for (phaystack = haystacks; *phaystack; ++phaystack) {
		/* parse the string */
		parse(*phaystack, &pargc, &pargv);
		/* print the argument and original string */
		printf("%d:\"%s\"\n", pargc, *phaystack);
		/* print each argument */
		for (k = 0; (k < pargc); ++k) {
			printf("\t\"%s\"\n", pargv[k]);
		} /* for (; (k < pargc); ) */
	} /* for (; *phaystack; ) */
} /* end main(int, char*) */


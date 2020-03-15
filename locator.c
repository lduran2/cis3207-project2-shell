/*
 * ./locator.c
 * Locator for commands used in the shell.
 * Created: 2020-03-15 13:01
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "locator.h"

/**
 * Locate the *relative path and store it in **location.
 * @params
 *   *relative  : char = the relative path
 *   **location : char = the resulting location
 * @returns true on success, false on failure
 */
bool locate(char *relative, char **location) {
	int result;	/* result of accessing relative */
	struct stat statbuf;	/* statistics of relative path */
	bool isreg;	/* whether relative is a regular file */
	char *local_location;	/* local copy of the location */
	char *error;	/* error message format */

	/* check if executable */
	result = access(relative, X_OK);
	/* check statistics of relative path */
	isreg = (0==stat(relative, &statbuf));
	/* check if relative path is a regular file */
	isreg = (isreg && S_ISREG(statbuf.st_mode));

	/* if executable regular file */
	if ((0 == result) && isreg) {
		*location = relative;	/* send relative as location */
		return true;	/* return success */
	} /* end if ((0 == result) && isreg) */

	/* if not, check if extant */
	result = access(relative, F_OK);
	if (0 == result) {
		/* if extant, the error was permission */
		error = "%s: Permission denied.\n";
	} /* end if (0 == result) */
	else {
		/* otherwise, the file ws not found */
		error = "%s: Command not found.\n";
	} /* end if (0 != result) */

	*location = NULL;	/* send no location */
	fprintf(stderr, error, relative);	/* print the rror message */
	return false;	/* return failure */
} /* end locate(char*, char**) */


/*
 * ./locator.c
 * Locator for commands used in the shell.
 * Created: 2020-03-15 13:01
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "locator.h"

/** environmental variables */
extern char **environ;

char *find_env(char *env) {
	size_t length = (strlen(env) + 1);
	char *value = malloc(length * sizeof(char));
	strcpy(value, env);
	strcat(value, "=");

	char **local_env = environ;
	for (; *local_env; ++local_env) {
		if (0 == strncmp(*local_env, value, length)) {
			return *local_env + length;
		}
	}
	return NULL;
}

typedef enum {
	EXECUTABLE,
	PERMISSION_DENIED,
	COMMAND_NOT_FOUND
} Executability;

/**
 * Tests the executability of a file.
 * @params
 *   *file : char = the file to test
 * @returns whether the given file is executable, or if not, then the
 * resulting error.
 */
Executability is_executable(char *file) {
	int result;	/* result of accessing relative */
	struct stat statbuf;	/* statistics of relative path */
	bool isreg;	/* whether relative is a regular file */

	/* check if executable */
	result = access(file, X_OK);
	/* check statistics of relative path */
	isreg = (0==stat(file, &statbuf));
	/* check if relative path is a regular file */
	isreg = (isreg && S_ISREG(statbuf.st_mode));

	/* if executable regular file */
	if ((0 == result) && isreg) {
		return EXECUTABLE;	/* return success */
	} /* end if ((0 == result) && isreg) */

	/* if not, check if extant */
	result = access(file, F_OK);
	if (0 == result) {
		/* if extant, the error was permission */
		return PERMISSION_DENIED;
	} /* end if (0 == result) */
	else {
		/* otherwise, the file ws not found */
		return COMMAND_NOT_FOUND;
	} /* end if (0 != result) */
} /* end is_executable(char*) */

/**
 * Locate the *relative path and store it in **location.
 * @params
 *   *relative  : char = the relative path
 *   **location : char = the resulting location
 * @returns true on success, false on failure
 */
bool locate(char *relative, char **location) {
	/* error message formats based on executability */
	static char *executability_formats[] = {
		"%s: To be executable.\n",
		"%s: Permission denied.\n",
		"%s: Command not found.\n"
	};

	/* whether the statics were initialized already */
	static bool initialized = false;

	/* direct paths */
	static const int N_DIRECT_PATHS = 3;	/* number of these */
	static char *direct_paths[] = {
		"/",
		"./",
		"../",
	};
	static size_t direct_path_lens[3];	/* length of each */


	static const char *HOME = "~/";	/* unexpanded home path */
	static size_t home_len;	/* length of home path */

	int k;	/* index (for direct paths) */

	char *home_expand;	/* expansion of home path */
	char *local_location;	/* local copy of location */
	int length;	/* length of location */

	/* intialize lengths once */
	if (!initialized) {
		/* for each directed path */
		for (k = 0; (N_DIRECT_PATHS != k); ++k) {
			/* save its length */
			direct_path_lens[k] = strlen(direct_paths[k]);
		} /* for (; (N_DIRECT_PATHS != k); ) */
		home_len = strlen(HOME);	/* find length of home path */
		initialized = true;	/* initalized once */
	} /* end if (!initialized) */

	/* base case */
	/* check if "/", "./", "../" */
	/* loop through direct paths */
	for (k = 0; (N_DIRECT_PATHS != k); ++k) {
			/* check the current direct path against *relative */
			if (0 == strncmp(relative,
			direct_paths[k], direct_path_lens[k]))
		{
			/* check its executability */
			Executability result = is_executable(relative);
			switch (result) {
				/* if executable */
				case (EXECUTABLE):
					/* send relative as location */
					*location = relative;
					/* return success */
					return true;
				break;
				/* otherwise, not executable */
				default:
					/* send no location */
					*location = NULL;
					/* print the error message */
					fprintf(stderr,
						executability_formats[result],
						relative);
					/* return failure */
					return false;
				break;
			} /* end switch (result) */
		} /* end if (0 == strncmp(relative,
			direct_paths[k], direct_path_lens[k]))
		   */
	} /* for (; (N_DIRECT_PATHS != k); ) */

	/* check if "~/" */
	if (0 == strncmp(relative, HOME, home_len)) {
		/* shift relative to remove home path */
		relative += (home_len - 1);
		home_expand = find_env("HOME");	/* lookup HOME */
		/* calculate the combined length */
		length = (strlen(home_expand) + strlen(relative));
		/* allocate a string of that length */
		local_location = malloc((length)*sizeof(char));
		/* copy the home expansion */
		strcpy(local_location, home_expand);
		/* concatenate the shifted relative */
		strcat(local_location, relative);
		/* locate the result and return */
		return locate(local_location, location);
	} /* end if (0 == strncmp(relative, HOME, home_len)) */
	

	char *paths = find_env("PATH");
	char *curr_path;
	char *next_path;
	int path_len;
	Executability best_executability = 4;
	char* best_path = NULL;
	
	Executability result;

	for (; paths; paths = next_path) {
		next_path = strstr(paths, ":");
		if (!next_path) break;
		++next_path;
		path_len = (next_path - paths - 1);
		curr_path = malloc(path_len*sizeof(char));
		strncpy(curr_path, paths, path_len);
	}
} /* end locate(char*, char**) */


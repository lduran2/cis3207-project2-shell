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

/**
 * @returns the value of an environment variable given its name.
 */
char *find_env(char *env)
{
	char *key;	/* key to search for */
	size_t length;	/* length of the key */
	char **local_env	/* current environment statement */

	/* append the '=' to the env variable name for the key */
	length = (strlen(env) + 1);
	key = malloc(length * sizeof(char));
	strcpy(key, env);
	strcat(key, "=");

	/* search for the environment value */
	/* for each environment statement: */
	for (**local_env = environ; *local_env; ++local_env) {
		/* check if it begins with the key */
		if (0 == strncmp(*local_env, value, length)) {
			/* if so, return the part after the '=' */
			return *local_env + length;
		} /* end if (0 == strncmp(*local_env, value, length)) */
	} /* end for (; *local_env; ) */
	/* otherwise, return NULL */
	return NULL;
} /* end find_env(char*) */

/**
 * Executability types.
 */
typedef enum {
	EXECUTABLE,	/* a file is executable */
	PERMISSION_DENIED,	/* a file exists, but is not regular or */
		/* cannot be executed */
	COMMAND_NOT_FOUND /* the file cannot be found */
} Executability;

/**
 * Tests the executability of a file.
 * @params
 *   *file : char = the file to test
 * @returns whether the given file is executable, or if not, then the
 * resulting error.
 */
Executability is_executable(char *file)
{
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
 * Contextualize a relative path.
 * @param
 *   *context  : char = the absolute path context
 *   *relative : char = the relative file path
 * @returns the contextualized absolute path.
 */
char *contextualize(char *context, char *relative)
{
	char *absolute;	/* the result */
	int length;	/* the length of the result */
	/* calculate the combined length */
	length = (strlen(context) + strlen(relative) + 1);
	/* allocate a string of that length */
	absolute = malloc((length)*sizeof(char));
	/* copy the context */
	strcpy(absolute, context);
	/* concatenate the "/" */
	strcat(absolute, "/");
	/* concatenate the relative */
	strcat(absolute, relative);
	/* return the result */
	return absolute;
} /* end *contextualize(char*, char*) */

/**
 * Locate the *relative path and store it in **location.
 * @params
 *   *relative  : char = the relative path
 *   **location : char = the resulting location
 * @returns true on success, false on failure
 */
bool locate(char *relative, char **location)
{
	/* static variables */

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

	/* for home paths */
	char *home_expand;	/* expansion of home path */
	char *local_location;	/* local copy of location */
	int length;	/* length of location */

	/* for paths that use $PATH */
	char *paths = find_env("PATH");	/* values of $PATH */
	char *curr_path;	/* current value in $PATH */
	char *next_path;	/* next value in $PATH */
	int path_len;	/* length of the current path */
	/* best executability so far */
	Executability best_executability = 4;
	Executability result;	/* current executability */
	/* path corresponding to best executability */
	char *best_path = NULL;
	char *absolute;	/* path corresponding to current executability */

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
			result = is_executable(relative);
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
		relative += home_len;
		home_expand = find_env("HOME");	/* lookup HOME */
		/* contextualize the shifted relative */
		/* to the home expansion */
		local_location = contextualize(home_expand, relative);
		/* locate the result and return */
		return locate(local_location, location);
	} /* end if (0 == strncmp(relative, HOME, home_len)) */

	/* for each of the $PATH */
	for (*paths = find_env("PATH"); paths; paths = next_path) {
		/* look ahead to the next path */
		next_path = strstr(paths, ":");
		/* if these is a next path: */
		if (next_path) {
			/* calculate the path length */
			path_len = (next_path - paths);
			/* allocate the path */
			curr_path = malloc(path_len*sizeof(char));
			/* truncate the path */
			strncpy(curr_path, paths, path_len);
		} /* end if (next_path) */
		else {
			/* otherwise, use the last path */
			curr_path = paths;
		} /* end if (!next_path) */

		/* contextualize with this path */
		absolute = contextualize(curr_path, relative);
		/* check if the absolute path is executable */
		result = is_executable(absolute);

		/* check if better than best executability */
		if (result <= best_executability) {
			/* update if so */
			best_executability = result;
			best_path = absolute;
		} /* end if (result <= best_executability) */
		/* stop if found an executable path */
		if (EXECUTABLE == best_executability) {
			break;
		} /* end if (EXECUTABLE = best_executability) */
		/* stop if no next path */
		if (!next_path) break;
		/* skip next ':' delimiter */
		++next_path;
	} /* end for (; paths; ) */

	/* locate the best path */
	return locate(best_path, location);
} /* end locate(char*, char**) */


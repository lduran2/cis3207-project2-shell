/*
 * ./parser.h
 * Functions for parsing the command line.
 * Created: 2020-03-04 23:50
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"

#ifndef LDURAN2_SHELL_PARSER_H
#define LDURAN2_SHELL_PARSER_H

/**
 * @returns the subclass that the *needle is a member of the
 * superset **haystack.
 * @params
 *   *needle    : char = the string to classify
 *   **haystack : char = the superset representing the class
 */
char *subclass_of(char *needle, char **haystack);

/**
 * @returns true if the *needle is a member of the superclass
 * **haystack.
 * @params
 *   *needle    : char = the string to classify
 *   **haystack : char = the superset representing the class
 */
bool in_class(char *needle, char **haystack);

/**
 * Enqueues the string between *offset inclusive and *end exclusive.
 * @params
 *   *queue  : Queue = the queue whereto, to add
 *   *offset : char  = the beginning of the subtring (inclusive)
 *   *end    : char  = the end of the substring (exclusive)
 */
void push_next_token(Queue *queue, char *offset, char *end);

/**
 * Parses the string *haystack and stores the number of tokens in
 * *pargc and the tokens themselves in ***pargv.
 * @params
 *   *haystack : char = the string to search
 *   *pargc    : int  = pointer to the argument count
 *   ***pargv  : char = pointer to the argument values
 * @return true on success, false on failure.
 */
bool parse(char *haystack, int *pargc, char ***pargv);

/**
 * Splits an argument list by the redirection tokens.  Each new element
 * starts with a redirection token, except the first which starts with
 * an empty string.
 * @params
 *   **argv : char = the argument list
 * @returns the queue of redirection processes.
 */
Queue *process_splitter(char **argv);

#endif


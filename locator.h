/*
 * ./locator.h
 * Functions for locating commands.
 * Created: 2020-03-15 14:15
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>

#ifndef LDURAN2_SHELL_LOCATOR_H
#define LDURAN2_SHELL_LOCATOR_H

bool locate(char *relative, char **location);

#endif


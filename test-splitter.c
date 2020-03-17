/*
 * ./test-splitter.c
 * Tests the process splitter against some test token arrays.
 * Created: 2020-03-15 20:35
 * Author : Leomar Duran <https://github.com/lduran2>
 * For    : CIS 3207, Spring 2020
 */
#include "parser.h"

void
main(int margc, char** margv)
{
	char *argv[] = {
		"ls",
		"-l",
		"-a",
		"/",
		"|",
		"sort",
		"-b",
		"-f",
		">",
		"lsl.srt",
		NULL
	};

	Queue *processes;
	Queue *process;

	processes = process_splitter(argv);

	while (queue_has_next(processes)) {
		process = queue_dequeue(processes)->data;
		while (queue_has_next(process)) {
			printf("%s\t", (char*)queue_dequeue(process)->data);
		}
		printf("\n");
	}
} /* end main(int, char**) */

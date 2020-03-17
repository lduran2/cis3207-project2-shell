# cis3207-project2-shell
A simulation of a shell for a Linux-like system.

## Manual
This shell supports running commands in either the releative paths, or
absolute paths including from the home directory.

In order to run the command, just type it with space-separated
arguments.

This shell also supports quoted multi-world arugments.

It also supports background operations by using the ampersand character
(&).

### Redirection

The background operation works like a normal execution, but the shell
does not wait for execution to finish.

### The environment

The environment supports paths based on both the `$HOME` and `$PATH`
environment variables.

## The solution implementation

The parser is divided into the parsing function that separates
arguments from the original commandline string and the process
splitter, which splits the arguments into redirection processes.

The parser depends on FIFO queues for lists of variable lenght.

The locator works by testing files for existance and executability.
The files are accepted if they begin with `/`, `./` or `../`.
Otherwise, they require a bit of processing.  Paths beginning with `~/`
are translated with the `$HOME` environment variable.  All other paths
are cycled through the paths in the `$PATH` variable until a location
is found.

## Testing

There are testers available for linked list, parser and the process
splitter.

### Linked list

The new, enqueue, to array, dequeue, has next and length methods are
tested for validity using strings of the first 6 nonnegative squares.

### Parser

Various inputs are tested with quotes, comments and redirection tokens.

### Process splitter

A complex set of commandline arguments are tested and the processes
queue is deep printed.


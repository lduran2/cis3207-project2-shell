TESTOUTPUTS = test-*.o test-*.out
ALLOUTPUTS = *.o shell.out $(TESTOUTPUTS)

default: shell

shell: shell.o parser.o linkedlist.o
	gcc $^ -o $@.out

test-parser: test-parser.o parser.o linkedlist.o
	gcc $^ -o $@.out

test-linkedlist: test-linkedlist.o linkedlist.o
	gcc $^ -o $@.out

shell.o: shell.c
	gcc -c $^

test-parser.o: test-parser.c
	gcc -c $^

test-linkedlist.o: test-linkedlist.c
	gcc -c $^

parser.o: parser.c parser.h
	gcc -c $<

linkedlist.o: linkedlist.c linkedlist.h
	gcc -c $<

clean:
	-rm -f $(ALLOUTPUTS)

clean-tests:
	-rm -f $(TESTOUTPUTS)


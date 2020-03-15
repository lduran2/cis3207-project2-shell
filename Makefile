COMPILE = gcc -g
TESTOUTPUTS = test-*.o test-*.out
ALLOUTPUTS = *.o shell.out $(TESTOUTPUTS)

default: shell

shell: shell.o parser.o linkedlist.o locator.o
	$(COMPILE) $^ -o $@.out

test-parser: test-parser.o parser.o linkedlist.o
	$(COMPILE) $^ -o $@.out

test-linkedlist: test-linkedlist.o linkedlist.o
	$(COMPILE) $^ -o $@.out

shell.o: shell.c
	$(COMPILE) -c $^

test-parser.o: test-parser.c
	$(COMPILE) -c $^

test-linkedlist.o: test-linkedlist.c
	$(COMPILE) -c $^

parser.o: parser.c parser.h
	$(COMPILE) -c $<

linkedlist.o: linkedlist.c linkedlist.h
	$(COMPILE) -c $<

locator.o: locator.c locator.h
	$(COMPILE) -c $<

clean:
	-rm -f $(ALLOUTPUTS)

clean-tests:
	-rm -f $(TESTOUTPUTS)

clean-build:
	make clean
	make



CC=g++
CFLAGS=-Wall -g

SOURCES=test_harness.c emulator.cpp

TEST=emulator

test: $(TEST)

all: test

emulator: $(SOURCES)
	$(CC) $(CFLAGS) test_harness.c emulator.cpp -o emulator


clean:
	rm -f $(TEST)

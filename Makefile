CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L

SRC=$(wildcard src/**/*.c src/*.c)
TESTS=$(wildcard tests/*.c)

all:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o bin/main

run:
	./bin/main

test:
	mkdir -p bin
	$(CC) $(CFLAGS) $(TESTS) $(SRC) -o bin/tests
	./bin/tests

clean:
	rm -rf bin/*
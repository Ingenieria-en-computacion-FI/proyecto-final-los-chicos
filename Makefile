CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -Iinclude -D_POSIX_C_SOURCE=200809L

SRC=$(wildcard src/*.c src/algorithms/*.c src/memory/*.c src/scheduler/*.c src/utils/*.c)

.PHONY: all run test clean

all:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) tests/main.c -o bin/main

run: all
	./bin/main

test:
	mkdir -p bin

	@echo "=== Test Queue ==="
	$(CC) $(CFLAGS) tests/test_queue.c \
		src/queue.c \
		src/process.c \
		-o bin/test_queue
	./bin/test_queue

	@echo "=== Test Stack ==="
	$(CC) $(CFLAGS) tests/test_stack.c \
		src/stack.c \
		-o bin/test_stack
	./bin/test_stack

	@echo "=== Test Circular Queue ==="
	$(CC) $(CFLAGS) tests/test_circular_queue.c \
		src/circular_queue.c \
		src/process.c \
		-o bin/test_circular_queue
	./bin/test_circular_queue

	@echo "=== Test Memory Manager ==="
	$(CC) $(CFLAGS) tests/test_memory_manager.c \
		src/memory_manager.c \
		src/doubly_linked_list.c \
		src/process.c \
		src/memory/*.c \
		-o bin/test_memory_manager
	./bin/test_memory_manager

	@echo "=== Test Scheduler ==="
	$(CC) $(CFLAGS) tests/test_scheduler.c \
		src/process.c \
		src/queue.c \
		src/circular_queue.c \
		src/linked_list.c \
		src/stack.c \
		src/scheduler/*.c \
		-o bin/test_scheduler
	./bin/test_scheduler

clean:
	rm -rf bin/*
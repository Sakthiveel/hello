CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g -O0

.PHONY: all run debug check clean
all: hello

hello: main.c
	$(CC) $(CFLAGS) $< -o $@

run: hello
	./hello

debug: hello
	gdb ./hello

hello-sanitize: main.c
	$(CC) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer $< -o $@

check: hello hello-sanitize
	./hello-sanitize
	valgrind --leak-check=full --error-exitcode=1 ./hello

clean:
	rm -f hello hello-sanitize

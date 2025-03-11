SRC = $(wildcard src/*.c)
TEST = $(wildcard test/*.c)
CC = gcc
CFLAGS = -Wall -Werror

build: $(SRC)
	$(CC) src/main.c -o run $(CFLAGS)

run: build
	./run

t: test/main.c test/utils.c test/hash.test.c
	$(CC) test/main.c -o t $(CFLAGS) && ./t && rm ./t

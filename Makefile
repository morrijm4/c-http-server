SRC = $(wildcard src/*.c)

build: $(SRC)
	gcc src/main.c -o run -Wall -Werror

run: build
	./run

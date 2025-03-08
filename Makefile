SRC = alloc.c arena.c dynamic-string.c error.c main.c math.c request.c response.c string.c tcp.c

build: $(SRC)
	gcc main.c -o run -Wall -Werror

run: build
	./run

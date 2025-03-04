build: main.c
	gcc main.c -o run -Wall -Werror

run: build
	./run

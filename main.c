#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include "arena.c"
#include "request.c"
#include "response.c"
#include "tcp.c"

#define ARENA_SIZE 1024

int main() 
{
    int socket_fd = get_tcp_socket();
 
    printf("Listening at http://localhost:%d\n", PORT);
 
    struct sockaddr_storage client_address;
 
    for (;;) {
        socklen_t addr_size = sizeof(client_address);
        memset(&client_address, 0, addr_size);
       
        int conn_fd = accept(socket_fd, (struct sockaddr *)&client_address, &addr_size);
       
        if (conn_fd < 0) {
          perror("accept");
          return EXIT_FAILURE;
        }

	printf("========== Incoming request ========== \n");
        print_address(&client_address);

	// Create file stream for reading 
	FILE *read_stream = fdopen(dup(conn_fd), "r");
	if (read_stream == NULL) {
	    perror("fdopen");
	    return EXIT_FAILURE;
	}

	// Initialize arena allocator
	Arena arena;
	arena_init(&arena, ARENA_SIZE);
       
        // read request
	Request req = {0};
        if (!process_request(&arena, read_stream, &req))
            return EXIT_FAILURE;
	print_request(&req);
	fclose(read_stream);
       
	// Create file stream for writing 
	FILE *write_stream = fdopen(dup(conn_fd), "w");
	if (write_stream == NULL) {
	    perror("fdopen");
	    return EXIT_FAILURE;
	}

        // write request
	send_response(&arena, write_stream, &req);


	fflush(write_stream);
	fclose(write_stream);
	close(conn_fd);
	arena_free(&arena);
    }
 
    close(socket_fd);
    return EXIT_SUCCESS;
}

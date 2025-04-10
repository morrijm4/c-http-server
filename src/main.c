#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <unistd.h>

// #define LOG_CLIENT_ADDRESS
#define LOG_REQUEST
// #define LOG_PARSED_REQUEST
#define LOG_RESPONSE

#include "stack-arena.c"
#include "request.c"
#include "response.c"
#include "tcp.c"

int main() 
{
    int socket_fd = get_tcp_socket();
 
    printf("Listening at http://localhost:%d\n", PORT);
 
    struct sockaddr_storage client_address;
    socklen_t addr_size = sizeof(client_address);
    
    // Initialize arena allocator
    StackArena arena = sa_init();
 
    for (;;) {
        memset(&client_address, 0, addr_size);
       
        int conn_fd = accept(socket_fd, (struct sockaddr *)&client_address, &addr_size);
       
        if (conn_fd < 0) {
          perror("accept");
          return EXIT_FAILURE;
        }

#ifdef LOG_CLIENT_ADDRESS
        print_address(&client_address);
#endif // LOG_CLIENT_ADDRESS

	// Create file stream for reading 
	FILE *read_stream = fdopen(dup(conn_fd), "r");
	if (read_stream == NULL) {
	    perror("fdopen");
	    return EXIT_FAILURE;
	}

       
        // read request
	Request req = {0};
        if (!process_request(&arena, read_stream, &req)) return EXIT_FAILURE;
	fclose(read_stream);

#ifdef LOG_PARSED_REQUEST
	print_request(&req);
#endif // LOG_PARSED_REQUEST
       
	// Create file stream for writing 
	FILE *write_stream = fdopen(dup(conn_fd), "w");
	if (write_stream == NULL) {
	    perror("fdopen");
	    return EXIT_FAILURE;
	}

        // write request and ignore if error if fails
	send_response(&arena, write_stream, &req);


	fclose(write_stream);
	close(conn_fd);
	sa_reset(&arena);
    }

    return EXIT_SUCCESS;
}

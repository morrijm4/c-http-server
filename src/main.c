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
    int socket_fd;
    if (!get_tcp_socket(&socket_fd)) return EXIT_FAILURE;
 
    printf("Listening at http://localhost:%d\n", PORT);
 
    struct sockaddr_storage client_address;
    socklen_t addr_size = sizeof(client_address);
    
    // Initialize arena allocator
    StackArena arena = sa_init();
 
    for (;;) {
        memset(&client_address, 0, addr_size);
       
        int conn_fd = accept(socket_fd, (struct sockaddr *)&client_address, &addr_size);
       
        if (conn_fd < 0) return perror_int("accept");

#ifdef LOG_CLIENT_ADDRESS
        print_address(&client_address);
#endif // LOG_CLIENT_ADDRESS

	// Create file stream for reading 
	FILE *read_stream = fdopen(dup(conn_fd), "r");
	if (read_stream == NULL) return perror_int("fdopen read_stream");

       
        // read request
	Request req = {0};
        if (!process_request(&arena, read_stream, &req)) return EXIT_FAILURE;
	if (fclose(read_stream)) return perror_int("fclose read_stream");

#ifdef LOG_PARSED_REQUEST
	print_request(&req);
#endif // LOG_PARSED_REQUEST
       
	// Create file stream for writing 
	FILE *write_stream = fdopen(dup(conn_fd), "w");
	if (write_stream == NULL) return perror_int("fdopen write_stream");

        // write request and ignore if error if fails
	if (!send_response(&arena, write_stream, &req)) return EXIT_FAILURE;


	if (fclose(write_stream)) return perror_int("fclose read_stream");
	if (close(conn_fd)) return perror_int("close conn_fd");

	sa_reset(&arena);
    }

    return EXIT_SUCCESS;
}

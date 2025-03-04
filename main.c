#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "string.c"

#define PORT 8080
#define LISTEN_BACKLOG 128

typedef enum {
  GET,
  HEAD,
} Method;

typedef struct {
  Method method;
} Request;

int get_tcp_socket() {
  int socket_fd;
  int yes = 1;

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in hostaddr = {0};
  hostaddr.sin_family = AF_INET;
  hostaddr.sin_addr.s_addr = INADDR_ANY;
  hostaddr.sin_port = htons(PORT);

  if ((bind(socket_fd, (struct sockaddr *)&hostaddr, sizeof(struct sockaddr_in))) <
      0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if ((listen(socket_fd, LISTEN_BACKLOG)) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

int main() {
  int socket_fd = get_tcp_socket();

  printf("Listening at http://localhost:%d\n", PORT);

  struct sockaddr_storage client_address;
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  for (;;) {
    socklen_t addr_size = sizeof(client_address);
    memset(&client_address, 0, addr_size);

    int conn_fd = accept(socket_fd, (struct sockaddr *)&client_address, &addr_size);

    if (conn_fd < 0) {
      perror("accept");
      return 1;
    }

    FILE *stream = fdopen(conn_fd, "r+");

    if (stream == NULL) {
      perror("fdopen");
      return 1;
    }

    // Get general header GET / HTTP/1.1

    while ((linelen = getline(&line, &linecap, stream)) > 0) {
      printf("Capacity %lu\t\t%s", linecap, line);

      if (strcmp(line, "\r\n") == 0) {
	break;
      }
    }

    if (feof(stream)) {
      printf("EOF reached\n");
    }

    if (ferror(stream)) {
      perror("getline");
      return 1;
    }

    fprintf(stream, "HTTP/1.1 200 OK\r\n\r\n<div>Hello, world!</div>");
    fflush(stream);

    memset(line, 0, linecap);
    fclose(stream);
  }


  return 0;
}

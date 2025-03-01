#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define LISTEN_BACKLOG 128

int main() {

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (socket_fd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in hostaddr = {0};
  hostaddr.sin_family         = AF_INET;
  hostaddr.sin_addr.s_addr    = INADDR_ANY;
  hostaddr.sin_port           = htons(PORT);

  if ((bind(socket_fd, (struct sockaddr *) &hostaddr, sizeof(hostaddr))) < 0) {
    perror("bind");
    return 1;
  }

  if ((listen(socket_fd, LISTEN_BACKLOG)) < 0) {
    perror("listen");
    return 1;
  }

  printf("Listening at http://localhost:%d\n", PORT);

  socklen_t addr_size = sizeof(hostaddr);
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  for (;;) {
    int conn_fd = accept(socket_fd, (struct sockaddr *)&hostaddr, &addr_size);

    if (conn_fd < 0) {
      perror("accept");
      return 1;
    }

    FILE *stream = fdopen(conn_fd, "r+");

    if (stream == NULL) {
      perror("fdopen");
      return 1;
    }

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

    memset(line, 0, linecap);
  }


  return 0;
}

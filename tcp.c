#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>

int tcp_socket() {
  int socket_fd;

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

void tcp_bind(int socket, const struct sockaddr_in *address) {
  if ((bind(socket, (struct sockaddr *)&address, sizeof(struct sockaddr_in))) <
      0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
}

void tcp_listen(int socket, int backlog) {
  if ((listen(socket, backlog)) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

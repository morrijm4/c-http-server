#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#define PORT 8080
#define LISTEN_BACKLOG 128

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
  hostaddr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &hostaddr.sin_addr);

  if ((bind(socket_fd, (struct sockaddr *)&hostaddr, sizeof(struct sockaddr_in))) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  if ((listen(socket_fd, LISTEN_BACKLOG)) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  return socket_fd;
}

void print_address(struct sockaddr_storage *addr) {
  switch (addr->ss_family) {
  case AF_INET: {
    struct sockaddr_in *tmp = (struct sockaddr_in *)addr;
    char ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &tmp->sin_addr, ip, INET_ADDRSTRLEN);

    fprintf(stderr, "============= Client Address =============\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "(Address) length: %d\n", tmp->sin_len);
    fprintf(stderr, "(Address) family: IPv4 (%d)\n", tmp->sin_family);
    fprintf(stderr, "(Address)     IP: %s\n", ip);
    fprintf(stderr, "(Address)   port: %d\n", ntohs(tmp->sin_port));
    fprintf(stderr, "\n");

    break;
  }
  default:
    printf("Unknown address: %d\n", addr->ss_family);
  }
}

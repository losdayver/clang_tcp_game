#include "server.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t fork_server(struct make_server_params *params) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    struct make_server_params server_params = {
        .port = 8080, .buf_size = 1024, .max_clients = 5};
    make_server(&server_params);
  }
  return pid;
}

void make_server(struct make_server_params *params) {
  struct make_server_params params_copy = *params;

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(params_copy.port);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    perror("server_socket");
    exit(1);
  }

  // prevents "address already in use" error
  int opt = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // tell OS to "open" socket on the given address
  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("bind");
    exit(1);
  }

  // allows this socket to "listen" for incoming data. 5 stands for "5 clients
  // will wait in a queue before connecting"
  listen(server_socket, 5);
  printf("started server on port %d\n", params_copy.port);

  int client, clients[params_copy.max_clients];
  for (int i = 0; i < params_copy.max_clients; i++)
    clients[i] = -1;

  int max_fd;
  fd_set readfds;
  while (1) {
    FD_ZERO(&readfds);
    FD_SET(server_socket, &readfds);
    max_fd = server_socket;

    for (int i = 0; i < params_copy.max_clients; i++) {
      if (clients[i] != -1) {
        FD_SET(clients[i], &readfds);
        if (clients[i] > max_fd)
          max_fd = clients[i];
      }
    }

    // waiting for events. if event happens readfds will contain sockets that
    // pushed data
    if (select(max_fd + 1, &readfds, NULL, NULL, NULL) < 0) {
      perror("select");
      break;
    }

    char buf[params_copy.buf_size];
    // accepting connection from client
    if (FD_ISSET(server_socket, &readfds)) {
      client = accept(server_socket, NULL, NULL);
      if (client < 0) {
        perror("accept");
        continue;
      }

      // Add client to clients
      for (int i = 0; i < params_copy.max_clients; i++) {
        if (clients[i] == -1) {
          clients[i] = client;
          printf("Client %d connected\n", client);
          break;
        }
      }
    }

    for (int i = 0; i < params_copy.max_clients; i++) {
      int sock = clients[i];
      if (sock == -1)
        continue;

      if (FD_ISSET(sock, &readfds)) {
        int n = read(sock, buf, params_copy.buf_size - 1);

        if (n <= 0) {
          // client disconnected
          close(sock);
          clients[i] = -1;
        } else {
          // data is collected here
          buf[n] = '\0';
          printf("[%d]: %s\n", sock, buf);
          write(sock, buf, n); // echo
        }
      }
    }
  }

  close(server_socket);
}
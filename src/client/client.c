#include "client.h"
#include "libbase64.h"
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct Client *Client_new(struct Start_client_params *params) {
  char buffer[4096];

  // creating new TCP-socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  // configuring address
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // connecting
  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect");
    close(sock);
    exit(1);
  }

  int flags = fcntl(sock, F_GETFL, 0);
  fcntl(sock, F_SETFL, flags | O_NONBLOCK);

  struct Client *client = (struct Client *)malloc(sizeof(struct Client));
  client->socket = sock;
  client->state = CLIENT_STATE_CONNECTED;
  return client;
}

void Client_send_str(struct Client *instance, char *msg) {
  size_t srclen = strlen(msg);
  size_t outlen;
  char out[1024];
  base64_encode(msg, srclen, out, &outlen, 0);
  strcat(out, "\n");
  send(instance->socket, out, strlen(out), 0);
};

void Client_pop(struct Client *instance) {
  char buf[4096];
  int n;

  // читаем данные из сокета
  n = read(instance->socket, buf, sizeof(buf) - 1);

  if (n < 0) {
    // no data
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return;
      // forced disconnect
    } else {
      perror("read");
      close(instance->socket);
      instance->socket = -1;
      return;
    }
  }

  if (n == 0) {
    // 0 bytes = server closed connection
    printf("INFO C: server closed connection\n");
    close(instance->socket);
    instance->socket = -1;
    return;
  }

  buf[n] = '\0';

  instance->state = CLIENT_STATE_REGISTERED; // todo DELETEME

  printf("INFO C: received: %s\n", buf);
}

#include "client.h"
#include "packet.h"
#include "server.h"
#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  struct Start_server_params server_params = {
      .port = 8080, .buf_size = 1024, .max_clients = 5};
  pid_t pid = fork_server(&server_params);
  if (pid != 0) {
    char *nickname = "mynameisjeff";
    struct Start_client_params *start_client_params =
        (struct Start_client_params *)(malloc(
            sizeof(struct Start_client_params)));
    start_client_params->host = "localhost";
    start_client_params->port = 8080;
    start_client_params->buf_size = 4096;
    struct Client *client = Client_new(start_client_params);
    char *packet_str = "{\"method\":101,\"nickname\":\"mynameisjeff\"}";
    Client_send_str(client, packet_str);
    start_window(client);
  }
}

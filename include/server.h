#pragma once
#include "packet.h"
#include <sys/types.h>

struct Make_server_params {
  int port;
  int buf_size;
  int max_clients;
};
void make_server(struct Make_server_params *params);
pid_t fork_server(struct Make_server_params *params);

struct Client_with_parser {
  int client;
  struct Parser *parser;
};
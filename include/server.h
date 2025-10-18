#pragma once
#include <sys/types.h>

struct make_server_params {
  int port;
  int buf_size;
  int max_clients;
};

int make_server(struct make_server_params *params);
pid_t fork_server(struct make_server_params *params);
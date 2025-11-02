#pragma once
#include <stddef.h>

#define PARSER_BUFFER_SIZE 4096

struct Packet {
  int method;
};
void Packet_free(struct Packet *instance);

// client -> server
#define PACKET_METHOD_CONNECT 101
struct Packet_connect {
  int method;
  char *nickname;
};

// server -> client

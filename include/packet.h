#pragma once
#include <stddef.h>

#define PARSER_BUFFER_SIZE 4096

struct Packet {
  int method;
};
void Packet_free(struct Packet *instance);

#define PACKET_METHOD_CONNECT 101
struct Packet_connect {
  int method;
  char *nickname;
};

struct Parser {
  struct Packet_queue *queue;
  char data[PARSER_BUFFER_SIZE];
  size_t length;
};
struct Parser *Parser_new(void);
void Parser_free(struct Parser *instance);
void Parser_acquire_buffer(struct Parser *instance, char *buffer);
struct Packet *Parser_pop_packet(struct Parser *instance);

struct Packet_queue {
  struct Packet_queue *next;
  struct Packet *packet;
};
void Packet_queue_free(struct Packet_queue *instance);

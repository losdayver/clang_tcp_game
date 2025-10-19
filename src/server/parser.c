#include "cJson.h"
#include "packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Parser *Parser_new() {
  struct Parser *parser = (struct Parser *)(malloc(sizeof(struct Parser)));
  return parser;
};
void Parser_free(struct Parser *parser) {
  Packet_queue_free(parser->queue);
  free(parser);
};

void Parser_acquire_buffer(struct Parser *instance, char *buffer) {
  size_t buf_len = strlen(buffer);
  // copy data into buffer
  if (instance->length + buf_len < PARSER_BUFFER_SIZE) {
    memcpy(instance->data + instance->length, buffer, buf_len);
    instance->length += buf_len;
    instance->data[instance->length] = '\0';
  } else {
    instance->length = 0;
    return;
  }

  // find start and end of json stream
  char *start = strchr(instance->data, '{');
  char *end = strchr(instance->data, '}');
  if (!start || !end || end < start) {
    return;
  }
  size_t json_len = end - start + 1;
  char *json_str = (char *)malloc(json_len + 1);
  memcpy(json_str, start, json_len);
  json_str[json_len] = '\0';

  // parse json_str
  cJSON *root = cJSON_Parse(json_str);
  if (root) {
    cJSON *method = cJSON_GetObjectItem(root, "method");

    struct Packet_queue *node =
        (struct Packet_queue *)malloc(sizeof(struct Packet_queue));
    node->packet.method =
        method && cJSON_IsString(method) ? strdup(method->valuestring) : NULL;
    node->next = instance->queue;
    instance->queue = node;

    cJSON_Delete(root);
    instance->length = 0;
    instance->data[0] = '\0';
  }
};

struct Packet *Parser_pop_packet(struct Parser *instance) {
  if (instance->queue == NULL)
    return NULL;
  struct Packet *packet = (struct Packet *)malloc(sizeof(struct Packet));
  struct Packet_queue *node = instance->queue;
  if (node->packet.method)
    packet->method = strdup(node->packet.method);
  else
    packet->method = NULL;
  instance->queue = node->next;
  free(node);
  return packet;
}

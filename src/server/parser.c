#include "cJson.h"
#include "packet.h"
#include "server.h"
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

void Parser_parse_packet(struct Parser *instance, char *json_str) {
  cJSON *root = cJSON_Parse(json_str);
  if (root) {
    cJSON *method_fld = cJSON_GetObjectItem(root, "method");

    int method =
        (method_fld && cJSON_IsNumber(method_fld) ? method_fld->valueint : 0);

    // parsing different packet types
    struct Packet *packet;
    if (method == PACKET_METHOD_CONNECT) {
      packet = (struct Packet *)malloc(sizeof(struct Packet_connect));
      packet->method = method;
      ((struct Packet_connect *)packet)->nickname =
          (char *)malloc(sizeof(char));
      strcpy(((struct Packet_connect *)packet)->nickname,
             cJSON_GetObjectItem(root, "nickname")->valuestring);
    } else {
      cJSON_Delete(root);
      return;
    }
    struct Packet_queue *node =
        (struct Packet_queue *)malloc(sizeof(struct Packet_queue));

    node->packet = packet;
    node->next = instance->queue;
    instance->queue = node;

    cJSON_Delete(root);
    instance->length = 0;
    instance->data[0] = '\0';
  }
}

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
  Parser_parse_packet(instance, json_str);
};

struct Packet *Parser_pop_packet(struct Parser *instance) {
  if (instance->queue == NULL)
    return NULL;
  struct Packet_queue *node = instance->queue;
  struct Packet *packet = node->packet;
  instance->queue = instance->queue->next;
  free(node);
  return packet;
}

void Packet_free(struct Packet *instance) {
  if (instance->method == PACKET_METHOD_CONNECT) {
    free(((struct Packet_connect *)instance)->nickname);
  }
  free(instance);
}
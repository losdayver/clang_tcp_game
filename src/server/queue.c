#include "packet.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>

void Packet_queue_free(struct Packet_queue *instance) {
  struct Packet_queue *prev_instance = NULL;
  struct Packet_queue *curr_instance = instance;
  while (curr_instance) {
    curr_instance = curr_instance->next;
    prev_instance = curr_instance;
    if (prev_instance) {
      free(prev_instance->packet);
      free(prev_instance);
    }
  }
};

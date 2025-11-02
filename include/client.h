#pragma once

#define CLIENT_STATE_CONNECTED 100
#define CLIENT_STATE_REGISTERED 200

struct Client {
  int socket;
  int state;
};

struct Start_client_params {
  const char *host;
  int port;
  int buf_size;
};

struct Client *Client_new(struct Start_client_params *params);
void Client_send_str(struct Client *instance, char *msg);
void Client_pop(struct Client *instance);
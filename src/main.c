#include "packet.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  struct Make_server_params server_params = {
      .port = 8080, .buf_size = 1024, .max_clients = 5};
  pid_t pid = fork_server(&server_params);
  if (pid != 0) {
    waitpid(pid, NULL, 0);
  }
}

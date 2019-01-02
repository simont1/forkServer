#include "pipe_networking.h"



int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char input[BUFFER_SIZE];
  char response[BUFFER_SIZE];

  while (1){

    printf("Type something: ");
    scanf(" %[^\n]s", input);
    write(to_server, input, sizeof(input));
    read(from_server, response, sizeof(response));
    printf("Response: %s\n", response);

  }

  return 0;
}

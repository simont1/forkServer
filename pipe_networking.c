#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("[server] Creating wkp...\n");
  mkfifo(WKP, 0644);

  printf("[server] Waiting for Client...\n");
  int from_client = open(WKP, O_RDONLY);

  char pid[HANDSHAKE_BUFFER_SIZE];
  read(from_client, pid, sizeof(pid));
  printf("[server] Message Received: %s\n", pid);

  printf("[server] Removing wkp...\n");
  remove(WKP);

  int f = fork();

  if (f) return 0;

  *to_client = open(pid, O_WRONLY);
  printf("[subserver %d] Writing to Client...\n", getpid());
  write(*to_client, ACK, sizeof(ACK));

  char msg[HANDSHAKE_BUFFER_SIZE];
  printf("[subserver %d] Waiting for Client...\n", getpid());
  read(from_client, msg, sizeof(msg));
  printf("[subserver %d] Message Received: %s\n", getpid(), msg);

  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("[client %d] Creating private FIFO...\n", getpid());
  char pid[HANDSHAKE_BUFFER_SIZE];
  sprintf(pid, "%d", getpid());
  mkfifo(pid, 0644);

  printf("[client %d] Writing to Server...\n", getpid());
  *to_server = open(WKP, O_WRONLY);
  write(*to_server, pid, sizeof(pid));

  int from_server = open(pid, O_RDONLY);
  printf("[client %d] Waiting for Server...\n", getpid());
  char msg[HANDSHAKE_BUFFER_SIZE];
  read(from_server, msg, sizeof(msg));
  printf("[client %d] Message Received: %s\n", getpid(), msg);
  printf("[client %d] Removing private FIFO...\n", getpid());
  remove(pid);

  printf("[client %d] Writing to Server...\n", getpid());
  write(*to_server, ACK, sizeof(ACK));

  return from_server;
}

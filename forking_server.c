#include "pipe_networking.h"

static void sighandler(int signo){
  if (signo == SIGINT){
    remove(WKP);
    exit(1);
  }
}

int main() {

  int to_client;
  int from_client;

  char str[BUFFER_SIZE];

  signal(SIGINT, sighandler);

  while (1){

    from_client = server_handshake( &to_client );

    if (from_client){

      while ( read(from_client, str, sizeof(str)) ){
	      printf("[subserver %d] Message Received: %s", getpid(), str);
	      char* dummy = str;
	      while (*dummy){
	        *dummy += 5;
	        dummy ++;
	      }
      	printf("[subserver %d] Message Sent: %s\n", getpid(), str);
      	write(to_client, str, sizeof(str));
      }

      break;
    }
  }

  return 0;
}

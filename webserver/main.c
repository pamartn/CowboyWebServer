#include <sys/types.h>
#include <sys/socket.h>
#include "signals.h"
#include "client.h"
#include "socket.h"
#include "debug.h"

#define SERVER_PORT 8080

int main(void){
	DEBUG_PRINT("Debugging enabled\n");
	initialiser_signaux();
	int socket_serveur = creer_serveur(SERVER_PORT);
	if(socket_serveur == -1)
		return 1;
	printf("Server listenning on %d\n", SERVER_PORT);
	int socket_client;
	while(1){
		socket_client = accept(socket_serveur, NULL, NULL);
		if (socket_client == -1){
			perror("accept ");
			return 1;
		}
		if(fork() == 0){
			dialoguer(socket_client);
			close(socket_client);
			exit(1);
		} else {
			close(socket_client);
		}
	}
	close(socket_serveur);
	return 0;
}

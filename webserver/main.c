#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include "signals.h"
#include "client.h"
#include "socket.h"
#include "debug.h"

#define SERVER_PORT 8080

char *BASE_DIR;

int main(int argc, char **argv){
	BASE_DIR = malloc(1024*sizeof(char));
	if(argc >= 2){
		struct stat sb;
		stat(argv[1], &sb);
		if(S_ISDIR(sb.st_mode)) {
			BASE_DIR = argv[1];
		} else {
			printf("Directory %s does not exists !\n", argv[1]);
			return 1;
		}
	}

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
	free(BASE_DIR);
	return 0;
}

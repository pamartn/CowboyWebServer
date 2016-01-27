// Autor : Etienne (Nicolas)
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include "socket.h"
#include <stdio.h>
#include <arpa/inet.h>

int creer_serveur(int port){
	int socket_serveur ;
	socket_serveur = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_serveur == -1){
		perror("socket_serveur");
		return -1;
	}
	/* Utilisation de la socket serveur */
	struct sockaddr_in saddr ;
	saddr.sin_family = AF_INET ; /* Socket ipv4 */
	saddr.sin_port = htons (port); /* Port d ’ écoute */
	saddr.sin_addr.s_addr = INADDR_ANY; /* écoute sur toutes les interfaces */
	if (bind(socket_serveur, (struct sockaddr *)&saddr, sizeof(saddr)) == -1){
		perror("bind socket_serveur");
		return -1;	
	}
	if (listen(socket_serveur, 10) == -1){
		perror("listen socket_serveur");
		return -1;
	}	
	return socket_serveur;
}

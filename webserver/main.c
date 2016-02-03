#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>

#define BUF_SIZE 1024

void initialiser_signaux(void) {
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){	
		perror("signal");
	}
}
int main(void){
	initialiser_signaux();
	int socket_serveur = creer_serveur(8080);
	if(socket_serveur == -1)
		return 1;
	int socket_client;
	socket_client = accept(socket_serveur, NULL, NULL);
	if (socket_client == -1){
		perror("accept ");
		return 1;
	}
	/* On peut maintenant dialoguer avec le client */
	const char *message_bienvenue = "Bonjour, bienvenue sur le serveur Cowboy. Ce serveur est créé pour remplacer Apache.\nLes cowboys ont tués les Indiens d'Amérique et se sont appropriés leurs terres.\n Le cow-boy ou cowboy, de l'anglais cow, « vache » et boy, « garçon »), qui signifie vacher ou bouvier en français, est un garçon de ferme s'occupant du bétail bovin dans les pays anglo-saxons de grands espaces comme le Far West américain et l'Outback australien.\n";
	sleep(1);
	int testErrorWrite = write(socket_client, message_bienvenue, strlen(message_bienvenue));
	if (testErrorWrite == -1) {
		perror("error write :");
		return 1;
	}
	char buf[BUF_SIZE];
	while(1){
		bzero(buf, BUF_SIZE);
		int n = read(socket_client, buf, BUF_SIZE-1);
		if(n == -1){
			perror("read socket");
			return 1;	
		}
		buf[BUF_SIZE-1] = '\0';
		printf("%s", buf);
		write(socket_client, buf, n);
	}

	close(socket_client);
	close(socket_serveur);
	return 0;
}




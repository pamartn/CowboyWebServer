#include <stdio.h>
#include <string.h>
#include "socket.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <signal.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void traitement_signal(){
	int status;
	int pid;
	while ((pid = waitpid(-1,&status,WNOHANG)) > 0) {
		printf ("Processus numéro %d a retourné %d\n ", pid, status);
	}
}

void initialiser_signaux(void){
	struct sigaction sa;
	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1){
		perror ("sigaction(SIGCHLD)");
	}
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){	
		perror("signal");
	}
}

int main(void){
	const char *message_bienvenue = "Bonjour, bienvenue sur le serveur Cowboy. Ce serveur est créé pour remplacer Apache.\nLes cowboys ont tués les Indiens d'Amérique et se sont appropriés leurs terres.\n Le cow-boy ou cowboy, de l'anglais cow, « vache » et boy, « garçon »), qui signifie vacher ou bouvier en français, est un garçon de ferme s'occupant du bétail bovin dans les pays anglo-saxons de grands espaces comme le Far West américain et l'Outback australien.\n";
	
	initialiser_signaux();
	int socket_serveur = creer_serveur(8080);
	if(socket_serveur == -1)
		return 1;
	
	int socket_client;
	while(1){
		socket_client = accept(socket_serveur, NULL, NULL);
		if (socket_client == -1){
			perror("accept ");
			return 1;
		}
		if(fork() == 0){
			/* On peut maintenant dialoguer avec le client */
			int testErrorWrite = write(socket_client, message_bienvenue, strlen(message_bienvenue));
			if (testErrorWrite == -1) {
				perror("error write :");
				exit(1);
			}
			char buf[BUF_SIZE];
			int n;
			bzero(buf, BUF_SIZE);
			while((n = read(socket_client, buf, BUF_SIZE-1)) > 0){
				buf[BUF_SIZE-1] = '\0';
				printf("%s", buf);
				if(write(socket_client, buf, n) <= 0)
					break;
				bzero(buf, BUF_SIZE);
			}
			close(socket_client);
			exit(1);
		} else {
			close(socket_client);
		}
	}
	close(socket_serveur);
	return 0;
}

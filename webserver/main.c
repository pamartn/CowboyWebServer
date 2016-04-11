#include "main.h"

char *BASE_DIR;


int set_base_dir(char *dirname){
	struct stat sb;
	stat(dirname, &sb);
	if(S_ISDIR(sb.st_mode)) {
		BASE_DIR = dirname;
	} else {
		printf("Directory %s does not exists !\n", dirname);
		return 0;
	}
	return 1;
}

int main(int argc, char **argv){
	BASE_DIR = "public";
	if(argc >= 2 && !set_base_dir(argv[1]))
		return 1;
	printf("Server basedir : %s\n", BASE_DIR);	

	DEBUG_PRINT("Debugging enabled\n");
	init_stats();
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

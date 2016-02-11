#include "client.h"

/* 
	Vérifie la première ligne du header http
	Retourne 0 si echec, 1 si réussi
*/
int verifier_ligne(char *ligne, char *url){
	int mots = 0;
	char *token;
	token = strtok(ligne, " ");

	while(token != NULL){
		//On va maintenant analyser les mots
		if(mots == 0 && !strcmp(token, "GET") == 0)
			return 0;
		if(mots == 1 && strlen(token) <= URL_SIZE)
			strcpy(url, token);
		if(mots == 2 && !(strncmp(token, "HTTP/1.0", 8) == 0 || strncmp(token, "HTTP/1.1", 8) == 0))
			return 0;
		// On récupère le prochain mot
		token = strtok(NULL, " ");
		mots++;
	}
	return mots == 3;
}

void creer_reponse(char *res, char *url, int valid){
	int content_length;		
	char *msg;
	if(valid && strcmp(url,"/") == 0){
		msg = "<html><head><meta charset=\"UTF-8\"></head><h1>It works!</h1>Bonjour, bienvenue sur le serveur Cowboy. Ce serveur est créé pour remplacer Apache.\nLes cowboys ont tués les Indiens d'Amérique et se sont appropriés leurs terres.\n Le cow-boy ou cowboy, de l'anglais cow, « vache » et boy, « garçon »), qui signifie vacher ou bouvier en français, est un garçon de ferme s'occupant du bétail bovin dans les pays anglo-saxons de grands espaces comme le Far West américain et l'Outback australien.</html>";
		content_length = strlen(msg);
		sprintf(res, "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-length: %d\r\nContent-Type: text/html\r\n\r\n%s", content_length, msg);	
	} else if(valid) {
		msg = "404 Not found : ";
		char *msg2 = " was not found on this server :(";
		content_length = strlen(msg)+strlen(url)+strlen(msg2);
		sprintf(res, "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-length: %d\r\n\r\n%s%s%s", content_length, msg, url, msg2);
	} else {
		msg = "400 Bad request";
		content_length = strlen(msg);
		sprintf(res, "HTTP/1.1 400 Bad Request\r\nConnection: close\r\nContent-length: %d\r\n\r\n%s", content_length, msg);
	}
}


void dialoguer(int socket_client){
	/* On peut maintenant dialoguer avec le client */
	FILE *f = fdopen(socket_client, "w+");	
	char buf[BUF_SIZE];

	char res[2048];
	char url[URL_SIZE];
	if(fgets(buf, BUF_SIZE, f) != NULL){
		DEBUG_PRINT("%s\n", buf);
		if(verifier_ligne(buf, url))
			creer_reponse(res, url, 1);
		else
			creer_reponse(res, url, 0);
		while(strcmp(buf, "\n") != 0 && strcmp(buf, "\r\n") != 0)
			fgets(buf, BUF_SIZE, f); 

		DEBUG_PRINT("response :\n%s\n", res);
		fprintf(f, res);		
	}
	fclose(f);
}
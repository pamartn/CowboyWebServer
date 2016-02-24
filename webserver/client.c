#include "client.h"

void send_status(FILE * client, int code, const char *reason_phrase){
	char res[strlen(reason_phrase)+ 20];
	sprintf(res, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
	fprintf(client, res);
}

void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body){
	char res[256+strlen(message_body)];
	sprintf(res, "Connection: close\r\nContent-length: %zu\r\n\r\n%s", strlen(message_body), message_body);
	send_status(client, code, reason_phrase);
	fprintf(client, res);
}

char *fgets_or_exit(char *buffer, int size, FILE *stream){
	if(fgets(buffer, size, stream) == NULL)
		exit(1);
	return buffer;
}


void skip_headers(FILE * client){
	char line[BUF_SIZE];
	
	while(fgets_or_exit(line, BUF_SIZE, client) != NULL && strcmp(line, "\n") != 0 && strcmp(line, "\r\n") != 0);
}

void dialoguer(int socket_client){
	/* On peut maintenant dialoguer avec le client */
	char *motd = "<html><head><meta charset=\"UTF-8\"></head><h1>It works!</h1>Bonjour, bienvenue sur le serveur Cowboy. Ce serveur est créé pour remplacer Apache.\nLes cowboys ont tués les Indiens d'Amérique et se sont appropriés leurs terres.\n Le cow-boy ou cowboy, de l'anglais cow, « vache » et boy, « garçon »), qui signifie vacher ou bouvier en français, est un garçon de ferme s'occupant du bétail bovin dans les pays anglo-saxons de grands espaces comme le Far West américain et l'Outback australien.</html>";

	FILE *client = fdopen(socket_client, "w+");	
	
	char first_line[BUF_SIZE];
	fgets_or_exit(first_line, BUF_SIZE, client);
	DEBUG_PRINT("%s", first_line);
	skip_headers(client);
	
	http_request request;
	int bad_request = !parse_http_request(first_line, &request);
	
	if(bad_request)
		send_response(client, 400, "Bad Request", "Bad request");
	else if (request.method == HTTP_UNSUPPORTED)
		send_response(client, 405, "Method Not Allowed", "Method Not Allowed");
	else if (strcmp(request.url , "/") == 0)
		send_response(client, 200, "OK", motd);
	else
		send_response(client, 404, "Not Found", "Not Found");

	fclose(client);	
}

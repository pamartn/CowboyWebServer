#include "client.h"

void send_status(FILE * client, int code, const char *reason_phrase){
	fprintf(client, "HTTP/1.1 %d %s\r\n", code, reason_phrase);
}

void send_response(FILE *client, int code, const char *reason_phrase, char *message_body){
	send_status(client, code, reason_phrase);
	fprintf(client, "Connection: close\r\nContent-length: %zu\r\n\r\n%s", strlen(message_body), message_body);
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

void send_stats(FILE *client){
	char msg[1024];
	web_stats *stats = get_stats();
	sprintf(msg, "<h1>Page des stats</h1><div>connections : %d<br/>requetes : %d<br/>ok_200 : %d<br/>ko_400 : %d<br/>ko_403 : %d<br/>ko_404 : %d<br/></div>", stats->served_connections, stats->served_requests, stats->ok_200, stats->ko_400, stats->ko_403, stats->ko_404);
	send_status(client, 200, "OK");
	fprintf(client, "Connection: close\r\nContent-type: text/html\r\nContent-length: %zu\r\n\r\n%s\r\n", strlen(msg), msg);

}

const char *get_content_type(char *filename){
	char *ext = strrchr(filename+1, '.')+1;
	printf("%s\n", filename);
	if(strcmp(ext, "html") == 0)
		return "text/html";
	else if(strcmp(ext, "js") == 0)
		return "application/javascript";
	else if(strcmp(ext, "css") == 0)
		return "text/css";
	else if(strcmp(ext, "jpg") == 0)
		return "image/jpeg";
	else if(strcmp(ext, "png") == 0)
		return "image/png";
	return "text";
}

void dialoguer(int socket_client){
	/* On peut maintenant dialoguer avec le client */
	FILE *client = fdopen(socket_client, "w+");	
	
	web_stats *stats = get_stats();
	if(sem_wait(&stats->sem) == -1){
		perror("sem_wait");
		exit(1);
	}
	stats->served_connections++;

	char first_line[BUF_SIZE];
	fgets_or_exit(first_line, BUF_SIZE, client);
	DEBUG_PRINT("%s", first_line);
	skip_headers(client);
	
	http_request request;
	int bad_request = !parse_http_request(first_line, &request);
	stats->served_requests++;	
	if(bad_request) {
		stats->ko_400++;
		send_response(client, 400, "Bad Request", "Bad request");
	}
	else if (request.method == HTTP_UNSUPPORTED) {
		send_response(client, 405, "Method Not Allowed", "Method Not Allowed");
	}
	else if (!verify_url(request.url)) {
		stats->ko_403++;
		send_response(client, 403, "Forbidden", "Forbidden");
	}
	else if(strcmp(request.url, "/stats") == 0) {
			stats->ok_200++;
			send_stats(client);
	} else {
		int fd = check_and_open(request.url, BASE_DIR);
		if(fd < 0) {
			stats->ko_404++;
			send_response(client, 404, "Not Found", "Not Found");
		}
		else {
			stats->ok_200++;
			send_status(client, 200, "OK");
			fprintf(client, "Connection: close\r\nContent-type: %s\r\nContent-length: %d\r\n\r\n", get_content_type(request.url), get_file_size(fd));
			fflush(client);
			copy(fd, socket_client);			
		}
	}
	fclose(client);
	close(socket_client);
	if(sem_post(&stats->sem) == -1){
		perror("sem_post");
		exit(1);
	}

}

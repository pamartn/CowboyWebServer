#include "request.h"

char *rewrite_url(char *url){
	if(strcmp(url, "/") == 0)
		return "/index.html";
	printf("url : %s\n", url);
	char *cutpoint = strchr(url, '?');
	if(cutpoint == NULL)
		return url;
	url[(cutpoint - url)] = '\0';
	return url;
}

int verify_url(char *url){
	return strstr(url, "..") == NULL;
}


/* 
	Vérifie la première ligne du header http
	Retourne 0 si echec, 1 si réussi
*/
int parse_http_request(const char *request_line, http_request *request) {
	int mots = 0;
	char *token;
	
	int line_length = strlen(request_line);
	char line[line_length+1];
	strcpy(line, request_line);
	
	token = strtok(line, " ");

	while(token != NULL){
		//On va maintenant analyser les mots
		if(mots == 0 && strcmp(token, "GET") != 0){
			request->method = HTTP_UNSUPPORTED;
		} else {
			request->method = HTTP_GET;
		}
		if(mots == 1 && strlen(token) <= URL_SIZE){
			request->url = strdup(token);
			request->url = rewrite_url(request->url);
			DEBUG_PRINT("URL REWRITED : %s\n", request->url);
		}
		if(mots == 2){
			if(strncmp(token, "HTTP/1.0", 8) == 0){
				request->major_version = 1;
				request->minor_version = 0;
			} else if(strncmp(token, "HTTP/1.1", 8) == 0){
				request->major_version = 1;
				request->minor_version = 1;
			} else {
				return 0;
			}
		}
		// On récupère le prochain mot
		token = strtok(NULL, " ");
		mots++;
	}
	return mots == 3;
}

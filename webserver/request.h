#ifndef __REQUEST_H__
# define __REQUEST_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "debug.h"

#define URL_SIZE 255

enum http_method {
	HTTP_GET ,
	HTTP_UNSUPPORTED ,
};
typedef struct
{
	enum http_method method ;
	int major_version ;
	int minor_version ;
	char * url;
} http_request;

char *rewrite_url(char *url);

int verify_url(char *url);

int parse_http_request(const char *request_line, http_request *request);

#endif

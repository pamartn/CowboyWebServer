#ifndef __CLIENT_H__
# define __CLIENT_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "debug.h"
#include "request.h"


#define BUF_SIZE 1024

void dialoguer(int socket_client);

#endif

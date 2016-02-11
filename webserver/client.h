#ifndef __CLIENT_H__
# define __CLIENT_H__


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include "debug.h"

#define BUF_SIZE 1024
#define URL_SIZE 255

/* 
	Vérifie la première ligne du header http
	Retourne 0 si echec, 1 si réussi
*/
int verifier_ligne(char *ligne, char *url);

void creer_reponse(char *res, char *url, int valid);

void dialoguer(int socket_client);

#endif

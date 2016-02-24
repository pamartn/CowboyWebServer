#ifndef __FILE_H__
#define __FILE_H__

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

char *rewrite_url(char *url);

int check_and_open(const char *url, const char *document_root);

int get_file_size(int fd);

int copy(int in, int out);

#endif

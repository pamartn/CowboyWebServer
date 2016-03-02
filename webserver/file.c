#include "file.h" 


int check_and_open(const char *url, const char *document_root){
	char path[strlen(document_root)+strlen(url)+1];
	sprintf(path, "%s%s", document_root, url);

	struct stat sb;
	stat(path, &sb);
	if(S_ISREG(sb.st_mode))
		return open(path, O_RDONLY);
	return -1;
}

int get_file_size(int fd){
	struct stat sb;
	fstat(fd, &sb);
	return sb.st_size;
}

int copy(int in, int out){
	int cpt;
	int buf_size = 2048;
	char buf[buf_size];
	memset(buf, 0, buf_size);
	while((cpt = read(in, buf, buf_size)) >= 0){
		if(write(out, buf, cpt) < 0)
			return 0;
		if(cpt == 0)
			break;
	}
	if(cpt < 0)
		return 0;
	return 1;
}

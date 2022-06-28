// cat.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void cat(int fd){
	int n;
	char buf[100];

	while((n = read(fd, buf, sizeof(buf))) > 0){
		if(write(1, buf, n) != n){ // "1" means "stdout"
			printf("cat: write error\n");
			return;
		}
	}
	if(n < 0){
		printf("cat: read error\n");
		return;
	}
}

int main(int argc, char *argv[]){
	int fd;

	if(argc <= 1){
		cat(0);
		exit(-1);
	}

	for(int i = 1; i < argc; i++){
		if((fd = open(argv[i], 0)) < 0){
			printf("cat: cannot open %s\n", argv[i]);
			exit(1);
		}
		cat(fd);
		close(fd);
	}
	exit(0);
}

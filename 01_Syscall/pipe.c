// pipe.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
	int p[2];
	char *argv[2];
	int wstatus;

	argv[0] = "wc";
	argv[1] = 0;

	pipe(p); // p[0]: readfd, p[1]: writefd
	if(fork() == 0){ // child process
		close(0); // empty fd table 0
		dup(p[0]); // copy piperead
		close(p[0]);
		close(p[1]); // close unused write end
		execve("/usr/bin/wc", argv, NULL);
	}
	else{
		close(p[0]); // close unused read end
		write(p[1], "hello world\n", 12);
		close(p[1]);
		wait(&wstatus);
	}
}

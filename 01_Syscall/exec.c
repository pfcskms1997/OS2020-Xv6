// exec.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	pid_t pid = fork();
	int wstatus;

	if(pid > 0){
		printf("parent: child=%d\n", pid);
		wait(&wstatus);
		printf("child %d is done\n", pid);
	}
	else if(pid == 0){
		printf("child is existing\n");
		execve("/bin/ls", argv, NULL); // need absolute path
	}
	else{
		printf("fork error\n");
	}
	return 0;
}

// fork.c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	pid_t pid = fork();
	int wstatus;

	if(pid > 0){
		printf("parent: child=%d\n", pid);
		wait(&wstatus); // wait until child process end
		printf("child %d is done\n", pid);
	}
	else if(pid == 0){
		printf("child is existing\n");
	}
	else{
		printf("fork error\n");
	}
	return 0;
}

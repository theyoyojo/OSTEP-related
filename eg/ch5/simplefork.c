#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int forkret ;
	int waitret ;

	printf("This PID: %d\n", (int)getpid()) ;
	forkret = fork() ;
	if (forkret < 0) {
		printf("FORK FAIL :( time to kms\n") ;
		exit(1) ;
	}
	else
	if (forkret == 0) {
		printf("This childish PID: %d\n", (int)getpid()) ;
	}
	else
	{
		waitret = wait(NULL) ;
		printf("This parental PID: %d (childpid: %d | waitret: %d)\n", (int)getpid(), forkret, waitret) ;
	}

	return 0 ;
}

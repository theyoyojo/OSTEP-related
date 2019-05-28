#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main() {
	int forkret ;
	int waitret ;
	char * args[3] ;

	memset(args,0,sizeof(args)) ;

	printf("This PID: %d\n", (int)getpid()) ;
	forkret = fork() ;
	if (forkret < 0) {
		printf("FORK FAIL :( time to kms\n") ;
		exit(1) ;
	}
	else
	if (forkret == 0) {
		close(STDOUT_FILENO) ;
		open("somefile",O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU) ;

		printf("This childish PID: %d\n", (int)getpid()) ;
		args[0] = strdup("bash") ;
		args[1] = strdup("hello") ;
		execvp(args[0],args) ;
		printf("exec failed :(\n") ;
	}
	else
	{
		waitret = wait(NULL) ;
		//waitret = 0 ;
		printf("This parental PID: %d (childpid: %d | waitret: %d)\n", (int)getpid(), forkret, waitret) ;
	}

	return 0 ;
}

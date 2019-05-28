#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int main() {
	int pipefd[2] ;
	
	int forkret,forkret2 ;

	char msg[] = "test message\n" ;
	
	if (pipe(pipefd) < 0) {
		printf("pipefail!\n") ;
		exit(1) ;
	}

	forkret = fork() ;
	if (forkret < 0) {
		printf("fork failed :(\n") ;
		exit(1) ;
	}
	else if (forkret == 0) {
		printf("i am fork1\n") ;


		close(pipefd[0]);
		write(pipefd[1], msg, strlen(msg)) ;
		close(pipefd[1]) ;

		printf("fork1done\n") ;
		return 0 ;
	}
	else {
		printf("created fork1\n") ;
	}
	
	forkret2 = fork() ;
	if (forkret2 < 0) {
		printf("fork failed :(\n") ;
		exit(1) ;
	}
	else if (forkret2 == 0) {
		char buff[32] ;
		memset(buff,0,32) ;

		printf("i am fork2\n") ;
		close(pipefd[1]);
		read(pipefd[0], buff, strlen(msg)) ;
		close(pipefd[0]) ;

		printf("fork2 reads: %s\n",buff) ;

		printf("fork2done\n") ;
		return 0 ;
	}
	else {
		printf("created fork2\n") ;
	}


	waitpid(forkret2,NULL,0) ;
	printf("parent says fork2 is done\n");
	waitpid(forkret,NULL,0) ;
	printf("parent says fork1 is done\n");

	printf("all done\n") ;
}

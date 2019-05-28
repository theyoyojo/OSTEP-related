#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
	int x = 20 ;

	int forkret = fork() ;

	//close (STDOUT_FILENO);
	//open("somefile",O_CREAT |O_WRONLY|O_TRUNC,755);

	if (forkret < 0) {
		printf("fork failed :(\n") ;
		exit(1) ;
	}
	else if (forkret == 0) {
		int waitret = wait(NULL) ;
		printf("child waitret: %d\n",waitret) ;
		printf("child x: %d\n",x) ;
		x = 5;
		printf("child x=5: %d\n",x) ;
	}
	else {
		int waitret = wait(NULL) ;
		printf("parent waitret: %d\n",waitret) ;
		printf("parent x: %d\n",x) ;
		x = 1;
		printf("parent x=1: %d\n",x) ;
	}


}

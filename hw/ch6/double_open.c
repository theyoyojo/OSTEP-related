#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

/*
 * File:	double_open.c
 * By: 	 	Joel savitz <joelsavitz@gmail.com> && https://joelsavitz.com
 * Created on: 	1 May 2019
 * Purpose:	Attempt to open the same file twice and observe the consequences
 */

void usage(void) ;

int main(int argc, char * argv[]) {

	int fd1, fd2 ;

	if (argc <= 1) {
		usage() ;
		exit(1) ;
	}

	(fd1 = open(argv[1],O_RDWR | O_CREAT, S_IRWXU)) < 1
		? perror("Unable to open file once")  : (void)0;
	(fd2 = open(argv[1],O_RDWR | O_CREAT, S_IRWXU)) < 1
		? perror("Unable to open file twice") : (void)0;

	printf("File descriptor values: 1: %d, 2: %d\n", fd1, fd2) ;



}

void usage(void) {
	printf("usage: ./double_open <filename>\n") ;
}

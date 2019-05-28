#include "mutex_file.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * File:	mutex_file.c (implements mutex_file.h)
 * By: 	 	Joel savitz <joelsavitz@gmail.com> && https://joelsavitz.com
 * Created on: 	1 May 2019
 * Purpose:	Provide an easy interface get and drop mutexes on a filesystem
 */

#define DEBUG 0

char * mutex_file_name = NULL ;
size_t mutex_file_size = 0 ;
int mutex_file_fd = -1 ;

int get_mutex_file(char * filename, size_t filename_size) {

	if(!filename) return -2 ;

	
	/* Case: file exists. Block until it doesn't*/
	while(!access(filename,F_OK)) ;

	/* while(!access(filename,F_OK)) { */
	/* 	printf("File \"%s\" currently exists\n", filename) ; */
	/* 	usleep(1000000) ; */
	/* } */

	mutex_file_size = filename_size ;
	mutex_file_name = (char *)malloc(filename_size) ;
	strncpy(mutex_file_name,filename,mutex_file_size) ;

	mutex_file_fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, S_IRWXU) ;

	return mutex_file_fd ;
	
}

int drop_mutex_file() {
	int close_retval ;

	/* Don't mess with the filenames */
	if (!mutex_file_name) return -2 ;

	/* If you don't have the mutex, the operation already successful*/
	if (mutex_file_fd < 0) return 0 ;

	close_retval = close(mutex_file_fd) ;

	remove(mutex_file_name) ;

	free(mutex_file_name) ;
	mutex_file_name = NULL ;

	return close_retval ;
}

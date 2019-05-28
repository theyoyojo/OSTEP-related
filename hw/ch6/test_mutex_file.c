#include "mutex_file.h"
#include <unistd.h>

#define ONE_SECOND_OF_MICROSECONDS 1000000

char filename[] = "test_mutex_file_temp_mutex_file" ;
size_t filename_size = sizeof(filename) ;

int main(void) {
	int mutex_fd ;

	printf("pre mutex getting\n") ;

	mutex_fd = get_mutex_file(filename, filename_size) ;
	
	if (mutex_fd < 0) {
		printf("failed to get mutex\n") ;
	}
	else {
		printf("in critical section\n") ;

		for (int i = 0; i < 5; ++i) {
			usleep(ONE_SECOND_OF_MICROSECONDS) ;
			printf("Sleeping %d...\n",i+1) ;
		}

		drop_mutex_file() ;
	}


	
}

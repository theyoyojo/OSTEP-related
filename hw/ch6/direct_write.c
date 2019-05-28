/* hello world with no include directives */
const char msg[] = "Hello, world!\n" ;

int main() {
	
	unsigned long long int msglen, __NR_write, fd, retcode ;
	char * msg_it ;
	void * msgptr ;

	fd = 1;					/* STDOUT_FILENO */
	msglen = 0 ;				/* will hold the number of bytes to write */
	__NR_write = 1 ;			/* number for sys_write in /usr/include/asm/unistd_64.h */
	retcode = 666 ;				/* dummy value */
	msgptr = (void *)&msg ;			/* store location of message */

	/* Determine the size of the message */
	msg_it = (char *)msg ;
	while(*msg_it++) msglen++ ;


	/*
	 Call Linux syscall sys_write using x86_64 Linux calling conventions
	 i.e.:
	 	retcode = SYS_write(STDOUT_FILENO, &msg, sizeof(msg)) ;
	 		
	 		i.e.:
	 			retcode = printf("%s",msg) ;
	*/
	__asm__ __volatile__ (
		"movq %1, %%rdi\n\t"
		"movq %2, %%rsi\n\t"
		"movq %3, %%rdx\n\t"
		"movq %4, %%rax\n\t"
		"syscall\n\t"
		"movq %%rax, %0\n\t"
		: "=r" (retcode)
		: "r" (fd), "r" (msgptr), "r" (msglen), "r" (__NR_write)
		: "%rdi", "%rsi", "%rdx", "%rax"
	);

	return 0 ;
}

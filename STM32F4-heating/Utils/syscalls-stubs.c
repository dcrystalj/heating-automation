/*
 * syscalls-stubs.c
 *
 *  Created on: Mar 7, 2013
 *      Author: patricio
 */

#include "syscalls-stubs.h"

/*-----------------------------------------------------------*/
/*
 * sbrk()
 *  Increase program data space.
 *  sbrk is a basic memory management system call used to control the amount
 *  of memory allocated to the data segment of the process.
 * Malloc and related functions depend on this
 * Pa3cio, 7.3.2013
*/
caddr_t _sbrk(int incr) {

	extern char _ebss; // Defined by the linker
    static char *heap_end;
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_ebss;
    }
    prev_heap_end = heap_end;

    char * stack = (char*) __get_MSP();
    if (heap_end + incr >  stack)
    {
    	/* Heap and stack collision */
    	return  (caddr_t) -1;
        //abort ();
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}



/*-----------------------------------------------------------*/


int _close(int file) {
	errno = ENOTSUP;
	return -1;
}

int _fstat(int file , struct stat *st) {
	st ->st_mode = S_IFCHR; // character device
	return 0;
}


int _isatty(int file) {
	return 1;
}

int _link(char *old , char *new) {
	errno = EMLINK;
	return -1;
}

int _lseek(int file , int ptr , int dir) {
	errno = ENOTSUP;
	return -1;
}

int _open(const char *name , int flags , int mode) {
	errno = ENOTSUP;
	return -1;
}


int _read(int file , char *ptr , int len) {
	if (len){
		//*ptr = (char) uart_getc(USART1);
		return 1;
	}
	return 0;
}


int _unlink(char *name) {
	errno = ENOENT;
	return -1;
}

int _write(int file , char *ptr , int len) {
	int n;
	for (n = 0; n < len; n++) {
		fputc( (int)(*ptr), file );
		ptr++;
	}
	return len;
}



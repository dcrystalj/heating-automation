/*
 * syscalls-stubs.h
 *
 *  Created on: Mar 7, 2013
 *      Author: patricio
 */

#ifndef SYSCALLS_STUBS_H_
#define SYSCALLS_STUBS_H_

/* Standard includes. */
#include "stm32f4xx.h"
#include "retarget.h"
#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>


caddr_t _sbrk(int incr);
int _close(int file);
int _fstat(int file , struct stat *st);
int _isatty(int file);
int _link(char *old , char *new);
int _lseek(int file , int ptr , int dir);
int _open(const char *name , int flags , int mode);
int _read(int file , char *ptr , int len);
int _unlink(char *name);
int _write(int file , char *ptr , int len);




#endif /* SYSCALLS_STUBS_H_ */

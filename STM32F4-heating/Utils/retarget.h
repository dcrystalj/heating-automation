#ifndef RETARGET_H_
#define RETARGET_H_


/* Standard includes. */
#include <stdio.h>

#include "SSD1289.h"
#include "stm32f4xx.h"

/*
 * Retargets the C library printf function to the LCD.
 */
int fputc( int ch, FILE *f );

#endif

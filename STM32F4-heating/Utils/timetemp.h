#ifndef TIMETEMP_H_
#define TIMETEMP_H_

//#include <stdlib.h>
#include <stdio.h>
#include "itoa.h"
#include "stm32f4xx.h"


/*
        Time structure.

        Pa3cio Bulic, 10.1.2013
*/
typedef struct {
	uint8_t hours; 
  uint8_t minutes;
  uint8_t seconds;
  uint8_t shours[2];
  uint8_t sminutes[2];
  uint8_t sseconds[2];
  uint8_t timestring[9];
} time_structure_t;


typedef struct {
  int8_t  temperature;
  int8_t  temp_frac;
  uint8_t count_remain;
  uint8_t count_per_c;
  uint8_t stemperature[2];
  uint8_t stemp_frac[2];
  uint8_t temperature_string[6];
} temperature_structure_t;



void CalculateTime(time_structure_t* time);
void FormatTimeString(time_structure_t* time);
void FormatTimeString2(time_structure_t* time);
void FormatTemperatureString(temperature_structure_t* temperature);
void FormatTemperatureStringHR(temperature_structure_t* temperature);

#endif

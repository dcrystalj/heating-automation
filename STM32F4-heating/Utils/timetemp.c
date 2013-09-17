
#include "timetemp.h"

/**
  * @brief  Calculates new time.
  * @param  None
  * @retval None
  * Pa3cio, 14.1.2013
  */
void CalculateTime(time_structure_t* time) {
  time->seconds++;
  if (time->seconds == 60) {
    time->seconds = 0;
    time->minutes++;
    if (time->minutes == 60) {
      time->minutes = 0;
      time->hours++;
      if (time->hours == 24) {
        time->hours = 0;
      } 
    }  
  }
}



/**
  * @brief  Format time string.
  * @param  None
  * @retval None
  * Pa3cio, 18.3.2013
  */
void FormatTimeString2(time_structure_t* time) {
    //time->timestring[6] = '0' + time->seconds/10;
    //time->timestring[7] = '0' + time->seconds%10;
    
    //time->timestring[5] = ':';
    
    time->timestring[3] = '0' + time->minutes/10;
    time->timestring[4] = '0' + time->minutes%10;
    
    time->timestring[2] = ' ';
    
    time->timestring[0] = '0' + time->hours/10;
    time->timestring[1] = '0' + time->hours%10;

    //time->timestring[8] = 0; // null terminated string
    time->timestring[5] = 0; // null terminated string
}


/**
  * @brief  Format time string.
  * @param  None
  * @retval None
  */
void FormatTimeString(time_structure_t* time) {
    //time->timestring[6] = '0' + time->seconds/10;
    //time->timestring[7] = '0' + time->seconds%10;

    //time->timestring[5] = ':';

    time->timestring[3] = '0' + time->minutes/10;
    time->timestring[4] = '0' + time->minutes%10;

    time->timestring[2] = ':';

    time->timestring[0] = '0' + time->hours/10;
    time->timestring[1] = '0' + time->hours%10;

    //time->timestring[8] = 0; // null terminated string
    time->timestring[5] = 0; // null terminated string
}



/**
  * @brief  Format temperature string - High Resolution.
  * @param  None
  * @retval None
  */
void FormatTemperatureStringHR(temperature_structure_t* temperature) {

  uint16_t fraction;


  /* calculate  */
  fraction = (temperature->count_per_c - temperature->count_remain) << 8;
  fraction = fraction / temperature->count_per_c;
  fraction = fraction >> 4;
  temperature->count_remain = ((uint8_t) fraction);


  switch (temperature->count_remain) {
    case 0:
    case 1:
      temperature->temperature_string[4] = '8';
      // correct the temperature
      temperature->temperature --;
      break;
    case 2:
    case 3:
      temperature->temperature_string[4] = '9';
      // correct the temperature
      temperature->temperature --;
      break;
    case 4:
      temperature->temperature_string[4] = '0';
      break;
    case 5:
    case 6:
      temperature->temperature_string[4] = '1';
      break;
    case 7:
      temperature->temperature_string[4] = '2';
      break;
    case 8:
    case 9:
      temperature->temperature_string[4] = '3';
      break;
    case 10:
    case 11:
      temperature->temperature_string[4] = '4';
      break;
    case 12:
      temperature->temperature_string[4] = '5';
      break;
    case 13:
    case 14:
      temperature->temperature_string[4] = '6';

      break;
    case 15:
      temperature->temperature_string[4] = '7';
      break;
    default:
      temperature->temperature_string[4] = '?';
      break;
  }

  temperature->temperature_string[1] = '0' + temperature->temperature/10;
  temperature->temperature_string[2] = '0' + temperature->temperature%10;

  temperature->temperature_string[3]  = '.';

  if (temperature->temperature < 0)
    temperature->temperature_string[0] = '-';
  else
    temperature->temperature_string[0] = '+';

  temperature->temperature_string[5] = '\0';
}


/**
  * @brief  Format time string.
  * @param  None
  * @retval None
  */
void FormatTemperatureString(temperature_structure_t* temperature) {
  
  if (temperature->temperature < 0)
    temperature->temperature_string[0] = '-';
  else
    temperature->temperature_string[0] = '+';
  
  temperature->temperature_string[1] = '0' + temperature->temperature/10;
  temperature->temperature_string[2] = '0' + temperature->temperature%10;

  temperature->temperature_string[3]  = '.';
  
  if (temperature->temp_frac != 0)
    temperature->temperature_string[4] = '5';
  else
    temperature->temperature_string[4] = '0';

  temperature->temperature_string[5] = '\0';
}


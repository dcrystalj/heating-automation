/*
 * I2C-DS1621.h
 *
 *  Created on: Mar 19, 2013
 *      Author: patricio
 */

#ifndef I2C_DS1621_H_
#define I2C_DS1621_H_

#include "stm32f4xx.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "FreeRTOSConfig.h"
/* LCD includes */
#include "LCD_Message.h"

/* Private define ------------------------------------------------------------*/
#define I2C_SPEED               100000
#define I2C_SLAVE_ADDRESS7      0x00
#define DS1621_READTEMP_CMD     0xAA
#define DS1621_STARTCONV_CMD    0xEE
#define DS1621_READCNT_CMD      0xA8
#define DS1621_READSLOPE_CMD    0xA9

/* Exported functions ------------------------------------------------------- */
void I2C_Configuration(void);
void DS1621_ReadTemp (int8_t *ptemperature_data);
void vReadTempTask (void *pvParameters);
void DS1621_WriteData (uint8_t data);
void DS1621_ReadData (int8_t* data);
void DS1621_SendCommand (uint8_t cmd, uint8_t direction);


#endif /* I2C_DS1621_H_ */

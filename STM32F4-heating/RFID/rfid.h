/*
 * rfid.h
 *
 *  Created on: Mar 10, 2013
 *      Author: patricio
 */

#ifndef RFID_H_
#define RFID_H_


#include "stm32f4_discovery.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "FreeRTOSConfig.h"
/* LCD includes. */
#include "LCD_Message.h"

/* Private define ------------------------------------------------------------*/
#define NOTAGS 11
#define MAXWAITTIME 0xFFFFFFFF

/* Exported functions ------------------------------------------------------- */
void RFIDInitMinimal();
void USART3_Config(void);
void NVIC_Configuration(void);
//ErrorStatus ReadRFIDTagCode(void);
uint32_t CheckRFIDTagCode(void);
inline void process_transponder(void);
void vRFIDTransponderTask( void *pvParameters );
void vToggleLED4Task (void *pvParameters);
void USART3_IRQHandler(void);
void STM_EVAL_PA8Init(void);
void STM_SSDRelay_On(void);
void STM_SSDRelay_Off(void);



#endif /* RFID_H_ */

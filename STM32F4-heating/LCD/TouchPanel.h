/*
 * TouchPanel.h
 *
 *  Created on: Mar 15, 2013
 *      Author: patricio
 */

#ifndef TOUCHPANEL_H_
#define TOUCHPANEL_H_


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
/* LCD includes */
#include "LCD_Message.h"
#include "SSD1289.h"

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2

/* Private typedef -----------------------------------------------------------*/
typedef struct Point
{
   int32_t x;
   int32_t y;
}POINT;


typedef struct MATRIX
{
int32_t     An,
            Bn,
            Cn,
            Dn,
            En,
            Fn,
            Divider ;
} MATRIX ;

/* Private variables ---------------------------------------------------------*/
extern POINT ScreenSample[3];
extern POINT DisplaySample[3];
extern MATRIX matrix ;
extern POINT  display ;

extern LCD_ScreenState xLCDScreenState;

/* Private define ------------------------------------------------------------*/

#define CHX   0x90
#define CHY   0xd0

/* Private function prototypes -----------------------------------------------*/
void TP_Init(void);
POINT *Read_Ads7846(void);
void TouchPanel_Calibrate(void);
void DrawCross(uint16_t Xpos,uint16_t Ypos);
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos);
ErrorStatus setCalibrationMATRIX( POINT *displayPtr,POINT *screenPtr, MATRIX *matrixPtr);
ErrorStatus getDisplayPoint(POINT *displayPtr,POINT *screenPtr, MATRIX *matrixPtr );
void vTouchPanelTask( void *pvParameters );

void delay_ms(__IO uint32_t ms);
void TP_Delay(__IO uint32_t nCount);
void DelayUS(__IO uint32_t cnt);

#endif /* TOUCHPANEL_H_ */

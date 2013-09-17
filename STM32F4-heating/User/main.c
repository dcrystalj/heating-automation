/*The MIT License (MIT)

Copyright (c) 2013 Tomaž Tomažič & Patricio Bulić

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


@Author of touch panel, LCD, RFID and DS1621 libraries is Patricio Bulić.
@Author of OneWire and DS1820 library is Vojtech Vigner
@Author of DS1307 is Tomaž Tomažič
*/

/* Standard includes. */
#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "defs.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* LCD includes */
#include "SSD1289.h"
#include "SSD1289.c"
#include "LCD_Message.h"
#include "TouchPanel.h"
#include "TouchPanel.c"
//include "touch_7846.h"
#include "pic_hdr.h"


/* Utilities includes */
#include "string.h"
#include "itoa.h"
#include "delay.h"
#include "timetemp.h"

#include "DS1820.c"
#include "OneWire.c"

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
/* The queue used to send messages to the LCD gate-keeper task. */
xQueueHandle xLCDQueue;

/* Private define ------------------------------------------------------------*/
#define DELAY1 0x0FFFFF

/* The maximum number of message that can be waiting for display at any one
time. */
#define mainLCD_QUEUE_SIZE          ( 3 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

int8_t pTime_data[64];

time_structure_t time;

uint16_t sec_elapsed;
uint16_t halfsec_elapsed;

FlagStatus switchTimeFormat;
FlagStatus xDrawSetTimeScreen = SET;

FlagStatus fSystemTask = RESET;

xTaskHandle xScreenSaverHandle;
xTaskHandle xTimerFunctionHandle;
xTaskHandle xSystemFunctionHandle;
xTaskHandle xRefreshHandle;


int8_t ab[2] = {11, 0x80};
int8_t a,b;

LCD_ScreenState xLCDScreenState;

/* Private variables ---------------------------------------------------------*/
  int iTemperature[MAX_TEMP_SESNORS];

/* For relays screen */
int8_t selectedRelay = -1;

/* Temperature sensor addresses */
uint64_t iRomAddress[] = {
  12538021383148670248,   //A
  10664523938158834216,   //B
  12898309353620190760,   //C
  8718968899422057768,    //D
  4611686038976452136    //E
};

FlagStatus faFurnaceRelay[MAX_CONDITIONS];
FlagStatus faHeatRelay[MAX_CONDITIONS];
FlagStatus faFloorRelay[MAX_CONDITIONS];
FlagStatus faGroundRelay[MAX_CONDITIONS];
FlagStatus faAtticRelay[MAX_CONDITIONS];
FlagStatus faBathroomRelay[MAX_CONDITIONS];
FlagStatus faBoilerRelay[MAX_CONDITIONS];
FlagStatus faResetRelay[MAX_CONDITIONS];

FlagStatus faSensorAlive[MAX_TEMP_SESNORS];
FlagStatus faSystemConditions[MAX_CONDITIONS];


////put relays to array
FlagStatus *pFsRelays[MAX_RELAYS] = {
  faFloorRelay, faGroundRelay,  faBathroomRelay, faAtticRelay,  faBoilerRelay, faFurnaceRelay, faHeatRelay, faResetRelay
};
//MUST BE consistent with pFsRelays array!!
int16_t piRelays[MAX_RELAYS] = {
  FLOOR_R, GROUND_R, BATHROOM_R, ATTIC_R, BOILER_R, FURNACE_R, HEATER_R, RESET_R
};
//MUST BE consistent with pFsRelays array!!
int8_t pStatusRelays[MAX_RELAYS] = {
  AUTO, AUTO, AUTO, AUTO, AUTO, AUTO, AUTO, AUTO
};

char* cInt2Temp(uint16_t i);
void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName );
void vApplicationMallocFailedHook(void);
void vTimerFunction (void *pvParameters);
void vTemperatureTask (void *pvParameters);
void vConditionTask (void *pvParameters);
void vSystemTask (void *pvParameters);
void vRefreshTask (void *pvParameters);

void ScreenNormal(xLCDMessage *xpMessage);
void ScreenSetTime(xLCDMessage *xpMessage);
void ScreenInfo (xLCDMessage *xpMessage);
void ScreenSettings (xLCDMessage *xpMessage);
void ScreenRelay (xLCDMessage *xpMessage);
void ScreenOff (xLCDMessage *xpMessage);

void vTurnOffRelay(uint16_t off);
void vTurnOnRelay(uint16_t on);

void vResetArray( FlagStatus *fsArray, int length) ;

void vSystemConditions();
void vThermostatConditions();
void vBoilerConditions();
void vFurnaceConditions();
void vHeaterConditions();
void vGroundConditions();

short int  iAllReset (FlagStatus *fsArray, int length);
short int  iNotAllReset (FlagStatus *fsArray, int length);
short int  iAllSet( FlagStatus *fsArray, int length);
short int  iOneReset (FlagStatus *fsArray, int length);
short int  iNotAllSet( FlagStatus *fsArray, int length);


void vInitGPIO();
void vInit();
void vInitIcons();

short int iCmpG(int a, int b);
short int iCmpL(int a, int b);

static void vLCDTask( void *pvParameters );


int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
  */

  /* set all temperature sensor to error while they are not initialized yet */
  int i;
  for (i = 0; i < MAX_TEMP_SESNORS; ++i)
    iTemperature[i] = DS1820_TEMP_ERROR;


  sec_elapsed = RESET;
  halfsec_elapsed = RESET;
  switchTimeFormat = RESET;

  time.hours = 20;
  time.minutes = 46;
  time.seconds = 37;
  
  vInitGPIO();
  vInit();

  vTaskStartScheduler();
  
  /* If all is well, main will never reach here, as the scheduler will now be running the tasks. */
  while (1){}
}

/*--------------------------- T A S K S --------------------*/



void vLCDTask( void *pvParameters )
{
  xLCDMessage xMessage;
  LCD_Clear(BLACK);

  xLCDScreenState = LCD_NORMAL;

  for( ;; )
  {

    /* Wait for a message to arrive that requires displaying. */
    while( xQueueReceive( xLCDQueue, &xMessage, portMAX_DELAY ) != pdPASS );

    switch( xLCDScreenState ) {
      case LCD_NORMAL:
        ScreenNormal(&xMessage);
        break;
      case LCD_SETTIME:
        ScreenSetTime(&xMessage);
        break;
      case LCD_SETTINGS:
        ScreenSettings(&xMessage);
        break;
      case LCD_INFO:
        ScreenInfo(&xMessage);
        break;
      case LCD_RELAY:
        ScreenRelay (&xMessage);
        break;
      case LCD_OFF:
        ScreenOff(&xMessage);
        break;
    }

  } //end for
}
/*-----------------------------------------------------------*/

void vTemperatureTask( void *pvParameters )
{
  xLCDMessage xMessage;

  uint8_t iRetry;
  uint8_t iCounter;

  int i;

  for( ;; )
  {

    OW_Reset();

    /* Check if sensors are connected and works */
    for (i = 0; i < MAX_TEMP_SESNORS; ++i)
    {
      if(OW_ROMMatch(iRomAddress[i]) == OW_OK)
        faSensorAlive[i] = SET;
      else
        faSensorAlive[i] = RESET;
    }
    /*-------------------------------*/

    do
    {


      for (i = 0; i < MAX_TEMP_SESNORS; i++)
      {
        if( faSensorAlive[i] == SET)
        {

          //init sensor to read temperature
          DS1820_TemperatureConvert(iRomAddress[i]);
          vTaskDelay( SECOND );
          iRetry = 0;

          //get temperature
          do {
			  iTemperature[i] = DS1820_TemperatureGet( iRomAddress[i] );
			  vTaskDelay( 100 / portTICK_RATE_MS );
			  iRetry++;

			  if(iTemperature[i] == 850){
				  DS1820_TemperatureConvert(iRomAddress[i]);
				  vTaskDelay( SECOND );
			  }
          } while (((iTemperature[i] == DS1820_TEMP_ERROR) && (iRetry < MAX_RETRIES) ) ||  //check if sensor has error value
        		  (iTemperature[i] == 850 && (iRetry < MAX_RETRIES))); 					   //check if sensor has default value


          faSensorAlive[i] = (iTemperature[i] == DS1820_TEMP_ERROR) ? RESET : SET;

        }
        else
        {
        	iTemperature[i] == DS1820_TEMP_ERROR;
        }

        // Update display temperature on home screen
	    xMessage.xMsgType =  TEMP;
	    xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

      }


      vTaskDelay( 5*SECOND );

      /* if all sensor works no need to check them again*/
    }while ( iAllSet(faSensorAlive, MAX_TEMP_SESNORS) );

    // You are here if cannot get temperature from all sensor, so try search again
    vTaskDelay( SECOND );

  } //endless loop for RTOS
}


void vConditionTask ( void *pvParameters )
{

  int8_t i;
  int8_t iRetry = 0;

  //wait for temperature sensors on boot
  while( iNotAllSet(faSensorAlive, MAX_TEMP_SESNORS) && iRetry < 5)
  {
    iRetry++;
    vTaskDelay( 10*SECOND );
  }

  for(;;)
  {

    /* check conditions */
    vBoilerConditions();
    vThermostatConditions();
    vGroundConditions();
    vHeaterConditions();

    /* Check to start/stop system task */
    vSystemConditions();

    /* These should be the last of conditions */
    vFurnaceConditions();



    /* turn on/off relays considering all above conditions  */
    for (i=0; i<MAX_RELAYS; i++){
        if (( iAllReset(pFsRelays[i], MAX_CONDITIONS) && pStatusRelays[i] == AUTO )
        			|| pStatusRelays[i] == OFF
        	)
        	vTurnOffRelay(piRelays[i]);
        else
        	vTurnOnRelay(piRelays[i]);

    }

    /* check conditions every second */
    vTaskDelay( SECOND );
  }
}


void vSystemTask ( void *pvParameters )
{
  for(;;)
  {

	  vTaskDelay(SECOND);

	  if( iTemperature[TOUTSIDE] > 0 || iTemperature[TOUTSIDE] == DS1820_TEMP_ERROR)
	  {

		  faFurnaceRelay[SYSTEM_COND] = RESET;

		  faHeatRelay[SYSTEM_COND] = SET;
		  int iTemperatureExchanger = iTemperature[TEXCHANGER];
		  vTaskDelay( 2*MINUTE ); //MINUTE

		  while(1)
		  {
			  /* If temperature has risen, clima works */
			  if (iTemperature[TEXCHANGER] > iTemperatureExchanger + 10 || iTemperature[TEXCHANGER] == DS1820_TEMP_ERROR) //OR ERROR
			  {

				  vTaskDelay( 10 * MINUTE );

				  while( iCmpG( iTemperature[TSYSTEM], 400)){
					vTaskDelay(MINUTE);
				  }

				  /* if clima cannot rise temperature enogh, furnace can help =) */
				  break;
			  }
			  else {

				/* Clima needs reset */
				faResetRelay[SYSTEM_COND] = SET;
				vTaskDelay ( 30 * SECOND );
				faResetRelay[SYSTEM_COND] = RESET;
				vTaskDelay ( 2 * MINUTE );

				/* Excanger is still cold, run furnace */
				if (iTemperature[TEXCHANGER] <= iTemperatureExchanger+10)  break;

			  }
		  }
	  }

	  /* Turn furnace on */
	  faFurnaceRelay[SYSTEM_COND] = SET;

	  vTaskSuspend(NULL);

  }
}

/*-----------------------------------------------------------*/
/* Display and calculate time */
void vTimerFunction (void *pvParameters){
  xLCDMessage xMessage;
  portTickType xLastWakeTime;
  
  xLastWakeTime = xTaskGetTickCount();
  
  while (1) {
    
    if (halfsec_elapsed == SET) {
      halfsec_elapsed = RESET;

      if (switchTimeFormat == SET) switchTimeFormat = RESET;
      else switchTimeFormat = SET;
    }

    if (sec_elapsed) {
      sec_elapsed = RESET;
      CalculateTime(&time);
    }
    
    if (switchTimeFormat == SET) FormatTimeString(&time);
    else FormatTimeString2(&time);


    xMessage.xMsgType = TIME;
    xMessage.pcMessage = time.timestring;

    /* Send the message to the LCD gate-keeper for display. */
    xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

    vTaskDelayUntil( &xLastWakeTime, (100 / portTICK_RATE_MS) );
  }
}

void vRefreshTask( void *pvParameters )
{
  xLCDMessage xMessage;
  LCD_Msg_Type x;
  int screenState = (int) pvParameters;

  for( ;; )
  {

    vTaskDelay(3*SECOND);

    if(xLCDScreenState == screenState ){

      //delete only part of the screen
      if(screenState == LCD_RELAY)
        xMessage.xColumn = 8;

      //refresh
      xMessage.xMsgType = LCDCLR;
      xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
    }

  }
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/
/*
 *  CONDITIONS
 */
/*-----------------------------------------------------------*/

void vThermostatConditions(){

  /* read pins, which thermostat is on */
  uint16_t thermostat = GPIO_ReadInputData(GPIOC);

  int8_t i;
  uint8_t condition = THERMOSTAT1;

  FlagStatus *pThermostatRelays[MAX_THERMOASTATS] = { faFloorRelay, faGroundRelay, faBathroomRelay, faAtticRelay };
  uint8_t *pTstat[MAX_THERMOASTATS] = { TSTAT1, TSTAT2, TSTAT3, TSTAT4 };

  /* loop through all relays */
  for (i=0; i<MAX_THERMOASTATS; i++, condition++){

    /* if thermostat is on, set condition so */
    if ( thermostat & (uint16_t) pTstat[i] )
      {

         faSystemConditions[condition] = SET;
         pThermostatRelays[i][condition] = SET;

         if(iTemperature[TBOILER] == DS1820_TEMP_ERROR)
        	 faBoilerRelay[condition] = SET;
      }
      else
      {
         faSystemConditions[condition] = RESET;
         pThermostatRelays[i][condition] = RESET;

         if(iTemperature[TBOILER] == DS1820_TEMP_ERROR)
        	 faBoilerRelay[condition] = RESET;
      }
  }

}
void vFurnaceConditions()
{
    /* furnace prevent explosion condition */
    if ( iTemperature[TFURNACE] > 900 || iTemperature[TFURNACE] == DS1820_TEMP_ERROR)
    {
		vTurnOffRelay(FURNACE_R);
		vResetArray( faFurnaceRelay, MAX_CONDITIONS );
    }
}

void vHeaterConditions()
{
    /* prevent frozen heater */
    if ( iCmpL( iTemperature[TOUTSIDE], 0 ) &&
				( iNotAllReset(faBoilerRelay, MAX_CONDITIONS) ||
				iNotAllReset(faFloorRelay, MAX_CONDITIONS) ||
				iNotAllReset(faAtticRelay, MAX_CONDITIONS) ||
				iNotAllReset(faGroundRelay, MAX_CONDITIONS) ||
				iNotAllReset(faBathroomRelay, MAX_CONDITIONS)))
    {
		vTurnOffRelay(HEATER_R);
		faFurnaceRelay[SYSTEM_COND] = SET;
		vResetArray( faHeatRelay, MAX_CONDITIONS );
    }
}
void vGroundConditions(){
	// system has GT temperature than 28 and is winter, and no other use heat
	if( iCmpL( iTemperature[TOUTSIDE], 150) && iCmpG( iTemperature[TSYSTEM], 250) &&
					iAllReset(faFurnaceRelay,MAX_CONDITIONS) &&
					iAllReset(faHeatRelay,MAX_CONDITIONS) &&
					iAllReset(faBoilerRelay,MAX_CONDITIONS))
		faGroundRelay[EXTRA_HEAT] = SET;
	else
		faGroundRelay[EXTRA_HEAT] = RESET;
}

void vBoilerConditions()
{
  /* if temperature in boiler is to low, start the system  */
    if ( iCmpL(iTemperature[TBOILER], 450))
    {
     faSystemConditions[LOW_TBOILER] = SET;
     faBoilerRelay[LOW_TBOILER] = SET;
    }
    else
    {
     faSystemConditions[LOW_TBOILER] = RESET;
     faBoilerRelay[LOW_TBOILER] = RESET;
    }

    /* if temperature of system is > boiler should turn boiler relay on */
    faBoilerRelay[SYSTEM_GT_FURNACE] = (iCmpG(iTemperature[TSYSTEM], iTemperature[TBOILER]+10)) ? SET : RESET;
}

void vSystemConditions()
{

  if ( iAllReset(faSystemConditions, MAX_CONDITIONS)){
    /* if task is already running, should be deleted */
    if( xSystemFunctionHandle != NULL )
    {
      vTaskDelay( ( portTickType ) 0 );
      vTaskDelete(xSystemFunctionHandle);
      xSystemFunctionHandle = NULL;
      fSystemTask = RESET;
    }

    /* also with all it's conditions */
    vResetArray( faFurnaceRelay, MAX_CONDITIONS );
    vResetArray( faHeatRelay, MAX_CONDITIONS);
    vResetArray( faResetRelay, MAX_CONDITIONS);

    vTurnOffRelay(FURNACE_R);
    vTurnOffRelay(HEATER_R);
    vTurnOffRelay(RESET_R);
    }
    /* if boiler or one of thermostats is on --> start system task  */
  else if( iNotAllReset(faSystemConditions, MAX_CONDITIONS) && xSystemFunctionHandle == NULL ) {
	  xTaskCreate( vSystemTask, ( const signed char * ) "SYSTEM", configMINIMAL_STACK_SIZE, ( void* ) NULL, tskIDLE_PRIORITY + 1UL, &xSystemFunctionHandle );
	  fSystemTask = SET;
    }
}


/*-----------------------------------------------------------*/
/*
 *  S C R E E N S
 */
/*-----------------------------------------------------------*/

void ScreenNormal (xLCDMessage *xpMessage) {

  vInitIcons();

  switch (xpMessage->xMsgType) {
    case TIME:
      LCD_CharSize(24);
      LCD_StringLine(80, 30, xpMessage->pcMessage);
      LCD_CharSize(16);
      break;
    case TEMP:
      vTaskDelay(SECOND/2);
      LCD_CharSize(24);
      int i;
      char string[15];
      char text[MAX_TEMP_SESNORS][10] = {
        "PEC      ",
        "SISTEM   ",
        "BOILER   ",
        "ZUNAJ    ",
        "IZMENJ.  "
      };
      uint8_t temperatureConst[MAX_TEMP_SESNORS] = {
        TFURNACE, TSYSTEM, TBOILER, TOUTSIDE, TEXCHANGER
      };

      for (i=0; i<MAX_TEMP_SESNORS; i++){

        //get temperature
        int temp = iTemperature[temperatureConst[i]];

        //red
        if(temp == DS1820_TEMP_ERROR){
          sprintf(string, "%sERR ", text[i]);
          LCD_SetTextColor(RED);
          LCD_StringLine(5, 90 + i*30, string);
          LCD_SetTextColor(WHITE);
          continue;
        }

        //white
        sprintf(string, "%s%d.%d", text[i], iTemperature[i]/10,
          abs(iTemperature[i]-iTemperature[i]/10*10));

        //draw
        LCD_StringLine(5, 90 + i*30, string);
      }

      LCD_CharSize(16);
      break;
    case UNAME:
      LCD_StringLine(8,150,xpMessage->pcMessage);
      break;
    case IMAGE:
      LCD_ClearRectangle(0, 0, 180, 240, BLACK);
      LCD_WriteBMP(
          xpMessage->stBitmapProperties.Xpos,
          xpMessage->stBitmapProperties.Ypos,
          xpMessage->stBitmapProperties.Height,
          xpMessage->stBitmapProperties.Width,
          xpMessage->pBitmap);
      break;
    case LCDCLR:
      LCD_Clear(BLACK);
      vTaskDelay(500 / portTICK_RATE_MS );
      vInitIcons();
      break;
  }
}


void ScreenSetTime(xLCDMessage *xpMessage) {

  if (xDrawSetTimeScreen == RESET) {
    switch (xpMessage->xMsgType) {
      case TIME:
        LCD_CharSize(24);
        LCD_StringLine(80, 30, xpMessage->pcMessage);
        LCD_CharSize(16);
        break;
    }
  }

  if (xDrawSetTimeScreen == SET) {
    // this is a constant "picture" on LCD:

    xDrawSetTimeScreen = RESET;

    switch (xpMessage->xMsgType) {
      case LCDCLR:
        LCD_Clear(BLACK);
        LCD_SetTextColor(LCD_COLOR_BLACK);
        LCD_DrawFullRect(0, 0, LCD_WIDTH , LCD_HEIGHT);
        LCD_SetTextColor(LCD_COLOR_WHITE);
        LCD_Clear(BLACK);
        vTaskDelay(500 / portTICK_RATE_MS );
        break;
      //case TIME:
        //LCD_CharSize(24);
        //FormatTimeString(&time);
        //LCD_StringLine(80, 30, time.timestring);
        //LCD_CharSize(16);
        //break;
    }

    LCD_StringLine(64,5, msg_SetTime);
    LCD_CharSize(24);
    FormatTimeString(&time);
    LCD_StringLine(80, 30, time.timestring);
    LCD_CharSize(16);
    LCD_StringLine(LCD_HOURP_X+18, LCD_HOURP_Y-20, msg_URA);
    LCD_StringLine(LCD_MINP_X+6, LCD_HOURP_Y-20, msg_MINUTE);

    LCD_WriteBMP( LCD_HOURP_X, LCD_HOURP_Y, BUTTON_H, BUTTON_H, pic_arrow_up_60x60_jpg);
    LCD_WriteBMP( LCD_MINP_X, LCD_HOURP_Y, BUTTON_H, BUTTON_H, pic_arrow_up_60x60_jpg);
    LCD_WriteBMP( LCD_HOURM_X, LCD_HOURM_Y, BUTTON_H, BUTTON_H, pic_arrow_down_60x60_jpg);
    LCD_WriteBMP( LCD_MINM_X, LCD_MINM_Y, BUTTON_H, BUTTON_H, pic_arrow_down_60x60_jpg);

    LCD_WriteBMP( LCD_ICONS_START_X, LCD_ICONS_START_Y, BUTTON_H, BUTTON_H, pic_cancel_60x60_jpg);
    LCD_WriteBMP( LCD_ICONS_START_X + BUTTON_H * 3, LCD_ICONS_START_Y, BUTTON_H, BUTTON_H, pic_ok_60x60_jpg);
  }
}



void ScreenSettings (xLCDMessage *xpMessage) {
  switch (xpMessage->xMsgType) {
    case LCDCLR:
      LCD_Clear(BLACK);
      vTaskDelay(500 / portTICK_RATE_MS );
      LCD_StringLine(36,10,"N A S T A V I T V E :\0");
      LCD_WriteBMP( LCD_ICONS_START_X, LCD_ICONS_START_Y, 60, 60, pic_arrL3_png);
      break;

  }
}



void ScreenInfo (xLCDMessage *xpMessage) {

  switch (xpMessage->xMsgType) {
    case LCDCLR:
      LCD_Clear(BLACK);
      vTaskDelay(500 / portTICK_RATE_MS );
      //LCD_DrawRect(0, 0, 239, 319);
      LCD_StringLine(95,10, msg_INFO_TITLE);
      LCD_StringLine(25,150, msg_INFO);
      LCD_StringLine(104,200,msg_YEAR);
      LCD_StringLine(68,220,msg_VERSION);
      LCD_WriteBMP( LCD_ICONS_START_X, LCD_ICONS_START_Y, 60, 60, pic_arrL3_png);
      break;

  }
}

void ScreenRelay (xLCDMessage *xpMessage) {

	int i;

  switch (xpMessage->xMsgType) {
    case LCDCLR:

      if(xpMessage->xColumn != 8)
        LCD_Clear(BLACK);
      else
        //LCD_ClearRectangle(180, 0, 60, LCD_HEIGHT - LCD_ICONS_START_Y, BLACK );

		vTaskDelay(300 / portTICK_RATE_MS );
	    LCD_WriteBMP( LCD_ICONS_START_X, LCD_ICONS_START_Y, 60, 60, pic_arrL3_png);
		LCD_WriteBMP( LCD_ICONS_START_X+BUTTON_H, LCD_ICONS_START_Y, 60, 60, pic_auto_png);
		LCD_WriteBMP( LCD_ICONS_START_X+2*BUTTON_H, LCD_ICONS_START_Y, 60, 60, pic_on_png);
		LCD_WriteBMP( LCD_ICONS_START_X+3*BUTTON_H, LCD_ICONS_START_Y, 60, 60, pic_off_png);

      LCD_CharSize(24);


    char string[17];

    char text[8][15] = {
        "NADSTROPJE",
        "PRITLICJE ",
        "KOPALNICA ",
        "MANSARDA  ",
        "BOJLER    ",
        "PEC       ",
        "TOPLOTNA C",
        "RESET     "
    };

    /* print relay screen */
    for (i=0; i<MAX_RELAYS; i++){

    	if (( iAllReset(pFsRelays[i], MAX_CONDITIONS) && pStatusRelays[i] == AUTO )
    	      		  || pStatusRelays[i] == OFF )
    		sprintf(string, "%s OFF", text[i]);
		else
			sprintf(string, "%s ON ", text[i]);

      if (i == selectedRelay)
    	  LCD_SetBackColor(BLUE);

      if (pStatusRelays[i] != AUTO)
    	  LCD_SetTextColor(RED);

      LCD_StringLine(5, 5 + i*30, string);

      LCD_SetBackColor(BLACK);
      LCD_SetTextColor(WHITE);

    }
      break;
    default: break;
  }
}

void ScreenOff (xLCDMessage *xpMessage) {

  switch (xpMessage->xMsgType) {
    case LCDCLR:
      LCD_Clear(BLACK);
      LCD_BackLight(0);
      vTaskDelay(1000 / portTICK_RATE_MS );
      break;
  }
}









/*-----------------------------------------------------------*/

/*  INITS                            */

/*-----------------------------------------------------------*/
void vInitGPIO(){


  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  /* Relays */
  GPIO_InitStructure.GPIO_Pin   = FLOOR_R | GROUND_R | BATHROOM_R | ATTIC_R | BOILER_R | FURNACE_R | HEATER_R | RESET_R ;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  /* turn off relays */
  GPIO_SetBits( GPIOA, FLOOR_R | GROUND_R | BATHROOM_R | BOILER_R |  ATTIC_R | FURNACE_R | HEATER_R | RESET_R );

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /* Thermostats input enable */
  GPIO_InitStructure.GPIO_Pin =  TSTAT1 | TSTAT2 | TSTAT3 | TSTAT4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void vInit(){
  Delay(0x3FFFFF);
  LCD_Init();
  Delay(0x3FFFFF);
  LCD_Clear(BLACK);
  LCD_SetTextColor(WHITE);
  LCD_SetBackColor(BLACK);
  LCD_BackLight(50);


  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCD_ClearRectangle(0, 0, 180, 240, BLACK);
    LCD_SetCursor(0, 10);
  #endif

  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCDprint( "Initializing peripherals....\n" );
    Delay(DELAY1);
  #endif

  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCDprint( "Initializing Touch panel....\n" );
    Delay(DELAY1);
  #endif

    /* Init touch panel */
    TP_Init();

  #if ( defsCALIBRATE_TOUCH_PANEL == 1 )
    TouchPanel_Calibrate();
  #endif

	#if ( defsTEMPERATURE_INIT == 1 )
	  LCDprint( "Initializing temperature sensors\n" );
	  DS1820_Init();
	  Delay(DELAY1);
	#endif

  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCDprint( "Starting FreeRTOS....\n" );
    Delay(DELAY1);
  #endif

  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCDprint( "Creating queues....\n" );
    Delay(DELAY1);
  #endif
    /* Create the queue used by the LCD task.  Messages for display on the LCD
     are received via this queue. */
    xLCDQueue = xQueueCreate( mainLCD_QUEUE_SIZE, sizeof( xLCDMessage ) );

  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCDprint( "Creating tasks....\n" );
    Delay(DELAY1);
  #endif

    /* Create the  tasks */
    xTaskCreate( vTimerFunction, ( const signed char * ) "Timer", configMINIMAL_STACK_SIZE, (void *) NULL, tskIDLE_PRIORITY + 1UL, &xTimerFunctionHandle );
    xTaskCreate( vLCDTask,    ( const signed char * ) "LCD", (2348), NULL, tskIDLE_PRIORITY + 1UL, NULL );
    xTaskCreate( vTouchPanelTask, ( const signed char * ) "TP", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, NULL );
    xTaskCreate( vTemperatureTask, ( const signed char * ) "TEMP", (2348), NULL, tskIDLE_PRIORITY + 1UL, NULL );
    xTaskCreate( vConditionTask, ( const signed char * ) "CONDITION", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1UL, NULL );

  #if ( defsPRINT_INTRO_MSGS == 1 )
    LCDprint( "Start the scheduler....\n" );
    Delay(DELAY1);
  #endif

    LCD_SetTextColor(LCD_COLOR_BLACK);
    LCD_DrawFullRect(0, 0, LCD_WIDTH , LCD_HEIGHT);
    LCD_SetTextColor(LCD_COLOR_WHITE);
}


void vInitIcons(){
  LCD_WriteBMP( LCD_ICONS_START_X, LCD_ICONS_START_Y, 60, 60, pic_clock3_png);
  LCD_WriteBMP( LCD_ICONS_START_X+BUTTON_H, LCD_ICONS_START_Y, 60, 60, pic_sett3_png);
  LCD_WriteBMP( LCD_ICONS_START_X+2*BUTTON_H, LCD_ICONS_START_Y, 60, 60, pic_switch3_png);
  LCD_WriteBMP( LCD_ICONS_START_X+3*BUTTON_H, LCD_ICONS_START_Y, 60, 60, pic_i_png);

}














/*-----------------------------------------------------------*/
/*
 * H O O K S
 */
/*-----------------------------------------------------------*/
void vApplicationTickHook( void ) {
  /*
   Tick hook function executes within the context of the tick interrupt, and so must be kept very
   short, must use only a moderate amount of stack space, and must not call any FreeRTOS API
   function whose name does not end with 'FromISR()'
   */
  static uint16_t ms_count;

  if (ms_count < configTICK_RATE_HZ) ms_count++ ;
  else {
    ms_count = 0;
      sec_elapsed = SET;
  }

  if (ms_count == configTICK_RATE_HZ / 2) halfsec_elapsed = SET;

}



/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
  ( void ) pcTaskName;
  ( void ) pxTask;
  char str[10] = "OVERFLOW";
  LCD_StringLine(5, 5, str);
  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	char str[10] = "MALLOCFAIL";
	LCD_StringLine(5, 5, str);
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */
  taskDISABLE_INTERRUPTS();
  for( ;; );
}
/*-----------------------------------------------------------*/




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif














/*-----------------------------------------------------------*/

/*  * HELPERS                          */

/*-----------------------------------------------------------*/

char* cInt2Temp(uint16_t i) {
  char string[20];

  if( faSensorAlive[i] == SET)
    sprintf(string, "%d.%d C", iTemperature[i]/10, ((int)iTemperature[i] - ((int)iTemperature / 10) * 10) );
  else
    sprintf(string, "ERROR");

  return string;
}


short int iAllReset( FlagStatus *fsArray, int length){
  int i;
  for (i = 0; i < length; ++i) {
    if( fsArray[i] == SET )
      return 0;
  }
  return 1;
}

short int iNotAllReset( FlagStatus *fsArray, int length){
  return !iAllReset(fsArray, length);
}

short int iOneReset( FlagStatus *fsArray, int length){
  int i;
  for (i = 0; i < length; ++i){
    if( fsArray[i] == RESET )
      return 1;
  }
  return 0;
}

short int iNotAllSet( FlagStatus *fsArray, int length){
  return iOneReset( fsArray, length);
}

short int iAllSet( FlagStatus *fsArray, int length){
  return !iOneReset( fsArray, length);
}

/* First argument is greater then second */
short int iCmpG(int a, int b){
  if (a>b && a != DS1820_TEMP_ERROR && b != DS1820_TEMP_ERROR)
    return 1;

  return 0;
}

/* Second argument is greater then second */
short int iCmpL(int a, int b){
  if (a<b && a != DS1820_TEMP_ERROR && b != DS1820_TEMP_ERROR)
    return 1;

  return 0;
}

void vResetArray( FlagStatus *fsArray, int length) {
  int i;
  for (i = 0; i < length; ++i){
  fsArray[i] = RESET;
  }
}

void vTurnOffRelay(uint16_t off) {

	GPIO_SetBits(GPIOA, off);
}
void vTurnOnRelay(uint16_t on) {

  GPIO_ResetBits(GPIOA, on);
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

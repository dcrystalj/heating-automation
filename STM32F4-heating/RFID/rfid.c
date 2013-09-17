/*
 * rfid.c
 *  RFID RERADER is coonected via USART3 Tx on PB11
 *  Created on: Mar 10, 2013
 *      Author: patricio
 */


/* Includes ------------------------------------------------------------------*/
#include "rfid.h"


/* Exported constants --------------------------------------------------------*/
/*
Table of valid RFID tags.
If you add/remove a tag from table, correct the NOTAGS constant!!!
*/
const uint16_t RFIDValidCodes[] = {
  '0','F','0','0','3','0','0','9','2','2','2','4','0','C',  // FRI
  '4','9','0','0','0','2','4','6','3','5','3','9','0','4',  // blue dongle 1
  '4','9','0','0','0','2','5','3','4','3','7','7','1','F',  // blue dongle 2
  '3','1','0','0','1','2','7','3','6','9','8','8','0','1',  // FRI-menza dongle
  '4','7','0','0','0','6','9','9','8','4','9','5','0','8',  // Bela-trda kartica
  '0','5','0','0','1','4','2','8','6','9','2','9','0','6',
  '4','B','0','0','1','0','8','3','9','6','4','9','0','7',
  '0','5','0','0','1','2','8','8','4','8','7','2','0','F',
  '4','B','0','0','1','0','8','4','7','8','1','7','0','3',
  '0','3','0','0','0','9','1','0','0','5','3','8','0','5',
  '0','5','0','0','1','2','9','9','0','8','8','3','0','C'
};


/*
Table of valid user names.
If you add/remove a user from table, correct the NOTAGS constant!!!
*/
const uint8_t RFIDUserNames[NOTAGS+2][31] = {
  "       Access Denied!       \0",  // Neznan uporabnik
  "        Janez Jansa         \0",  //
  "       Zoran Jankovic       \0",  //
  "        Sadam Husein        \0",  //
  "      George Bush, jr.      \0",  //
  "      Papa Francesko I      \0",  //
  "       Alenka Bratusek      \0",  //
  "      Patricio Bulic 0      \0",  //
  "      Patricio Bulic 1      \0",  //
  "      Patricio Bulic 2      \0", //
  "      Patricio Bulic 3      \0",  //
  "      Patricio Bulic 4      \0",  //
  "                            \0"   //  Empty string

};




static xSemaphoreHandle xRFIDSemaphore;
static xTaskHandle xLEDBlinkHandle;

/* The queue used to send message to LCD gate-keeper task  */
extern xQueueHandle xLCDQueue;



/* Globals -------------------------------------------------------------------*/
uint16_t RFIDTagCode[14];
FlagStatus nStartReadRFIDTransponder = RESET;
FlagStatus nLFReceived = RESET;
FlagStatus nCRReceived = RESET;
uint32_t userID = NOTAGS+1;

int nDataCount = 0;

void vRFIDTransponderTask( void *pvParameters ) {
  xLCDMessage xMessage;

  /*
     * Take the semaphore once to start so the semaphore is empty before the
     * infinite loop is started.
     * */
  xSemaphoreTake(xRFIDSemaphore, 0);
  /* Start read the transponder */
  nStartReadRFIDTransponder = SET;


  // clear the name from LCD:
  userID = NOTAGS+1;
  xMessage.xMsgType = UNAME;
  xMessage.pcMessage = RFIDUserNames[userID];
  /* Send the message to the LCD gate-keeper for display. */
  xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

  // create LED blinking task
  xTaskCreate(
        vToggleLED4Task,                  /* Pointer to the function that implements task*/
        ( const signed char * ) "LED4",   /* Task name - for debugging only*/
        configMINIMAL_STACK_SIZE,         /* Stack depth in words */
        ( void* ) NULL,                   /* Pointer to tasks arguments (parameter) */
        tskIDLE_PRIORITY + 1UL,           /* Task priority*/
        &xLEDBlinkHandle                  /* Task handle */
    );

  for (;;) {
    /*
       * Use the semaphore to wait for the event. The task blocks indefinitely - this function call
       * will return once the semaphore has been successfully obtained.
       * */
    xSemaphoreTake(xRFIDSemaphore, portMAX_DELAY);

    /*
     * The ISR has given the semaphore, so process the event : */
    vTaskSuspend( xLEDBlinkHandle ); /* temporarly suspend LED blinking task */
    //process_transponder();           /* process RFID */

    /* RFID Transponder has been detected and read, so validate its code  */

    /* Disable further read into buffer */
    nStartReadRFIDTransponder = RESET;

    /* Transponder has sent the RFID code, so validate it  */
    userID = CheckRFIDTagCode();
    if (CheckRFIDTagCode() != ERROR ) {
      STM_EVAL_LEDOn(LED4);
      STM_SSDRelay_On();
      xMessage.xMsgType = UNAME;
      xMessage.pcMessage = RFIDUserNames[userID];
      /* Send the message to the LCD gate-keeper for display. */
      xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
      vTaskDelay( 100 / portTICK_RATE_MS );
      STM_SSDRelay_Off();
      vTaskDelay( 1500 / portTICK_RATE_MS );
    }
    else { // the code is invalid:
      STM_EVAL_LEDOff(LED4);
      STM_EVAL_LEDOn(LED3);
      xMessage.xMsgType = UNAME;
      xMessage.pcMessage = RFIDUserNames[userID];
      /* Send the message to the LCD gate-keeper for display. */
      xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
      STM_SSDRelay_Off();
      vTaskDelay( 2100 / portTICK_RATE_MS );
      STM_EVAL_LEDOff(LED3);
    }

    // wait before new read
    STM_EVAL_LEDOff(LED4);
    STM_EVAL_LEDOff(LED3);
    vTaskDelay( 1000 / portTICK_RATE_MS );
    nStartReadRFIDTransponder = SET;
    userID = NOTAGS+1;
    STM_SSDRelay_Off();

    // clear the name from LCD:
    xMessage.xMsgType = UNAME;
    xMessage.pcMessage = RFIDUserNames[userID];
    /* Send the message to the LCD gate-keeper for display. */
    xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

    vTaskResume( xLEDBlinkHandle );  /* resume LED blinking task */
  }

}


void vToggleLED4Task (void *pvParameters){

  /* The task is implemented in an infinite loop */
  while (1) {
    /*
    Delay for a period of time. vTaskDelay() places the task into
    the Blocked state until the period has expired.
    The delay period is spacified in 'ticks'. We can convert
    yhis in milisecond with the constant portTICK_RATE_MS.
    */
    STM_EVAL_LEDOn(LED4);
    vTaskDelay(70 / portTICK_RATE_MS);
    STM_EVAL_LEDOff(LED4);
    vTaskDelay(750 / portTICK_RATE_MS);
  }
}
/*-----------------------------------------------------------*/


inline void process_transponder(void) {

  /* RFID Transponder has been detected and read, so validate its code  */

  /* Disable further read into buffer */
  nStartReadRFIDTransponder = RESET;

  /* Transponder has sent the RFID code, so validate it  */
  userID = CheckRFIDTagCode();
  if (CheckRFIDTagCode() != ERROR ) {
    STM_EVAL_LEDOn(LED4);
    STM_SSDRelay_On();
    vTaskDelay( 100 / portTICK_RATE_MS );
    STM_SSDRelay_Off();
    vTaskDelay( 1500 / portTICK_RATE_MS );
  }
  else { // the code is invalid:
    STM_EVAL_LEDOff(LED4);
    STM_EVAL_LEDOn(LED3);
    STM_SSDRelay_Off();
    vTaskDelay( 2100 / portTICK_RATE_MS );
    STM_EVAL_LEDOff(LED3);
  }
}


/**
  * @brief  Checks RFID TAG against valid codes
  * @param  None
  * @retval ERROR (0) if RFID code invalid, else userID
  * @author  Patricio Bulic
  * @version V1.0.0
  * @date    14-September-2012
  */
uint32_t CheckRFIDTagCode(void)
{
  uint32_t status = ERROR;
  int i,j;

  for (j = 0; j < NOTAGS; j++) {
    if (status != ERROR) break;
    for (i = 0; i < 14; i++) {
      if (RFIDTagCode[i] != RFIDValidCodes[j*14 + i]) {
        status = ERROR;
        break;
      }
      else status = j+1;
    }
  }
  return status;
}





/**
  * @brief  This function handles USART3 interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
  uint16_t data;
  const uint16_t cCR = 0x000A;
  const uint16_t cLF = 0x000D;

  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  /*
  Here we read 125kHz RFID module connected to USART3.
  The RFID module continuously sends 14 bytes of ID code followed by LF/CR (0D/0A).
  We synchronize the read of the ID code with LF/CR i.e. we first wait for LF/CR sequence then we start
  to read the 14 bytes if code.
  */

  /* Is RXNE flag set? */
  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {

    /*
      Start read, only if nStartReadRFIDTransponder == SET i.e. the buffer is not already processed.
      The RFID processing task will set/reset this flag.
      If we are not allowed to read into buffer,  we just perform dummy read from USART3 Data Reg.
    */

    if (nStartReadRFIDTransponder == RESET) {
      USART_ReceiveData(USART3);    // DUMMY read
    }

    else { // process the card.....
      data = USART_ReceiveData(USART3);

      // 1. check whether the LF/CR has been received
      if (nLFReceived == SET) {
        if (nCRReceived == SET) {
          // LF/CR received. Start read into buffer...
          RFIDTagCode[nDataCount] = data;
          // if read all 14 bytes, set the flag to main program
          if (nDataCount < 13) {
            nDataCount ++;
          }
          else {
            nDataCount = 0;
            // "Give" the semaphore from ISR and unblock the task:
            xSemaphoreGiveFromISR(xRFIDSemaphore, &xHigherPriorityTaskWoken);
            // clear all flags...
            nLFReceived = RESET;
            nCRReceived = RESET;
          }
        }

        else { //nCRReceived == RESET
          if (data == cCR)  nCRReceived = SET;
          /* on the next interrupt we will start to read the RFID code into buffer, so
          clear the counter value
          */
          nDataCount = 0;
        }
      }
      else { //nLFReceived == RESET
        if (data == cLF)  nLFReceived = SET;
      }
    }
  }

  /* If writing to a queue or semaphore caused a task to unblock, and the unblocked task
    has a priority equal to or above the task that this interrupt interrupted,
    then xHigherPriorityTaskWoken will have been set to pdTRUE internally within
    xQueuesendFromISR() or xSemaphoreGiveFromISR(), and portEND_SWITCHING_ISR() will ensure that this
    interrupt returns directly to the higher priority unblocked task.

    THIS MUST BE THE LAST THING DONE IN THE ISR. */

  // Force a context switch. This will invoke the ready task, which has the highest priority
  portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}


void RFIDInitMinimal(){

  // Semaphore cannot be used before a call to vSemaphoreCreateBinary ().
  // This is a macro so pass the variable in directly.
  vSemaphoreCreateBinary( xRFIDSemaphore );

  /* If the queue/semaphore was created correctly then setup the serial port
    hardware. */
  if ( xRFIDSemaphore != NULL ) {
    nStartReadRFIDTransponder = RESET;
    nLFReceived = RESET;
    nCRReceived = RESET;

    STM_EVAL_PA8Init();

    USART3_Config();
    NVIC_Configuration();
  }
}



/**
  * @brief  Configures USART3 and maps Tx on PB10 and RX on PB11
  * @param  None
  * @retval None
  * @author  Patricio Bulic
  * @version V1.0.0
  * @date    7-March-2012
  */
void USART3_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  USART_ClockInitTypeDef USART_ClockInitStructure;

  /* GPIOB Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /********************************************************/
  /* Configure USART 3 */
  /* We will use USART 3 Rx at PB11, as PB11 is dedicated as Free I/O at the Discovery board */
  /* USART : 1. Enable Peripheral Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  /* USART : 2. Enable GPIO Clock according to USART Mode */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  /* USART : 3. Connect the IO pin to the desired AF */
  /* Transmitter should be used only for test purposes as PB10 is dedicated to MP45DT02 digital microphone */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
  /* Rx is mapped to PB11 that is Free IO pin at board */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

  /* USART : 4. Configure Tx pin (PB10) as AF PP, NOPULL, 50Mhz */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* USART : 5. Configure Rx pin (PB11) as AF IN, NO PULL (float type) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* USART : 6. Set the USART clock - standard setup */
  USART_ClockStructInit(&USART_ClockInitStructure);
  USART_ClockInit(USART3, &USART_ClockInitStructure);

  /* USART : 7. Select baud rate, word length, stop, parity */
  USART_InitStructure.USART_BaudRate = 9600 ;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b ;
  USART_InitStructure.USART_StopBits = USART_StopBits_1 ;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
  USART_Init(USART3, &USART_InitStructure);

  /* USART : 8. Enable USART */
  USART_Cmd(USART3, ENABLE);

  /* Enabling interrupt from USART3 */
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}



/**
  * @brief  Configures  NVIC module (enable interrupt from USART 3)
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enabling interrupt from USART3 */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configKERNEL_INTERRUPT_PRIORITY;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  Configures PA8 GPIO.
  * @ Pa3cio Bulic, 25.1.2013
  * @retval None
  */
void STM_EVAL_PA8Init(void)
{
  // it will be used to control SSD Relay
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


/**
  * @brief  Turns SSD Relay On.
  * @retval None
  */
void STM_SSDRelay_On(void)
{
  GPIOA->BSRRL = GPIO_Pin_8;
}

/**
  * @brief  Turns SSD Relay Off.
  * @retval None
  */
void STM_SSDRelay_Off(void)
{
  GPIOA->BSRRH = GPIO_Pin_8;
}








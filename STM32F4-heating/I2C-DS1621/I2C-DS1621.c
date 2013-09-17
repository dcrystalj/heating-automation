/*
 * I2C-DS1621.c
 *
 *  Created on: Mar 19, 2013
 *      Author: patricio
 */

#include "I2C-DS1621.h"
#include "timetemp.h"

int8_t Temperature[2];


extern temperature_structure_t xTemperature;
/* The queue used to send message to LCD gate-keeper task  */
extern xQueueHandle xLCDQueue;

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void I2C_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  /* Enable the I2C periph and GPIOB */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  I2C_InitStructure.I2C_Ack=I2C_Ack_Disable;
  //I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed=I2C_SPEED ;
  I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
  I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;

  I2C_Init(I2C1, &I2C_InitStructure);

  I2C_Cmd(I2C1,ENABLE);
}


void DS1621_WriteData (uint8_t data) {

  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  // Send I2C1 START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // wait for I2C1 EV5 --> Slave has acknowledged start condition
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  // Send slave Address:
  I2C_Send7bitAddress(I2C1, 0x90,  I2C_Direction_Transmitter);
  //wait for I2C1 EV6, check if Slave has acknowledged Master transmitter mode
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1, data);
  // wait for I2C1 EV8_2 --> byte has been transmitted
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  // Send I2C1 STOP Condition
  I2C_GenerateSTOP(I2C1, ENABLE);

}

void DS1621_SendCommand (uint8_t cmd, uint8_t direction) {

  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  // Send I2C1 START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // wait for I2C1 EV5 --> Slave has acknowledged start condition
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  // Send slave Address:
  I2C_Send7bitAddress(I2C1, 0x90,  direction);
  //wait for I2C1 EV6, check if Slave has acknowledged Master transmitter mode
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C1, cmd);
  // wait for I2C1 EV8_2 --> byte has been transmitted
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  // Send I2C1 STOP Condition
  I2C_GenerateSTOP(I2C1, ENABLE);
}





void DS1621_ReadTemp (int8_t ptemperature_data[]) {
  // Get temperature:

  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  // Send I2C1 START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // wait for I2C1 EV5 --> Slave has acknowledged start condition
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  // Send slave Address:
  //I2C_Send7bitAddress(I2C1, 0x90,  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
  I2C_Send7bitAddress(I2C1, 0x90,  I2C_Direction_Receiver);
  //wait for I2C1 EV6, check if Slave has acknowledged Master receiver mode
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  // enable acknowledge of recieved data:
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // wait until one byte has been received
  while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  // read data from I2C data register
  ptemperature_data[0] = I2C_ReceiveData(I2C1);


  // disabe acknowledge for the second byte
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  // wait until one byte has been received
  while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  // read data from I2C data register
  ptemperature_data[1] = I2C_ReceiveData(I2C1);


  // Send I2C1 STOP Condition
  I2C_GenerateSTOP(I2C1, ENABLE);
}


void DS1621_ReadData (int8_t* data) {
  // Get temperature:

  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  // Send I2C1 START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // wait for I2C1 EV5 --> Slave has acknowledged start condition
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

  // Send slave Address:
  //I2C_Send7bitAddress(I2C1, 0x90,  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
  I2C_Send7bitAddress(I2C1, 0x90,  I2C_Direction_Receiver);
  //wait for I2C1 EV6, check if Slave has acknowledged Master receiver mode
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

  // enable acknowledge of recieved data:
  //I2C_AcknowledgeConfig(I2C1, ENABLE);
  // wait until one byte has been received
  while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  // read data from I2C data register
  *data = I2C_ReceiveData(I2C1);

  // disabe acknowledge for the second byte
  I2C_AcknowledgeConfig(I2C1, DISABLE);


  // Send I2C1 STOP Condition
  I2C_GenerateSTOP(I2C1, ENABLE);
}


void vReadTempTask (void *pvParameters){
  xLCDMessage xMessage;
  int8_t xCountRemain;
  int8_t xCountPerC;

  /* Init I2C DS1621 Temperature Sensor: */
  I2C_Configuration();
  // DS1621 Start conversion command:
  DS1621_WriteData (DS1621_STARTCONV_CMD);

  /* The task is implemented in an infinite loop */
  while (1) {
    /*
    Delay for a period of time. vTaskDelay() places the task into
    the Blocked state until the period has expired.
    The delay period is spacified in 'ticks'. We can convert
    yhis in milisecond with the constant portTICK_RATE_MS.
    */
    vTaskDelay(300 / portTICK_RATE_MS);

    // DS1621 read temperature command:
    DS1621_WriteData (DS1621_READTEMP_CMD);
    // Receive 2 bytes from DS1621:
    DS1621_ReadTemp (&Temperature[0]);
    xTemperature.temperature = Temperature[0];
    xTemperature.temp_frac = Temperature[1];
    // DS1621 read counter command:
    DS1621_SendCommand (DS1621_READCNT_CMD, 0);
    // Receive data from DS1621::
    DS1621_ReadData (&xCountRemain);
    xTemperature.count_remain = xCountRemain;
    // DS1621 read slope command:
    DS1621_SendCommand (DS1621_READSLOPE_CMD, 0);
    // Receive data from DS1621::
    DS1621_ReadData (&xCountPerC);
    xTemperature.count_per_c = xCountPerC;

    //FormatTemperatureString(&xTemperature);
    FormatTemperatureStringHR(&xTemperature);


    xMessage.xMsgType = TEMP;
    xMessage.pcMessage = xTemperature.temperature_string;

    /* Send the message to the LCD gate-keeper for display. */
    xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );

  }
}
/*-----------------------------------------------------------*/






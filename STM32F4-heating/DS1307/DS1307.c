/*
 * I2C-DS1307.h
 *
 *  Created on: Aug 12, 2013
 *      Author: Crystal
 */

#include "timetemp.h"
#include "DS1307.h"
/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    :
* Input          : None
* Output         : None
* Return         : None
* Attention    : None
*******************************************************************************/
void I2C_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  /* Enable the I2C periph and GPIOB */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //SCK
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1); //SDA

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);


  //I2C_InitStructure.I2C_Ack=I2C_Ack_Disable;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed=I2C_SPEED ;
  I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
  I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;

  I2C_Init(I2C1, &I2C_InitStructure);

  I2C_Cmd(I2C1,ENABLE);
}


void I2C_start (uint8_t direction) {

  // wait until I2C1 is not busy anymore
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

  // Send I2C1 START condition
  I2C_GenerateSTART(I2C1, ENABLE);
  // wait for I2C1 EV5 --> Slave has acknowledged start condition
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

//  int8_t addr = 0x68;
  // Send slave Address:
  I2C_Send7bitAddress(I2C1, I2C_SLAVE_ADDRESS7,  direction);

//  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && addr<= 0x7F){
//	  I2C_GenerateSTOP(I2C1, ENABLE);
//	  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
//
//	    // Send I2C1 START condition
//	    I2C_GenerateSTART(I2C1, ENABLE);
//	    // wait for I2C1 EV5 --> Slave has acknowledged start condition
//	    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//
//	    // Send slave Address:
//	    I2C_Send7bitAddress(I2C1, 53,  direction);
//	    addr+=1;
//  }

  /* wait for I2C1 EV6, check if 
   * either Slave has acknowledged Master transmitter or
   * Master receiver mode, depending on the transmission
   * direction
   */ 
  if(direction == I2C_Direction_Transmitter){
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  }
  else if(direction == I2C_Direction_Receiver){
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  }
  
}

void I2C_write(uint8_t data){

  I2C_SendData(I2C1, data);
  // wait for I2C1 EV8_2 --> byte has been transmitted
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
}

uint8_t I2C_read_byte(void){
  // enable acknowledge of recieved data
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  // wait until one byte has been received
  while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  // read data from I2C data register and return data byte
  return I2C_ReceiveData(I2C1);

}

uint8_t I2C_read_last_byte(void){
  // disabe acknowledge of received data
  I2C_AcknowledgeConfig(I2C1, DISABLE);
  // wait until one byte has been received
  while( !I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) );
  // read data from I2C data register and return data byte
  uint8_t data = I2C_ReceiveData(I2C1);
  return data;
}

void I2C_stop(void){
  // Send I2C1 STOP Condition 
  I2C_GenerateSTOP(I2C1, ENABLE);
}

void I2C_reset_pointer(){
  //set to write
  I2C_start(I2C_Direction_Transmitter);

  //set pointer to first register 0x00
  I2C_write(0x00);

  //I2C_stop();
}
 

void DS1307_ReadTime () {
//	  I2C_reset_pointer();
//	  I2C_stop();
//	  I2C_start(I2C_Direction_Receiver);
//
//	  //read registers
//	  int i;
//	  for (i=0; i<7; i++){
//	    pTime_data[i] = I2C_read_byte();
//	  }
//	  pTime_data[i] = I2C_read_last_byte();
//
//	  I2C_stop();
}







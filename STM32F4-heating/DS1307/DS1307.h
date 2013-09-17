/*
 * I2C-DS1307.h
 *
 *  Created on: Aug 12, 2013
 *      Author: Crystal
 */

#ifndef I2C_DS1307_H_
#define I2C_DS1307_H_


/* Private define ------------------------------------------------------------*/
#define I2C_SPEED               100000
#define I2C_SLAVE_ADDRESS7      53


/* Exported functions ------------------------------------------------------- */
void I2C_init(void);
void I2C_start(uint8_t direction);
void I2C_write(uint8_t data);
uint8_t I2C_read_byte(void);
uint8_t I2C_read_last_byte(void);
void I2C_stop(void);
void I2C_reset_pointer();
void DS1307_ReadTime ();


#endif /* I2C_DS1307_H_ */


#ifndef I2C_H_
#define I2C_H_

#include <stdio.h>
#include "reg_adr.h"
#include "Kmx32.h"

//void i2c ();
uint8_t tmp;
void I2C_Config(void);
void I2C_Off(void);
//uint16_t I2C_Send(uint8_t data);
uint8_t I2C_Get(void);
void I2C_Strobe(uint8_t addr, uint8_t cntr, uint8_t byte[], uint16_t count);
void I2C_Byte(uint8_t addr, uint8_t cntr, uint8_t byte);
//void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack_Waiter(void);





#endif /* I2C_H_ */

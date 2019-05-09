/*
 * iic_Interface.h
 *
 *  Created on: May 8, 2019
 *      Author: Administrator
 */

#ifndef INC_IIC_INTERFACE_H_
#define INC_IIC_INTERFACE_H_

bool iic_Start(void);
bool iic_Write(uint8_t data);
void iic_Stop(void);
uint8_t iic_Read(bool rAck);

#endif /* INC_IIC_INTERFACE_H_ */

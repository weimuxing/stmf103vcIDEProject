/*
 * iic_Interface.h
 *
 *  Created on: May 8, 2019
 *      Author: Administrator
 */

#ifndef INC_IIC_INTERFACE_H_
#define INC_IIC_INTERFACE_H_

#define PIN_HIGH GPIO_PIN_SET
#define PIN_LOW  GPIO_PIN_RESET

#define IIC_PORT_A IIC_SCL_A_GPIO_Port,IIC_SDA_A_GPIO_Port,IIC_SCL_A_Pin,IIC_SDA_A_Pin
#define IIC_PORT_B IIC_SCL_B_GPIO_Port,IIC_SDA_B_GPIO_Port,IIC_SCL_B_Pin,IIC_SCL_B_Pin

#define IIC_SCL_SET(x) HAL_GPIO_WritePin(iicPortConfig[iicPortSel].SCLGPIOx,iicPortConfig[iicPortSel].SCLGPIO_Pin,x)
#define IIC_SDA_SET(x) HAL_GPIO_WritePin(iicPortConfig[iicPortSel].SDAGPIOx,iicPortConfig[iicPortSel].SDAGPIO_Pin,x)
#define IIC_SCL_GET    HAL_GPIO_ReadPin(iicPortConfig[iicPortSel].SCLGPIOx,iicPortConfig[iicPortSel].SCLGPIO_Pin)
#define ICI_SDA_GET	   HAL_GPIO_ReadPin(iicPortConfig[iicPortSel].SDAGPIOx,iicPortConfig[iicPortSel].SDAGPIO_Pin)
#define IIC_SCL_INPUT IIC_SCL_SET(PIN_HIGH)
#define IIC_SDA_INPUT IIC_SDA_SET(PIN_HIGH)

typedef enum
{
	I2C_TRANS_READ, I2C_TRANS_WRITE
} I2cIoTransType;

bool iic_Start(void);
bool iic_Write(uint8_t data);
void iic_Stop(void);
uint8_t iic_Read(uint32_t rAck);

#endif /* INC_IIC_INTERFACE_H_ */

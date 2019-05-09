/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define IIC_SCL_A_Pin GPIO_PIN_4
#define IIC_SCL_A_GPIO_Port GPIOC
#define IIC_SDA_A_Pin GPIO_PIN_5
#define IIC_SDA_A_GPIO_Port GPIOC
#define GPIOD_EXTI15_Pin GPIO_PIN_15
#define GPIOD_EXTI15_GPIO_Port GPIOD
#define IIC_SCL_B_Pin GPIO_PIN_8
#define IIC_SCL_B_GPIO_Port GPIOC
#define IIC_SDA_B_Pin GPIO_PIN_9
#define IIC_SDA_B_GPIO_Port GPIOC
#define USB_PWR_Pin GPIO_PIN_8
#define USB_PWR_GPIO_Port GPIOA
#define Status_LED_Pin GPIO_PIN_8
#define Status_LED_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef enum
{
	false = 0, true,
} bool;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

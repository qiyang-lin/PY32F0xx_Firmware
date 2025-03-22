/**
  ******************************************************************************
  * @file    main.h
  * @author  MCU Application Team
  * @brief   Header for main.c file.
  *          This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "py32f0xx_hal.h"
#include "py32f0xx_ll_usart.h"
#include <stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* 雷达唤醒引脚定义 */
#define RADAR_WAKEUP_PORT        	GPIOF
#define RADAR_WAKEUP_PIN         	GPIO_PIN_1
#define RADAR_WAKEUP_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
/* Exported variables prototypes ---------------------------------------------*/
extern UART_HandleTypeDef UartHandle;

/* Exported functions prototypes ---------------------------------------------*/
void APP_ErrorHandler(void);
void APP_Usart1IRQCallback(USART_TypeDef *USARTx);
void APP_Usart2IRQCallback(USART_TypeDef *USARTx);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

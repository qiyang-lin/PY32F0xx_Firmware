/**
  ******************************************************************************
  * @file    py32f0xx_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/**
  * @brief Initialize global MSP
  */
void HAL_MspInit(void)
{
  BSP_LED_Init(LED_GREEN);
  BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);

  HAL_NVIC_SetPriority(LPTIM1_IRQn, 0x01,  0);                     /* Set LPTIM interrupt priority */
  HAL_NVIC_EnableIRQ(LPTIM1_IRQn);                                 /* Enable LPTIM global interrupt */

}

/************************ (C) COPYRIGHT Puya *****END OF FILE****/

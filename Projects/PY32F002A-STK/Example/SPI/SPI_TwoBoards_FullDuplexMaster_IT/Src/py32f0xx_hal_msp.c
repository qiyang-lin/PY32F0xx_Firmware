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
}

/**
  * @brief Initialize global MSP
  */
 void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
 {
   GPIO_InitTypeDef  GPIO_InitStruct = {0};
 
    __HAL_RCC_GPIOB_CLK_ENABLE();                   /* Enable GPIOB clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();                   /* Enable GPIOA clock */
    __HAL_RCC_SPI1_CLK_ENABLE();                    /* Enable SPI1 clock */

    /*SCK*/
    GPIO_InitStruct.Pin       = GPIO_PIN_3;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    if (hspi->Init.CLKPolarity == SPI_POLARITY_LOW)
    {
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    }
    else
    {
      GPIO_InitStruct.Pull = GPIO_PULLUP;
    }
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*SPI NSS*/
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


    /*Configure GPIO as SPI: MISO/MOSI*/
    GPIO_InitStruct.Pin       = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    /*Interrupt configuration*/
    HAL_NVIC_SetPriority(SPI1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
 
 }
 
 void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    /*##-1- Reset peripherals ##################################################*/
    __HAL_RCC_SPI1_FORCE_RESET();
    __HAL_RCC_SPI1_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Deconfigure SPI SCK */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7 );
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3);

    HAL_NVIC_DisableIRQ(SPI1_IRQn);
  }
}

/************************ (C) COPYRIGHT Puya *****END OF FILE****/

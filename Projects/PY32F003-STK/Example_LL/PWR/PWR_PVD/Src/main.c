/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
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
#include "py32f003xx_ll_Start_Kit.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_ExtiConfig(void);
static void APP_PvdConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Configure system clock */
  APP_SystemClockConfig();

  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);

  /* Configure EXTI */
  APP_ExtiConfig();
  
  /* Configure PVD */
  APP_PvdConfig();
  
  /* Enable PVD */
  LL_PWR_EnablePVD();
  
  while (1)
  {
  }
}

/**
  * @brief  Configure system clock
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  /* Enable and initialize HSI */
  LL_RCC_HSI_Enable();
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
  while (LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* Set AHB frequency division */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* HSISYS used as SYSCLK clock source */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* Set APB1 divider */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(24000000);
  /* Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(24000000);
}

/**
  * @brief  Configure EXTI
  * @param  None
  * @retval None
  */
static void APP_ExtiConfig(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  /* Enable GPIOB clock */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  /* Select PB07 pin */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  /* Select analog mode */
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  /* Select pull-up */
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  /* Initialize GPIOB */
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Select EXTI16 */
  EXTI_InitStruct.Line = LL_EXTI_LINE_16;
  /* Enable */
  EXTI_InitStruct.LineCommand = ENABLE;
  /* Select interrupt mode */
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  /* Select rising/falling edge trigger */
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
  /* Initialize external interrupt */
  LL_EXTI_Init(&EXTI_InitStruct);
}

/**
  * @brief  Configure PVD
  * @param  None
  * @retval None
  */
static void APP_PvdConfig(void)
{
  /* Enable PWR clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  
  /* PB07 as the detection source, this parameter is invalid */
  LL_PWR_SetPVDLevel(LL_PWR_PVDLEVEL_0);
  
  /* Filter functionality disabled */
  LL_PWR_DisablePVDFilter();
  LL_PWR_SetPVDFilter(LL_PWR_PVD_FILTER_1CLOCK);
  
  /* PVD detection source is PB07 */
  LL_PWR_SetPVDSource(LL_PWR_PVD_SOURCE_PB7);

  /* Set interrupt priority */
  NVIC_SetPriority(PVD_IRQn, 1);
  /* Enable interrupt */
  NVIC_EnableIRQ(PVD_IRQn);
}

/**
  * @brief  PVD interrupt callback
  * @param  None
  * @retval None
  */
void APP_PvdCallback(void)
{
  if (LL_PWR_IsActiveFlag_PVDO())
  {
    BSP_LED_On(LED_GREEN);
  }
  else
  {
    BSP_LED_Off(LED_GREEN);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  /* infinite loop */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line)  */
  /* infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

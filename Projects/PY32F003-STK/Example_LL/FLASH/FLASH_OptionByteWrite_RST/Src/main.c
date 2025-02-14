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
/* #define OB_GPIO_PIN_MODE LL_FLASH_NRST_MODE_RESET */
#define OB_GPIO_PIN_MODE LL_FLASH_NRST_MODE_GPIO

/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

/**
  * @brief  应用程序入口函数.
  * @retval int
  */
int main(void)
{
  /* 时钟初始化,配置系统时钟为HSI */
  APP_SystemClockConfig();

  /* Enable SYSCFG and PWR clocks */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* 初始化LED */  
  BSP_LED_Init(LED_GREEN);
  
  /* 初始化按键 */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* 等待按键按下 */
  while (BSP_PB_GetState(BUTTON_USER))
  {
  }

  /* 判断RST管脚 */

  if(LL_FLASH_GetNrstMode(FLASH) != OB_GPIO_PIN_MODE)
  {
    /* 解锁 Flash */
    LL_FLASH_Unlock(FLASH);
  
    /* 解锁 Option */
    LL_FLASH_OBUnlock(FLASH);

    LL_FLASH_TIMMING_SEQUENCE_CONFIG_24M();

    /* 设置PF2为普通GPIO模式 */
    LL_FLASH_SetOPTR(FLASH,LL_FLASH_BOR_DISABLE,LL_FLASH_BOR_LEV0,LL_FLASH_NBOOT1_CLR,LL_FLASH_IWDG_MODE_SW, \
                           LL_FLASH_WWDG_MODE_SW, OB_GPIO_PIN_MODE,LL_FLASH_RDP_LEVEL_0);

    LL_FLASH_EnableOptionProgramStart(FLASH);
    LL_FLASH_TriggerOptionProgramStart(FLASH);
    
    while(LL_FLASH_IsActiveFlag_BUSY(FLASH)==1);
    
    /* 锁定 Option */
    LL_FLASH_OBLock(FLASH);
    
    /* 锁定 Flash */
    LL_FLASH_Lock(FLASH);

    /* 产生一个复位，option byte装载  */
    LL_FLASH_Launch(FLASH);
  }
  else
  {
    BSP_LED_On(LED_GREEN);
  }

  while (1)
  {
  }
}

/**
  * @brief  系统时钟配置函数
  * @param  无
  * @retval 无
  */
static void APP_SystemClockConfig(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  /* HSI使能及初始化 */
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
  LL_RCC_HSI_Enable();

  while (LL_RCC_HSI_IsReady() != 1)
  {
  }

  /* 设置AHB分频*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* 配置HSISYS为系统时钟及初始化 */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS)
  {
  }

  /*设置APB1分频及初始化*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(24000000);
  
  /* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
  LL_SetSystemCoreClock(24000000);
}

/**
  * @brief  错误执行函数
  * @param  无
  * @retval 无
  */
void APP_ErrorHandler(void)
{
  /* 无限循环 */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  输出产生断言错误的源文件名及行号
  * @param  file：源文件名指针
  * @param  line：发生断言错误的行号
  * @retval 无
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以根据需要添加自己的打印信息,
     例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

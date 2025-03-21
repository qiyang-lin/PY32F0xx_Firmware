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

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
COMP_HandleTypeDef hcomp1;
PWR_StopModeConfigTypeDef PwrStopModeConf = {0};

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_RccInit(void);
static void APP_CompInit(void);
static void APP_CompIt(void);
static void APP_LedRun(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick */
  HAL_Init();
  
  /* System clock configuration */
  APP_SystemClockConfig(); 

  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);

  /* Initialize button */
  BSP_PB_Init(BUTTON_KEY,BUTTON_MODE_GPIO);

  /* Initialize clock settings */
  APP_RccInit();

  /* Initialize COMP */
  APP_CompInit();
 
  /* Enable interrupts */
  APP_CompIt();
 
  /* Start COMP */
  HAL_COMP_Start(&hcomp1);
  
  BSP_LED_On(LED_GREEN);

  /* Wait for button press */
  while(BSP_PB_GetState(BUTTON_USER) != 0)
  {
  }
  BSP_LED_Off(LED_GREEN);

  /* Suspend SysTick interrupt */
  HAL_SuspendTick();

  /* VCORE = 1.0V  when enter stop mode */
  PwrStopModeConf.LPVoltSelection       =  PWR_STOPMOD_LPR_VOLT_SCALE2;
  PwrStopModeConf.FlashDelay            =  PWR_WAKEUP_FLASH_DELAY_5US;
  PwrStopModeConf.WakeUpHsiEnableTime   =  PWR_WAKEUP_HSIEN_AFTER_MR;
  PwrStopModeConf.RegulatorSwitchDelay  =  PWR_WAKEUP_LPR_TO_MR_DELAY_2US;
  PwrStopModeConf.SramRetentionVolt     =  PWR_SRAM_RETENTION_VOLT_VOS;
  HAL_PWR_ConfigStopMode(&PwrStopModeConf);

  /* Enter STOP mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);  

  /* Resume the SysTick interrupt */
  HAL_ResumeTick();

  HAL_Delay(1000);
  while(1)
  {
    APP_LedRun();
  }
}

/**
  * @brief  System clock configuration
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator Configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI; /* Selective Oscillator HSE,HSI,LSI */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI not divided */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;  /* Configure HSI output clock as 8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* Disable HSE */
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_24MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* Disable LSI */

  /* Initialize RCC oscillators */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock Source Configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCC system clock types */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; /* SYSCLK source is HSI */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB clock not divided */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB clock not divided */
  /* Initialize RCC system clock */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  Comparator clock configuration function
  * @param  None
  * @retval None
  */
static void APP_RccInit(void)
{                    
  RCC_OscInitTypeDef RCCCONF = {0};
  RCC_PeriphCLKInitTypeDef COMPRCC = {0};
  
  RCCCONF.OscillatorType = RCC_OSCILLATORTYPE_LSI;        /* RCC uses internal LSI */
  RCCCONF.LSIState = RCC_LSI_ON;                          /* Enable LSI */
  
  COMPRCC.PeriphClockSelection = RCC_PERIPHCLK_COMP1;     /* Peripheral selection: COMP1 */
  COMPRCC.Comp1ClockSelection = RCC_COMP1CLKSOURCE_LSC;   /* Independent clock source for COMP1: LSC */

  HAL_RCC_OscConfig(&RCCCONF);                            /* Initialize clock settings */
  HAL_RCCEx_PeriphCLKConfig(&COMPRCC);                    /* Initialize RCC peripheral clock settings */
}

/**
  * @brief  Comparator initialization function
  * @param  None
  * @retval None
  */
static void APP_CompInit(void)
{
  __HAL_RCC_COMP1_CLK_ENABLE();                              /* Enable COMP1 clock */
  hcomp1.Instance = COMP1;                                   /* COMP1 */
  hcomp1.Init.InputPlus = COMP_INPUT_PLUS_IO3;               /* Positive input: PA1 */
  hcomp1.Init.InputMinus = COMP_INPUT_MINUS_VREFINT;         /* Negative input: VREFINT */
  hcomp1.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED; 
  hcomp1.Init.Mode = COMP_POWERMODE_MEDIUMSPEED;             /* COMP1 power mode: Medium speed */
  hcomp1.Init.Hysteresis = COMP_HYSTERESIS_ENABLE;           /* Hysteresis function is enabled */
  hcomp1.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  hcomp1.Init.TriggerMode = COMP_TRIGGERMODE_IT_FALLING;     /* Trigger mode: Falling edge interrupt */
  hcomp1.Init.DigitalFilter = 0;                          
  
  HAL_COMP_Init(&hcomp1);
}

/**
  * @brief  Comparator interrupt enable function
  * @param  None
  * @retval None
  */
static void APP_CompIt(void)
{
  /*Enable COMP interrupt*/
  HAL_NVIC_EnableIRQ(ADC_COMP_IRQn);
  HAL_NVIC_SetPriority(ADC_COMP_IRQn, 0x01, 0);
}

/**
  * @brief  LED toggle function
  * @param  None
  * @retval None
  */
static void APP_LedRun(void)
{
  BSP_LED_Toggle(LED_GREEN);
  HAL_Delay(200);
  BSP_LED_Toggle(LED_GREEN);
  HAL_Delay(200);
}

/**
  * @brief  Comparator interrupt callback function
  * @param  hcomp：Comparator handle
  * @retval None
  */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
  BSP_LED_On(LED_GREEN);
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
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

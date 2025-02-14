/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  * @date
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
#define Vcc_Power     3.3l                                            /* VCC power supply voltage, modify according to actual situation */
#define TScal1        (float)((HAL_ADC_TSCAL1) * 3.3 / Vcc_Power)     /* Voltage corresponding to calibration value at 30 ℃*/
#define TScal2        (float)((HAL_ADC_TSCAL2) * 3.3 / Vcc_Power)     /* Voltage corresponding to calibration value at 85 ℃ */
#define TStem1        30l                                             /* 30 ℃*/
#define TStem2        85l                                             /* 85 ℃ */
#define Temp_k        ((float)(TStem2-TStem1)/(float)(TScal2-TScal1)) /* Temperature calculation */
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef             AdcHandle;
ADC_ChannelConfTypeDef        sConfig;
volatile int16_t              aADCxConvertedData;
int16_t                       aTEMPERATURE;
/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void APP_ADCConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Systick */
  HAL_Init();

  /* Initialize UART */
  BSP_USART_Config();

  /* Configure ADC */  
  APP_ADCConfig();

  /* infinite loop */
  while (1)
  {
    HAL_Delay(500);                                                  
    if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)                         /* Start ADC and enable ADC interrupts */
    {
      APP_ErrorHandler();
    }
  }
}

/**
  * @brief  ADC configuration function
  * @param  None
  * @retval None
  */
void APP_ADCConfig(void)
{
  __HAL_RCC_ADC_FORCE_RESET();
  __HAL_RCC_ADC_RELEASE_RESET();
  __HAL_RCC_ADC_CLK_ENABLE();                                                     /* Enable ADC clock */

  AdcHandle.Instance = ADC1;
  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)                          /* ADC calibration */
  {
    APP_ErrorHandler();
  }
  AdcHandle.Instance                   = ADC1;                                    /* ADC*/
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;                /* Set ADC clock*/
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;                      /* 12-bit resolution for converted data*/
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;                     /* Right-alignment for converted data */
  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;              /* Scan sequence direction: forward*/
  AdcHandle.Init.EOCSelection          = ADC_EOC_SEQ_CONV;                        /* Single sampling*/
  AdcHandle.Init.LowPowerAutoWait      = ENABLE;                                  /* Enable wait for conversion mode */
  AdcHandle.Init.ContinuousConvMode    = DISABLE;                                 /* Single conversion mode */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                                 /* Disable discontinuous mode */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;                      /* Software triggering */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;           /* No external trigger edge */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;                /* When an overload occurs, overwrite the previous value*/
  AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_239CYCLES_5;              /* Set sampling time */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)                                         /* ADC initialization*/
  {
    APP_ErrorHandler();
  }

  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;                                 /* Set whether to sample */
  sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;                                  /*  Setting the sampling channel */
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)                      /* Configuring ADC Channels */
  {
    APP_ErrorHandler();
  }

}

/**
  * @brief  ADC transfer complete callback function
  * @param  None
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  aADCxConvertedData = HAL_ADC_GetValue(hadc);
  aTEMPERATURE =(int16_t)(Temp_k * aADCxConvertedData - Temp_k * TScal1 + TStem1);
  printf("Temperature = %d \r\n", aTEMPERATURE);

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

/************************ (C) COPYRIGHT Puya *****END OF FILE****/

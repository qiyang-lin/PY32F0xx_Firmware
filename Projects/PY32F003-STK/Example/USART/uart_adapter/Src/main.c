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

#define RX_BUF_SIZE (256)

typedef enum {
	UART_STATE_IDLE,
	UART_STATE_BUSY_TX,
	UART_STATE_BUSY_RX,
	UART_STATE_RX_COMPLETE,
	UART_STATE_ERROR
} uart_state_t;

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef UartHandle;
uint8_t aRxBuffer[RX_BUF_SIZE];
uint8_t m_rx_cnt;
uart_state_t m_uart_state;

/* Private user code ---------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static void APP_SystemClockConfig(void);
static void APP_IntPinConfig(void);

/**
 * @brief  应用程序入口函数.
 * @retval int
 */
int main(void)
{
	/* systick初始化 */
	HAL_Init();

	/* 配置系统时钟为HSI */
	APP_SystemClockConfig();

	APP_IntPinConfig();

	/* USART初始化 */
	UartHandle.Instance = USART1;
	UartHandle.Init.BaudRate = 921600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits = UART_STOPBITS_1;
	UartHandle.Init.Parity = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode = UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&UartHandle) != HAL_OK) {
		APP_ErrorHandler();
	}

	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_IDLE(USART1);

	for (;;) {
		if (m_uart_state == UART_STATE_RX_COMPLETE) {
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_Delay(5U);
			(void)HAL_UART_Transmit(&UartHandle, (uint8_t *)aRxBuffer, m_rx_cnt, 0xFFFFU);
			HAL_Delay(5U);
			HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
			m_rx_cnt = 0;
			m_uart_state = UART_STATE_IDLE;
		}
	}
}

/**
 * @brief  USART错误回调执行函数
 * @param  huart：USART句柄
 * @retval 无
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	printf("Uart Error, ErrorCode = %d\r\n", huart->ErrorCode);
}

/**
 * @brief   系统时钟配置函数
 * @param   无
 * @retval  无
 */
static void APP_SystemClockConfig(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/* 配置时钟源HSE/HSI/LSE/LSI */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* 开启HSI */
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* 不分频 */
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz; /* 配置HSI输出时钟为24MHz */
	RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* 关闭HSE */
	/* RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz; */               /* HSE工作频率范围16M~32M */
	RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* 关闭LSI */

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) /* 初始化RCC振荡器 */
	{
		APP_ErrorHandler();
	}

	/* 初始化CPU,AHB,APB总线时钟 */
	RCC_ClkInitStruct.ClockType =
	        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* RCC系统时钟类型 */
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;                   /* SYSCLK的源选择为HSI */
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;                       /* APH时钟不分频 */
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;                        /* APB时钟不分频 */

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) !=
	    HAL_OK) /* 初始化RCC系统时钟(FLASH_LATENCY_0=24M以下;FLASH_LATENCY_1=48M) */
	{
		APP_ErrorHandler();
	}
}

static void APP_IntPinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOF_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, GPIO_PIN_RESET);
}

/**
 * @brief  USART中断处理函数
 * @param  USARTx：USART模块，可以是USART1、USART2
 * @retval 无
 */
void APP_UsartIRQCallback(USART_TypeDef *USARTx)
{
	if ((LL_USART_IsActiveFlag_RXNE(USARTx) != RESET) && (LL_USART_IsEnabledIT_RXNE(USARTx) != RESET)) {
		aRxBuffer[m_rx_cnt] = LL_USART_ReceiveData8(USARTx);
		m_rx_cnt++;
		m_uart_state = UART_STATE_BUSY_RX;
	}
	if ((LL_USART_IsActiveFlag_IDLE(USARTx) != RESET) && (LL_USART_IsEnabledIT_IDLE(USARTx) != RESET)) {
		LL_USART_ClearFlag_IDLE(USARTx);
		m_uart_state = UART_STATE_RX_COMPLETE;
	}
}

/**
 * @brief  错误执行函数
 * @param  无
 * @retval 无
 */
void APP_ErrorHandler(void)
{
	/* 无限循环 */
	while (1) {}
}

#ifdef USE_FULL_ASSERT
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
	while (1) {}
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/

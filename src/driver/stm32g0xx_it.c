/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
#include "usart1.h"
#include "usart2.h"
#include "application.h"
/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
	 app_timer_callback();
	main_timer_callback();
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/*
	DMA CHANNEL

	UART1 RX ----> LL_DMA_CHANNEL_4
	UART1 TX ----> LL_DMA_CHANNEL_3
	UART2 RX ----> LL_DMA_CHANNEL_1
	UART2 TX ----> LL_DMA_CHANNEL_2
	
*/


/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
	if(LL_DMA_IsActiveFlag_TC1(DMA1))
	{
		uart2_rx_dma_complete_irq();

		LL_DMA_ClearFlag_TC1(DMA1);
	}	

	if(LL_DMA_IsActiveFlag_HT1(DMA1))
	{
		uart2_rx_dma_half_irq();

		LL_DMA_ClearFlag_HT1(DMA1);
	}
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
	/* Channel2 */
	if(LL_DMA_IsActiveFlag_TC2(DMA1))
	{
		uart2_tx_dma_complete_irq();

		LL_DMA_ClearFlag_TC2(DMA1);
	}

	/* Channel3 */
	if(LL_DMA_IsActiveFlag_TC3(DMA1))
	{
		// uart1_tx_dma_complete_irq();

		LL_DMA_ClearFlag_TC3(DMA1);
	}

}

/**
  * @brief This function handles DMA1 channel 4, channel 5 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_5_DMAMUX1_OVR_IRQHandler(void)
{
	/* Channel4 */
	if(LL_DMA_IsActiveFlag_TC4(DMA1))
	{
		// uart1_rx_dma_complete_irq();

		LL_DMA_ClearFlag_TC4(DMA1);
	}

	if(LL_DMA_IsActiveFlag_HT4(DMA1))
	{
		// uart1_rx_dma_half_irq();

		LL_DMA_ClearFlag_HT4(DMA1);
	}
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{

	if(LL_USART_IsActiveFlag_IDLE(USART1))
	{

		// uart1_rx_idle_irq();

		LL_USART_ClearFlag_IDLE(USART1);
	}
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
	if(LL_USART_IsActiveFlag_IDLE(USART2))
	{
		uart2_rx_idle_irq();

		LL_USART_ClearFlag_IDLE(USART2);
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

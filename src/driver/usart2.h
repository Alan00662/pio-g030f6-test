/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define UART2_RX_FIFO_BUF_LEN	256

#define UART2_TX_DMA_BUF_LEN	64
#define UART2_RX_DMA_BUF_LEN	64

#define UART2_RX_DMA_CHANNEL	LL_DMA_CHANNEL_1
#define UART2_TX_DMA_CHANNEL	LL_DMA_CHANNEL_2


void uart2_init(void);

int uart2_write(uint8_t * buf, int len);
int uart2_read(uint8_t * buf, int len);
int uart2_read_buf_len(void);
uint8_t uart2_read_byte(void);

void uart2_rx_dma_half_irq(void);
void uart2_rx_dma_complete_irq(void);
void uart2_tx_dma_complete_irq(void);
void uart2_rx_idle_irq(void);

void uart2_send_one_byte(uint8_t Data);
void uart2_send_buf(uint8_t *buf,uint8_t len) ;
void debug_tx2(char * fmt, ...);	
void uart2_send_string(char *str);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

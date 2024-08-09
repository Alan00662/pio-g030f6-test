/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#ifndef __USART4_H__
#define __USART4_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define UART4_TX_DMA_BUF_LEN	128
#define UART4_RX_DMA_BUF_LEN	256

extern char usart4RxBuff[UART4_RX_DMA_BUF_LEN];
extern uint8_t usart4TxBuff[UART4_TX_DMA_BUF_LEN] ;
/* USER CODE END Private defines */


void uart4_init(void);

/* USER CODE BEGIN Prototypes */
void tx4_dma_send(void);
void uart4_rx_handler(void);
void uart4_send_one_byte(uint8_t Data);
void uart4_send_buf(uint8_t *buf,uint8_t len) ;
void uart4_send_string(char *str);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

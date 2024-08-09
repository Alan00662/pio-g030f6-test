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
#ifndef __USART1_H__
#define __USART1_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#define UART1_TX_DMA_BUF_LEN	128
#define UART1_RX_DMA_BUF_LEN	80

#define RX1_SENSOR_BUF_SIZE   50

typedef struct crsf_sensor_s
{
  uint8_t buf[RX1_SENSOR_BUF_SIZE];
  uint8_t pack_size;
  uint8_t id;

} __attribute__((packed)) crsf_sensor_t;

extern crsf_sensor_t rx_sensor;

extern char usart1RxBuff[UART1_RX_DMA_BUF_LEN];
extern uint8_t usart1TxBuff[UART1_TX_DMA_BUF_LEN] ;
/* USER CODE END Private defines */


void uart1_init(void);

/* USER CODE BEGIN Prototypes */
void tx1_dma_send(void);
void uart1_rx_handler(void);
void uart1_send_one_byte(uint8_t Data);
void uart1_send_buf(uint8_t *buf,uint8_t len) ;
/* USER CODE END Prototypes */


#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

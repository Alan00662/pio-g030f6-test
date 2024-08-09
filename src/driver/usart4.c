/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart4.h"
#include "config.h"
#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 1 */
char usart4RxBuff[UART4_RX_DMA_BUF_LEN] = {0};
uint8_t usart4TxBuff[UART4_TX_DMA_BUF_LEN] = {0x11, 0x22, 0x33, 0x44, 0x55};
/* USER CODE END 0 */

/* USART4 init function */

static void tx4_dma_config(void)
{
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)&USART4->TDR);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_3, (uint32_t)usart4TxBuff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 0);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_USART_EnableDMAReq_TX(USART4);
}

// LL库的DMA USART接收配置
static void rx4_dma_config(void)
{

    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)&USART4->RDR);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)usart4RxBuff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, UART4_RX_DMA_BUF_LEN);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_4);
    LL_USART_EnableDMAReq_RX(USART4);
}

void uart4_init(void)
{

    /* USER CODE BEGIN USART4_Init 0 */

    /* USER CODE END USART4_Init 0 */

    LL_USART_InitTypeDef USART_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART4);

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    /**USART4 GPIO Configuration
    PA0   ------> USART4_TX
    PA1   ------> USART4_RX
    */

    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART4 DMA Init */

    /* USART4_RX Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_4, LL_DMAMUX_REQ_USART4_RX);

    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_VERYHIGH);

    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_CIRCULAR);

    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT);

    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT);

    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);

    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MDATAALIGN_BYTE);

    /* USART4_TX Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMAMUX_REQ_USART4_TX);

    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_HIGH);

    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);

    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);

    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);

    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);

    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_BYTE);

    /* USART4 interrupt Init */
    NVIC_SetPriority(USART3_4_IRQn, 0);
    NVIC_EnableIRQ(USART3_4_IRQn);

    /* USER CODE BEGIN USART4_Init 1 */

    /* USER CODE END USART4_Init 1 */
    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
    USART_InitStruct.BaudRate = USART4_BAUDRATE;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;

    LL_USART_Init(USART4, &USART_InitStruct);
    LL_USART_SetTXFIFOThreshold(USART4, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_SetRXFIFOThreshold(USART4, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_DisableFIFO(USART4);
    LL_USART_ConfigAsyncMode(USART4);

    /* USER CODE BEGIN WKUPType USART4 */

    /* USER CODE END WKUPType USART4 */

    LL_USART_Enable(USART4);

    /* Polling USART4 initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(USART4))) || (!(LL_USART_IsActiveFlag_REACK(USART4))))
    {
    }
    /* USER CODE BEGIN USART4_Init 2 */
    tx4_dma_config();
    rx4_dma_config();
    /* USER CODE END USART4_Init 2 */
}

void tx4_dma_send(void)
{
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, 5);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

char *wellhandled_string = NULL;
char USART_RX_BUF[UART4_RX_DMA_BUF_LEN]; // 接收缓冲

void uart4_rx_handler(void) //50ms
{


}

void uart4_send_one_byte(uint8_t Data)
{
    while (LL_USART_IsActiveFlag_TC(USART4) != 1)
        ; // wait;
    LL_USART_TransmitData8(USART4, Data);
}

void uart4_send_buf(uint8_t *buf, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        uart4_send_one_byte(*(buf + i));
    }
}

void uart4_send_string(char *str)
{
    uint8_t i;
    for (i = 0; i < strlen(str); i++)
    {
        uart4_send_one_byte(*(str + i));
    }
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

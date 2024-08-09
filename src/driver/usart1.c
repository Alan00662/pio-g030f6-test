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
#include "usart1.h"
#include "usart2.h"
#include "config.h"
<<<<<<< HEAD
=======
#include "gps.h"
>>>>>>> 8db0e3e6a8b8e581f900a2eb1dc2e7793f54450c
#include "crsf_protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
<<<<<<< HEAD
=======
#include "stdarg.h"
>>>>>>> 8db0e3e6a8b8e581f900a2eb1dc2e7793f54450c
/* USER CODE BEGIN 0 */
/* USER CODE BEGIN 1 */
char usart1RxBuff[UART1_RX_DMA_BUF_LEN] = {0};
uint8_t usart1TxBuff[UART1_TX_DMA_BUF_LEN] = {0x11, 0x22, 0x33, 0x44, 0x55};
/* USER CODE END 0 */

#if UART1_USE_SBUS

void uart1_init(void)
{
    LL_USART_InitTypeDef USART_InitStruct = {0};
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    fifo_init(&uart1_rx_fifo, uart1_rx_fifo_buf, UART1_RX_FIFO_BUF_LEN);

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    /**USART1 GPIO Configuration
    PB7   ------> USART1_RX
    PB6   ------> USART1_TX
    */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 DMA Init */

    /* USART1_TX Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_5, LL_DMAMUX_REQ_USART1_TX);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_NORMAL);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_SetPeriphAddress(DMA1, UART1_TX_DMA_CHANNEL, (uint32_t)(&USART1->TDR));
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);

    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

    /* USART1_RX Init */
    /*LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_6, LL_DMAMUX_REQ_USART1_RX);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_6, (uint32_t)uart1_rx_dma_buf);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, UART1_RX_DMA_BUF_LEN);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PDATAALIGN_BYTE);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MDATAALIGN_BYTE);
    LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_6);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_6);
    LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_6);**/

    /* USART1 interrupt Init */
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);

    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
    USART_InitStruct.BaudRate = USART1_BAUDRATE;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_9B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_2;
    USART_InitStruct.Parity = LL_USART_PARITY_ODD;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &USART_InitStruct);
    LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_DisableFIFO(USART1);
    LL_USART_SetRXPinLevel(USART1, LL_USART_RXPIN_LEVEL_INVERTED);
    LL_USART_SetTXPinLevel(USART1, LL_USART_TXPIN_LEVEL_INVERTED);
    LL_USART_ConfigAsyncMode(USART1);

    LL_USART_EnableDMAReq_RX(USART1);
    LL_USART_EnableDMAReq_TX(USART1);
    LL_USART_Enable(USART1);

    /* Polling USART1 initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
    {
    }

    // LL_USART_EnableIT_IDLE(USART1);
}
int uart1_write(uint8_t *buf, int len)
{
    if (len > UART1_TX_DMA_BUF_LEN)
    {
        return -1;
    }

    while (1 == LL_DMA_IsEnabledChannel(DMA1, UART1_TX_DMA_CHANNEL))
    {
        // return -1;
    }

    memcpy(uart1_tx_dma_buf, buf, len);

    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)uart1_tx_dma_buf);
    LL_DMA_SetDataLength(DMA1, UART1_TX_DMA_CHANNEL, len);
    LL_DMA_EnableChannel(DMA1, UART1_TX_DMA_CHANNEL);

    return 0;
}

int uart1_read(uint8_t *buf, int len)
{
    return fifo_read(&uart1_rx_fifo, buf, len);
}

int uart1_read_buf_len(void)
{
    return fifo_get_len(&uart1_rx_fifo);
}

void uart1_clear_fifo(void)
{
    fifo_clean(&uart1_rx_fifo);
}

#endif

#if 1

/* USART1 init function */

static void tx1_dma_config(void)
{
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)&USART1->TDR);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)usart1TxBuff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, 0);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
    LL_USART_EnableDMAReq_TX(USART1);
}

// LL库的DMA USART接收配置
static void rx1_dma_config(void)
{

    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_6, (uint32_t)&USART1->RDR);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_6, (uint32_t)usart1RxBuff);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, UART1_RX_DMA_BUF_LEN);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
    LL_USART_EnableDMAReq_RX(USART1);
}

void uart1_init(void)
{

    /* USER CODE BEGIN USART1_Init 0 */

    /* USER CODE END USART1_Init 0 */

    LL_USART_InitTypeDef USART_InitStruct = {0};

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    /**USART1 GPIO Configuration
    PA9   ------> USART1_TX
    PA10   ------> USART1_RX
    */

    GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */

    /* USART1_RX Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_6, LL_DMAMUX_REQ_USART1_RX);

    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PRIORITY_VERYHIGH);

    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MODE_CIRCULAR);

    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PERIPH_NOINCREMENT);

    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MEMORY_INCREMENT);

    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_PDATAALIGN_BYTE);

    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_6, LL_DMA_MDATAALIGN_BYTE);

    /* USART1_TX Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_5, LL_DMAMUX_REQ_USART1_TX);

    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_HIGH);

    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_NORMAL);

    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT);

    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT);

    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);

    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MDATAALIGN_BYTE);

    /* USART1 interrupt Init */
    NVIC_SetPriority(USART1_IRQn, 0);
    NVIC_EnableIRQ(USART1_IRQn);

    /* USER CODE BEGIN USART1_Init 1 */

    /* USER CODE END USART1_Init 1 */
    USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
    USART_InitStruct.BaudRate = USART1_BAUDRATE;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init(USART1, &USART_InitStruct);
    LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_DisableFIFO(USART1);
    LL_USART_ConfigAsyncMode(USART1);

    /* USER CODE BEGIN WKUPType USART1 */

    /* USER CODE END WKUPType USART1 */

    LL_USART_Enable(USART1);

    /* Polling USART1 initialisation */
    while ((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
    {
    }
    /* USER CODE BEGIN USART1_Init 2 */
    tx1_dma_config();
    rx1_dma_config();
    /* USER CODE END USART1_Init 2 */
}

void tx1_dma_send(void)
{
    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, 5);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
}

crsf_sensor_t rx_sensor;
void uart1_rx_handler(void)
{
    uint16_t i = 0;
    uint8_t j = 0;
    for (i = 0; i < UART1_RX_DMA_BUF_LEN; i++) //
    {
        if (usart1RxBuff[i] == CRSF_ADDRESS_CRSF_RECEIVER) // 如果是CRSF数据帧
        {
            rx_sensor.pack_size = (usart1RxBuff[i + 1] + 2); // 获取包长度= buf[2]+2
            rx_sensor.id = usart1RxBuff[i + 2];              // 获取ID

            for (j = 0; j < rx_sensor.pack_size; j++) // 获取完整数据包
            {
                rx_sensor.buf[j] = usart1RxBuff[i + j];
            }
            // uart2_send_buf(rx_sensor.buf,rx_sensor.pack_size);
        }
    }
}

#endif

void uart1_send_one_byte(uint8_t Data)
{
    while (LL_USART_IsActiveFlag_TC(USART1) != 1)
        ; // wait;
    LL_USART_TransmitData8(USART1, Data);
}

void uart1_send_buf(uint8_t *buf, uint8_t len)
{
    uint8_t i;
    for (i = 0; i < len; i++)
    {
        uart1_send_one_byte(*(buf + i));
    }
}

<<<<<<< HEAD
=======
void debug_tx1(char * fmt, ...)
{
	int len;
	char buf[200];

	va_list	arguments;
		
	va_start(arguments, fmt);
	len = vsnprintf(buf, sizeof(buf) - 1, fmt, arguments);
	va_end(arguments);
	uart1_send_buf((uint8_t *)buf,len);
}

>>>>>>> 8db0e3e6a8b8e581f900a2eb1dc2e7793f54450c
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

#include "usart2.h"
#include "fifo.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

fifo_t uart2_rx_fifo;


uint8_t uart2_rx_fifo_buf[UART2_RX_FIFO_BUF_LEN];

uint8_t uart2_rx_dma_buf[UART2_RX_DMA_BUF_LEN];
uint8_t uart2_tx_dma_buf[UART2_TX_DMA_BUF_LEN];


uint32_t uart2_dma_rx_head=0;

void uart2_init(void)
{
	LL_USART_InitTypeDef USART_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	fifo_init(&uart2_rx_fifo, uart2_rx_fifo_buf, UART2_RX_FIFO_BUF_LEN);
	

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	/**USART2 GPIO Configuration
	PA2   ------> USART2_TX
	PA3   ------> USART2_RX
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART2 DMA Init */

	/* USART2_TX Init */
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_USART2_TX);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_2, (uint32_t)(&USART2->TDR));
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_NORMAL);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);

	/* USART2_RX Init */
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_1, LL_DMAMUX_REQ_USART2_RX);
	LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)uart2_rx_dma_buf);
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_1, UART2_RX_DMA_BUF_LEN);
	LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_1, (uint32_t)(&USART2->RDR));
	LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_LOW);
	LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR);
	LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_BYTE);
	LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_1);
	LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_1);

	/* USART2 interrupt Init */
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);

	NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);

	NVIC_SetPriority(DMA1_Channel1_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = 420000;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART2, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART2);

#if CONFIG_CRSF_HALFDUAL
	LL_USART_EnableHalfDuplex(USART2);
#endif

#if CONFIG_CRSF_TX_INVERT
	LL_USART_SetTXPinLevel(USART2, LL_USART_TXPIN_LEVEL_INVERTED);
#else
	LL_USART_SetTXPinLevel(USART2, LL_USART_TXPIN_LEVEL_STANDARD);
#endif

#if CONFIG_CRSF_RX_INVERT
		LL_USART_SetRXPinLevel(USART2, LL_USART_RXPIN_LEVEL_INVERTED);	
#else
		LL_USART_SetRXPinLevel(USART2, LL_USART_RXPIN_LEVEL_STANDARD);	
#endif


	LL_USART_EnableDMAReq_RX(USART2);
	LL_USART_EnableDMAReq_TX(USART2);
	LL_USART_Enable(USART2);
	
	/* Polling USART2 initialisation */
	while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
	{
	}

	LL_DMA_EnableChannel(DMA1,UART2_RX_DMA_CHANNEL);
	LL_USART_EnableIT_IDLE(USART2);

}



//
int uart2_write(uint8_t * buf, int len)
{
	if(len > UART2_TX_DMA_BUF_LEN)
	{
		return -1;
	}

	while(1==LL_DMA_IsEnabledChannel(DMA1, UART2_TX_DMA_CHANNEL))
	{
		//return -1;
	}

	memcpy(uart2_tx_dma_buf,buf,len);
	LL_DMA_SetDataLength(DMA1, UART2_TX_DMA_CHANNEL, len);
	LL_DMA_EnableChannel(DMA1, UART2_TX_DMA_CHANNEL);

    return 0;
}

void uart2_send_one_byte(uint8_t Data)
{
		while(LL_USART_IsActiveFlag_TC(USART2)!=1);//wait;
		LL_USART_TransmitData8(USART2,Data);
}

void uart2_send_buf(uint8_t *buf,uint8_t len) 
{
	uint8_t i;
	for(i = 0;i < len;i++)
	{
			uart2_send_one_byte(*(buf+i));
	}
}


int uart2_read(uint8_t * buf, int len)
{
	return fifo_read(&uart2_rx_fifo,buf,len);
}

int uart2_read_buf_len(void)
{
	return fifo_get_len(&uart2_rx_fifo);
}

uint8_t uart2_read_byte(void)
{
	uint8_t data[1];
	fifo_read(&uart2_rx_fifo,data,1);
	return data[0];
}



void uart2_rx_dma_half_irq(void)
{	
	/*
	 * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	 * |				  half					|
	 * |	 head_ptr	tail_ptr				|
	 * |		 |			|					|
	 * |		 v			v					|
	 * | --------*******************----------- |
	 * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
	 */
	uint32_t tail;
	uint32_t len;

	tail = UART2_RX_DMA_BUF_LEN / 2;
	len = tail - uart2_dma_rx_head;
	fifo_write(&uart2_rx_fifo,uart2_rx_dma_buf + uart2_dma_rx_head,len);
	uart2_dma_rx_head = tail;
}

void uart2_rx_dma_complete_irq(void)
{
    /*
     * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
     * |                  half                  |
     * |                    | head_ptr tail_ptr |
     * |                    |    |            | |
     * |                    v    v            v |
     * | ------------------------************** |
     * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
     */
	uint32_t tail;
	uint32_t len;

	tail = UART2_RX_DMA_BUF_LEN;
	len = tail - uart2_dma_rx_head;
	fifo_write(&uart2_rx_fifo,uart2_rx_dma_buf + uart2_dma_rx_head,len);
	uart2_dma_rx_head = 0;
}

void uart2_rx_idle_irq(void)
{
	uint32_t tail;
	uint32_t len;
    /*
     * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
     * |     head_ptr          tail_ptr         |
     * |         |                 |            |
     * |         v                 v            |
     * | --------*******************----------- |
     * +~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+
     */

	tail = UART2_RX_DMA_BUF_LEN - LL_DMA_GetDataLength(DMA1,UART2_RX_DMA_CHANNEL);
	len = tail - uart2_dma_rx_head;
	fifo_write(&uart2_rx_fifo,uart2_rx_dma_buf + uart2_dma_rx_head,len);
	uart2_dma_rx_head = tail;
}

void uart2_tx_dma_complete_irq(void)
{
	LL_DMA_DisableChannel(DMA1, UART2_TX_DMA_CHANNEL);
}


void debug_tx2(char * fmt, ...)
{
	int len;
	char buf[200];

	va_list	arguments;
		
	va_start(arguments, fmt);
	len = vsnprintf(buf, sizeof(buf) - 1, fmt, arguments);
	va_end(arguments);
	uart2_send_buf((uint8_t *)buf,len);
}

void uart2_send_string(char *str)
{
    uint8_t i;
    for (i = 0; i < strlen(str); i++)
    {
        uart2_send_one_byte(*(str + i));
    }
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

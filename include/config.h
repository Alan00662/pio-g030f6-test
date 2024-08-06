#ifndef __CONFIG_H__
#define __CONFIG_H__


#define UART1_USE_LOG 0
#define USE_GPS 1
#define USE_BAT 0

#if UART1_USE_LOG
#define USART1_BAUDRATE	115200
#else
#define USART1_BAUDRATE	420000

#endif


#if USE_GPS
#define USART4_BAUDRATE	115200
// #define USART4_BAUDRATE	38400
#endif

#define CRSF_RX_TX_TIMEOUT	20

/*
    Watch Dog
*/
#define IWDG_ENABLE 0

#define LIST_SIZE 20



#endif


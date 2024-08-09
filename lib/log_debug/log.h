#ifndef LOG_H
#define LOG_H

#include "stdint.h"
#include "stdio.h"
#include "stdarg.h"
#include "driver/usart1.h"
#include "main.h"

void debug(char * fmt, ...);
#if UART1_USE_LOG

void debug_hex(char * head, uint8_t* buf, int len);

#define LOG_DEBUG(fmt, ...)  do{  \
        debug("[%6d.%03d][%s:%d]"fmt"\r\n",(millis()/1000),(millis()%1000), __func__, __LINE__, ##__VA_ARGS__); \
    }while(0)

#define LOG_HEX(head,buf,len) do{   \
        debug_hex(head,buf,len); \
    }while(0)

#else /*UART1_USE_LOG*/
#define LOG_DEBUG(fmt, ...)
#define LOG_HEX(head,buf,len) 	
#endif/*UART1_USE_LOG*/

#endif
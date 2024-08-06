#include "log.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "driver/usart1.h"

void debug(char * fmt, ...)
{
	int len;
	char buf[200];

	va_list	arguments;
		
	va_start(arguments, fmt);
	len = vsnprintf(buf, sizeof(buf) - 1, fmt, arguments);
	va_end(arguments);
	uart1_send_buf((uint8_t *)buf,len);
}

#if UART1_USE_LOG

void debug(char * fmt, ...)
{
	int len;
	char buf[200];

	va_list	arguments;
		
	va_start(arguments, fmt);
	len = vsnprintf(buf, sizeof(buf) - 1, fmt, arguments);
	va_end(arguments);
	uart1_write((uint8_t *)buf,len);
}

void debug_hex(char * head, uint8_t* buf, int len)
{
	char temp_buf[200];
	char * p = temp_buf;
	int i;
	int head_len=0;
    char out_len=0;

	if(head!=NULL)
	{
		head_len = strlen(head);
		memcpy(temp_buf,head,head_len);
		p += head_len;
	}
	
	for(i=0;i<len;i++)
	{
		sprintf(p,"%02X ",buf[i]);
		p+=3;
	}

	sprintf(p,"\r\n");
    
    out_len = head_len + (len*3) +2;

	uart1_write((uint8_t *)temp_buf, out_len);
}



#endif
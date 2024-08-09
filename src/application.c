
#include "driver/usart1.h"
#include "driver/usart4.h"
#include "main.h"

#include "crsf.h"

#include "config.h"

#include "application.h"


uint32_t system_tick = 0;


void setup(void)
{
    uart4_init();  //CRSF,接遥控器
    uart1_init();  //接飞控
	crc8_init();

}

void main_loop(void)
{

    CRSF_handle_uart_in(); 
	uart1_rx_handler();	
}

/**
 * @brief call by systick interrupu per one milli second
 * 
 */
void app_timer_callback(void)
{
	static uint16_t count = 0;

	count++;
	if(count==500)
	{
	}
	if(count==1000)
	{

		count = 0;
	}
	
}







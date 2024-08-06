#include "led.h"
#include "driver/usart1.h"
#include "driver/usart2.h"
#include "driver/usart4.h"
#include "main.h"
#include "log.h"
#include "crsf.h"
#include "sbus.h"
#include "config.h"
#include "gps.h"
#include "debugio.h"
#include "application.h"
#include "sensor.h"
#include "flash.h"
#include "key.h"


void setup(void)
{

    uart2_init();  //CRSF
	crc8_init();

	uart1_init();  

#ifdef USE_GPS    	
	GPS_RES_Init();
	uart4_init();
#endif

    sensor_list_init();

}

void main_loop(void)
{

    // GPS_Message_All();
    CRSF_handle_uart_in();

    Read_Input_Sensors_Loop();//500hz

    Read_Sensors_Loop(); /* read  sensor data .40hz 主要GPS串口波特率是38400还是115200*/

    Send_Sensors_Loop(); /* send sensor data to CRSF .50Hz*/
	Add_Sensor_List_Loop(); /* add sensor data to list .10Hz*/
	// Remove_Sensor_List_Loop(); /* remove sensor data from list .10Hz*/
    Key_Scan_Loop(); /* key scan .10Hz*/
    Led_Status_Loop(); /* led status .5Hz*/
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
		count = 0;
		// if(CRSF_get_state() == CRSF_DISCONNECT)
		// {
		// 	// led1_off();
        //         debug_tx2("GPS_massage=%d",GPS_massage);
		// }
	}
	
}







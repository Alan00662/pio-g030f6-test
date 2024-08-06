#ifndef CRSF_H
#define CRSF_H


#include "main.h"
#include "crsf_protocol.h"

#define CRSF_CONNETED   1
#define CRSF_DISCONNECT 0

#define BAT_LEN   0x0A

#define BAT_SIZE   BAT_LEN+2

extern uint16_t crsf_channels[CRSF_NUM_CHANNELS];
	

void crc8_init(void);
uint8_t crc8_calc(const uint8_t *data, uint8_t len);
void  CRSF_handle_uart_in(void);
uint8_t CRSF_get_state(void);
void send_battery_val(const uint8_t bat);
#endif

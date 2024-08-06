#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "linklist.h"
#include "crsf_protocol.h"
#include "driver/usart1.h"

extern list_t Sensor_list;
extern list_item_t Sensor_item;
extern uint8_t Sensor_Amount;

void sensor_list_init(void);
void print_sensor_list(void);

void Read_Sensors_Loop(void);
void Send_Sensors_Loop(void);

void Add_Sensor_List_Loop(void);
void Remove_Sensor_List_Loop(void);

void Read_Input_Sensors_Loop(void);

#endif
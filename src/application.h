#ifndef __APPLICATION_H
#define __APPLICATION_H

#include "linklist.h"

extern list_t Sensor_list;
extern list_item_t Sensor_item;

void setup(void);
void main_loop(void);
void app_timer_callback(void);


#endif


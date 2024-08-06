#ifndef SBUS_H
#define SBUS_H

#include "stdint.h"

extern uint8_t sbus_frame[25];

void sbus_build_packet(uint16_t * channels, uint8_t connect);

#endif

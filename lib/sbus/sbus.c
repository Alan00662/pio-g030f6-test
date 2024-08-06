#include "sbus.h"
#include "string.h"


uint8_t sbus_frame[25];

void sbus_build_packet(uint16_t * channels, uint8_t connect)
{
	int i=0;
	uint16_t value=0;
	uint8_t byteindex = 1;
	uint8_t offset = 0;
	memset(sbus_frame,0,25);
	sbus_frame[0]=0x0f;
	sbus_frame[24]=0x00;

    if(connect) {
		sbus_frame[23] = 0x00;
	}
	else {
		sbus_frame[23] = 0x0C;
	}
        
	for (i = 0; i < 16; ++i)
	{
		value = channels[i] ;//(unsigned short)(((channels[i] - SBUS_SCALE_OFFSET) / SBUS_SCALE_FACTOR) + .5f);
		if (value > 0x07ff)
		{
			value = 0x07ff;
		}

		while (offset >= 8)
		{
			++byteindex;
			offset -= 8;
		}
		
		sbus_frame[byteindex] |= (value << (offset)) & 0xff;
		sbus_frame[byteindex + 1] |= (value >> (8 - offset)) & 0xff;
		sbus_frame[byteindex + 2] |= (value >> (16 - offset)) & 0xff;
		offset += 11;
	}
}
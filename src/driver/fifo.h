#ifndef __FIFO_H
#define __FIFO_H

#include "stdint.h"

typedef struct _fifo_t
{
	uint8_t * buf;
	uint32_t size;
	uint32_t len;
	uint32_t write;
	uint32_t read;
} fifo_t;

void fifo_init(fifo_t * fifo,uint8_t * buf, int len);
int fifo_write(fifo_t * fifo,uint8_t * buf, int len);
int fifo_read(fifo_t * fifo, uint8_t * buf, int len);
int fifo_get_len(fifo_t * fifo);
void fifo_clean(fifo_t * fifo);
#endif


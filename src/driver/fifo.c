#include "fifo.h"
#include "string.h"

void fifo_init(fifo_t * fifo,uint8_t * buf, int len)
{
	fifo->buf = buf;
	fifo->size = len;
	fifo->len = 0;
	fifo->write = 0;
	fifo->read = 0;
}

int fifo_write(fifo_t * fifo, uint8_t * buf, int len)
{
	uint32_t free_size;
	uint32_t i=0;

	free_size = fifo->size - fifo->len;

	if(len > free_size)
	{
		return 0;
	}

	for(i=0;i<len;i++)
	{
		fifo->buf[fifo->write] = buf[i];
		fifo->len++;
		fifo->write++;
		if(fifo->write > fifo->size - 1)
		{
			fifo->write = 0;
		}
	}
	
	return len;
}

int fifo_read(fifo_t * fifo, uint8_t * buf, int len)
{
	uint32_t read_len;
	int i;

	if(len > fifo->len)
	{
		read_len = fifo->len;
	}
	else
	{
		read_len = len;
	}

	for(i=0;i<read_len;i++)
	{
		buf[i] = fifo->buf[fifo->read];
		fifo->len--;
		fifo->read++;
		if(fifo->read > fifo->size - 1)
		{
			fifo->read = 0;
		}
	}

	return read_len;
}

int fifo_get_len(fifo_t * fifo)
{
	return fifo->len;
}

void fifo_clean(fifo_t * fifo)
{
	memset(fifo->buf,0,fifo->size);
	fifo->len = 0;
	fifo->read = 0;
	fifo->write = 0;
}

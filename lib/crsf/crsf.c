#include "crsf.h"
#include "driver/usart2.h"
#include "main.h"
#include "crsf_protocol.h"
#include "log.h"
#include "string.h"

/* CRC START*************************/
#define CRSF_CRC_POLY 0xd5
#define crclen 256
uint8_t crcpoly;
uint8_t crc8tab[crclen];

void crc8_init(void)
{
    uint8_t crc;

    for (uint16_t i = 0; i < crclen; i++)
    {
        crc = i;
        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc << 1) ^ ((crc & 0x80) ? CRSF_CRC_POLY : 0);
        }
        crc8tab[i] = crc & 0xFF;
    }
}

uint8_t crc8_calc(const uint8_t *data, uint8_t len)
{
    uint8_t crc = 0;
    while (len--)
    {
        crc = crc8tab[crc ^ *data++];
    }
    return crc;
}

/* CRC END ***************************************************************************/

static uint8_t _rx_buf[64 + 3];
static uint8_t _rx_buf_pos = 0;
uint16_t crsf_channels[CRSF_NUM_CHANNELS];
crsfLinkStatistics_t _linkStatistics;

uint8_t battery_buf[BAT_SIZE];

uint8_t CRSF_state = 0;
uint32_t CRSF_last_recv_time = 0;

void CRSF_handle_byte_received();
void process_packet_in(uint8_t len);
void packetChannelsPacked(const crsf_header_t *p);
void packetLinkStatistics(const crsf_header_t *p);

// Shift the bytes in the RxBuf down by cnt bytes
void shift_rx_buffer(uint8_t cnt)
{
    // If removing the whole thing, just set pos to 0
    if (cnt >= _rx_buf_pos)
    {
        _rx_buf_pos = 0;
        return;
    }

    // Otherwise do the slow shift down
    uint8_t *src = &_rx_buf[cnt];
    uint8_t *dst = &_rx_buf[0];
    _rx_buf_pos -= cnt;
    uint8_t left = _rx_buf_pos;
    while (left--)
        *dst++ = *src++;
}

void CRSF_handle_uart_in(void)
{
    while (uart2_read_buf_len())
    {
        _rx_buf[_rx_buf_pos] = uart2_read_byte();
        _rx_buf_pos++;
        CRSF_handle_byte_received();
    }

    uint32_t now = millis();

    if (now > CRSF_last_recv_time && (now - CRSF_last_recv_time) > 1000)
    {
        CRSF_state = CRSF_DISCONNECT;
    }
}

void CRSF_handle_byte_received()
{
    uint8_t reprocess;
    do
    {
        reprocess = 0;
        if (_rx_buf_pos > 1)
        {
            uint8_t len = _rx_buf[1];
            // Sanity check the declared length, can't be shorter than Type, X, CRC
            if (len < 3 || len > CRSF_MAX_PACKET_LEN)
            {
                shift_rx_buffer(1);
                reprocess = 1;
            }
            else if (_rx_buf_pos >= (len + 2))
            {
                uint8_t inCrc = _rx_buf[2 + len - 1];
                uint8_t crc = crc8_calc(&_rx_buf[2], len - 1);
                if (crc == inCrc)
                {
                    LOG_DEBUG("[%d]", len + 2);
                    LOG_HEX("", _rx_buf, len + 2);

                    process_packet_in(len);
                    shift_rx_buffer(len + 2);
                    reprocess = 1;
                }
                else
                {
                    shift_rx_buffer(1);
                    reprocess = 1;
                }
            } // if complete packet
        } // if pos > 1
    } while (reprocess);
}

void process_packet_in(uint8_t len)
{
    const crsf_header_t *hdr = (crsf_header_t *)_rx_buf;
    if (hdr->device_addr == CRSF_ADDRESS_FLIGHT_CONTROLLER)
    {
        switch (hdr->type)
        {
        case CRSF_FRAMETYPE_RC_CHANNELS_PACKED:
            packetChannelsPacked(hdr);
            break;
        case CRSF_FRAMETYPE_LINK_STATISTICS:
            packetLinkStatistics(hdr);
            break;
            //		case CRSF_FRAMETYPE_BATTERY_SENSOR:
            //			send_battery_val(hdr);
            //        break;
        }

    } // CRSF_ADDRESS_FLIGHT_CONTROLLER
}

void packetChannelsPacked(const crsf_header_t *p)
{
    crsf_channels_t *ch = (crsf_channels_t *)&p->data;
    crsf_channels[0] = ch->ch0;
    crsf_channels[1] = ch->ch1;
    crsf_channels[2] = ch->ch2;
    crsf_channels[3] = ch->ch3;
    crsf_channels[4] = ch->ch4;
    crsf_channels[5] = ch->ch5;
    crsf_channels[6] = ch->ch6;
    crsf_channels[7] = ch->ch7;
    crsf_channels[8] = ch->ch8;
    crsf_channels[9] = ch->ch9;
    crsf_channels[10] = ch->ch10;
    crsf_channels[11] = ch->ch11;
    crsf_channels[12] = ch->ch12;
    crsf_channels[13] = ch->ch13;
    crsf_channels[14] = ch->ch14;
    crsf_channels[15] = ch->ch15;

    CRSF_last_recv_time = millis();
    CRSF_state = CRSF_CONNETED;
}

void packetLinkStatistics(const crsf_header_t *p)
{
    uint8_t test_num = 3;
    crsfLinkStatistics_t *link = (crsfLinkStatistics_t *)p->data;
    link->uplink_TX_Power = test_num;
    memcpy(&_linkStatistics, link, sizeof(_linkStatistics));
}

uint8_t CRSF_get_state(void)
{
    return CRSF_state;
}

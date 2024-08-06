#ifndef GPS_H
#define GPS_H

#include "driver/usart4.h"

#define GPS_PRS_Pin LL_GPIO_PIN_5
#define GPS_RES_Port GPIOA

#define  PAYLOAD_SIZE  15
#define  GPS_LEN  PAYLOAD_SIZE+2 //17
#define  GPS_CRC_SIZE GPS_LEN-1 //16
#define  GPS_SIZE  GPS_LEN+2 //19

#define false 0
#define true 1

//定义数组长度
#define GPS_Buffer_Length 70
#define latitude_Length 11
#define longitude_Length 11
#define heading_Length 15

#define Ground_Buffer_Length 34
#define groundspeed_Length 6

#define ALT_Buffer_Length 72
#define alt_Length 6
#define stars_Length 3

extern uint8_t CloseGGA[16];
extern uint8_t CloseGLL[16];
extern uint8_t CloseGSA[16];
extern uint8_t CloseGSV[16];
extern uint8_t CloseMRC[16];

/**
 * Define the shape of a standard header
 */
typedef struct crsf_gps_s
{
 	int32_t latitude; // degree / 10,000,000 big endian
	int32_t longitude; // degree / 10,000,000 big endian
	uint16_t groundspeed; // km/h / 10 big endian
	uint16_t heading; // GPS heading, degree/100 big endian
	uint16_t altitude; // meters, +1000m big endian
	uint8_t	satellites; // satellites
} __attribute__((packed)) crsf_gps_t;

extern crsf_gps_t SendGPS;

#define BUF_SIZE 12
typedef struct _GNRMC
{
    char utc_time[BUF_SIZE];
    char status;
    char latitude_value[BUF_SIZE];
    char latitude;
    char longtitude_value[BUF_SIZE];
    char longtitude;
    char speed[BUF_SIZE];
    char heading[BUF_SIZE];
}__attribute__((packed)) GNRMC_t;

typedef struct _GNVTG
{
    char heading[BUF_SIZE];
    char T;
	char M;
    char speed_N[BUF_SIZE];
    char N;
    char speed_K[BUF_SIZE];
}__attribute__((packed)) GNVTG_t;

typedef struct _GNGGA
{
    char utc_time[BUF_SIZE];
    char latitude_buf[BUF_SIZE];
    char NS;
    char longtitude_buf[BUF_SIZE];
    char EW;
	char ID;
	char stars_buf[stars_Length]; //卫星数量
	char level_buf[BUF_SIZE];
	char alt_buf[alt_Length]; //高度
}__attribute__((packed)) GNGGA_t;

enum 
{
    GPS_Message_ALL,
    GPS_Message_VTG,

};

extern uint8_t GPS_massage;

extern uint8_t gps_data_buf[GPS_SIZE];
extern char *GPS_Buffer;
extern char *Ground_Buffer;
extern char *ALT_Buffer;

void GPS_RES_Init(void);
void gps_res_set(void);
void gps_res_reset(void);

void ParseGPS(void);

void crsf_pack_gps_data(crsf_gps_t GPSdata);
void GPS_Message_Only_VTG(void);
void GPS_Message_All(void);
#endif


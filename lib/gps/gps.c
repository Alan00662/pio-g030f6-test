#include "gps.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "driver/usart2.h"
#include "crsf.h"
#include "crsf_protocol.h"

#if USE_GPS
#define DEBUG_GPS 0

#if 1
uint8_t CloseGGA[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x23};
uint8_t CloseGLL[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2A};
uint8_t CloseGSA[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x31};
uint8_t CloseGSV[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x38};
uint8_t CloseMRC[16] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3F};
#else

uint8_t CloseGGA[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x00,  0xFA, 0x0F};
uint8_t CloseGLL[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x00,  0xFB, 0x11};
uint8_t CloseGSA[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x00,  0xFC, 0x13};
uint8_t CloseGSV[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x00,  0xFD, 0x15};
uint8_t CloseMRC[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x00,  0xFE, 0x17};
#endif

uint8_t OpenGGA[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x00, 0x01, 0xFB, 0x10};
uint8_t OpenGLL[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x01, 0x01, 0xFC, 0x12};
uint8_t OpenGSA[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x02, 0x01, 0xFD, 0x14};
uint8_t OpenGSV[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x03, 0x01, 0xFE, 0x16};
uint8_t OpenMRC[11] = {0xB5, 0x62, 0x06, 0x01, 0x03, 0x00, 0xF0, 0x04, 0x01, 0xFF, 0x18};

uint8_t SaveCfg[21] = {0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x21, 0xAF};

uint8_t gps_data_buf[GPS_SIZE] = {0};
crsf_gps_t SendGPS;
uint8_t GPS_massage = 0;

char *GPS_Buffer = NULL;
char *ALT_Buffer = NULL;
char *Ground_Buffer = NULL;

#if DEBUG_GPS
char GPS_Buffer1[GPS_Buffer_Length + 1] = "$GNRMC,14.01,A,2237.56240,N,11401.59614,E,1.329,2.11,020916,,,A,V*37";
char Ground_Buffer1[Ground_Buffer_Length + 1] = "$GNVTG,21.11,T,,M,1.329,N,2.462,K,A*1B";
char ALT_Buffer1[ALT_Buffer_Length + 1] = "$GNGGA,073114.00,2237.56240,N,11401.59614,E,1,2,0.78,11.9,M,-2.5,M,,*54";
#endif

GNRMC_t gnrmc;
GNVTG_t gnvtg;
GNGGA_t gngga;

void GPS_RES_Init(void)
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

    /*LED1*/
    GPIO_InitStruct.Pin = GPS_PRS_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPS_RES_Port, &GPIO_InitStruct);

    gps_res_set(); // Reset GPS_PRS to high level
                   // gps_res_reset(); // Reset GPS_PRS to high level
}

void gps_res_set(void)
{
    LL_GPIO_SetOutputPin(GPS_RES_Port, GPS_PRS_Pin);
}

void gps_res_reset(void)
{
    LL_GPIO_ResetOutputPin(GPS_RES_Port, GPS_PRS_Pin);
}

static void parseGpsBuffer(void)
{

    float latitude_f = 0.0;
    float longitude_f = 0.0;
    float heading_f = 0.0;
    char *pEnd;

    uint8_t ret = 0;
#if DEBUG_GPS
    ret = sscanf(GPS_Buffer1, "$GNRMC,%[^,],%c,%[^,],%c,%[^,],%c,%[^,],%[^,]",
                 gnrmc.utc_time, &(gnrmc.status), gnrmc.latitude_value, &(gnrmc.latitude),
                 gnrmc.longtitude_value, &gnrmc.longtitude, gnrmc.speed, gnrmc.heading);

    // debug("ret=%d\n",ret);
    // debug(gnrmc.latitude_value);
#else
    ret = sscanf(GPS_Buffer, "$GNRMC,%[^,],%c,%[^,],%c,%[^,],%c,%[^,],%[^,]",
                 gnrmc.utc_time, &(gnrmc.status), gnrmc.latitude_value, &(gnrmc.latitude),
                 gnrmc.longtitude_value, &gnrmc.longtitude, gnrmc.speed, gnrmc.heading);
#endif

    latitude_f = strtof(gnrmc.latitude_value, &pEnd); // 字符转浮点
    SendGPS.latitude = ((int)(latitude_f / 100) + (latitude_f - (int)latitude_f / 100 * 100) / 60) * 10000000;

    longitude_f = strtof(gnrmc.longtitude_value, &pEnd);
    SendGPS.longitude = ((int)(longitude_f / 100) + (longitude_f - (int)longitude_f / 100 * 100) / 60) * 10000000;

    heading_f = strtof(gnrmc.heading, &pEnd);

    SendGPS.heading = (heading_f * 100); // 一旦大于32768就会溢出
    // debug("heading=%d\n",SendGPS.heading);
}

//$GNVTG
static void parseGroundBuffer(void)
{
    float groundspeed_f = 0.0;
    char *pEnd;

    uint8_t ret = 0;
#if DEBUG_GPS
    ret = sscanf(Ground_Buffer1, "$GNVTG,%[^,],%c,,%c,%[^,],%c,%[^,]",
                 gnvtg.heading, &gnvtg.T, &gnvtg.M, gnvtg.speed_N, &gnvtg.N, gnvtg.speed_K);

    // debug("ret=%d\n",ret); //6
    // debug(gnvtg.speed_K);
#else
    ret = sscanf(Ground_Buffer, "$GNVTG,%[^,],%c,,%c,%[^,],%c,%[^,]",
                 gnvtg.heading, &gnvtg.T, &gnvtg.M, gnvtg.speed_N, &gnvtg.N, gnvtg.speed_K);
#endif

    groundspeed_f = strtof(gnvtg.speed_K, &pEnd);
    SendGPS.groundspeed = groundspeed_f * 10;
}

static void parseAltBuffer(void)
{
    float alt_f = 0.0;
    float sta_f = 0.0;
    char *pEnd;

    uint8_t ret = 0;
#if DEBUG_GPS
    ret = sscanf(ALT_Buffer1, "$GNGGA,%[^,],%[^,],%c,%[^,],%c,%c,%[^,],%[^,],%[^,]", // 结构变量类型统一用char,float用数组，在这里用%[^,]
                 gngga.utc_time, gngga.latitude_buf, &gngga.NS, gngga.longtitude_buf,
                 &gngga.EW, &gngga.ID, gngga.stars_buf, gngga.level_buf, gngga.alt_buf);

    // debug("ret=%d\n",ret);
    // debug(gngga.alt_buf);
#else
    ret = sscanf(ALT_Buffer, "$GNGGA,%[^,],%[^,],%c,%[^,],%c,%c,%[^,],%[^,],%[^,]",
                 gngga.utc_time, gngga.latitude_buf, &gngga.NS, gngga.longtitude_buf,
                 &gngga.EW, &gngga.ID, gngga.stars_buf, gngga.level_buf, gngga.alt_buf);
#endif

    sta_f = strtof(gngga.stars_buf, &pEnd);
    SendGPS.satellites = sta_f;
    alt_f = strtof(gngga.alt_buf, &pEnd);
    SendGPS.altitude = alt_f + 1000;
}

void ParseGPS(void)
{
    parseGpsBuffer();
    parseGroundBuffer();
    parseAltBuffer();
}

void crsf_pack_gps_data(crsf_gps_t GPSdata)
{
    uint8_t i = 0;
    uint8_t crc_data[GPS_CRC_SIZE];
    gps_data_buf[0] = CRSF_ADDRESS_CRSF_RECEIVER;
    gps_data_buf[1] = GPS_LEN;
    gps_data_buf[2] = CRSF_FRAMETYPE_GPS;

    gps_data_buf[3] = (GPSdata.latitude >> 24); // latitude
    gps_data_buf[4] = (GPSdata.latitude >> 16);
    gps_data_buf[5] = (GPSdata.latitude >> 8);
    gps_data_buf[6] = GPSdata.latitude & 0xFF;

    gps_data_buf[7] = (GPSdata.longitude >> 24); // longitude
    gps_data_buf[8] = (GPSdata.longitude >> 16);
    gps_data_buf[9] = (GPSdata.longitude >> 8);
    gps_data_buf[10] = GPSdata.longitude & 0xFF;

    gps_data_buf[11] = (GPSdata.groundspeed >> 8); // ground speed
    gps_data_buf[12] = GPSdata.groundspeed & 0xFF;

    gps_data_buf[13] = (GPSdata.heading >> 8); // heading
    gps_data_buf[14] = GPSdata.heading & 0xFF;

    gps_data_buf[15] = (GPSdata.altitude >> 8); // altitude
    gps_data_buf[16] = GPSdata.altitude & 0xFF;

    gps_data_buf[17] = GPSdata.satellites; // satellites

    for (i = 0; i < GPS_CRC_SIZE; i++)
    {
        crc_data[i] = gps_data_buf[i + 2]; // form buf[2] to end
    }
    gps_data_buf[18] = crc8_calc(crc_data, GPS_CRC_SIZE);
}

void GPS_Message_Only_VTG(void)
{

    uart4_send_buf(CloseGGA, sizeof(CloseGGA));
    LL_mDelay(5);
    uart4_send_buf(CloseGLL, sizeof(CloseGLL));
    LL_mDelay(5);
    uart4_send_buf(CloseGSA, sizeof(CloseGSA));
    LL_mDelay(5);
    uart4_send_buf(CloseGSV, sizeof(CloseGSV));
    LL_mDelay(5);
    uart4_send_buf(CloseMRC, sizeof(CloseMRC));
    LL_mDelay(5);
    uart4_send_buf(SaveCfg, sizeof(SaveCfg));
}

void GPS_Message_All(void)
{
    uart4_send_buf(OpenGGA, sizeof(OpenGGA));
    LL_mDelay(5);
    uart4_send_buf(OpenGLL, sizeof(OpenGLL));
    LL_mDelay(5);
    uart4_send_buf(OpenGSA, sizeof(OpenGSA));
    LL_mDelay(5);
    uart4_send_buf(OpenGSV, sizeof(OpenGSV));
    LL_mDelay(5);
    uart4_send_buf(OpenMRC, sizeof(OpenMRC));
    LL_mDelay(5);
    uart4_send_buf(SaveCfg, sizeof(SaveCfg));
}

#endif
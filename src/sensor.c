#include "sensor.h"
#include "gps.h"
#include "driver/usart2.h"
#include <stdlib.h>
#include "string.h"

list_t Sensor_list = {0};
list_item_t Sensor_item = {0};
list_item_t Sensor1_item = {0};
list_item_t new_item = {0};
uint8_t add_sensor_flg = 0;
uint8_t Sensor_Amount = 0;

uint8_t bat_data_buf[12] = {0xEC, 0x0A, 0x08, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88};

static void clear_list_item(list_item_t *item)
{
    item->previous_item = NULL;
    item->next_item = NULL;
    item->sensor_id = 0;
    item->buf_len = 0;
    item->sensor_buf = NULL;
    item->exit_cnt = 0;
}

void sensor_list_init(void)
{
    /*初始化链表和结点   */
    list_init(&Sensor_list, LIST_SIZE);
    list_item_init(&Sensor_list, &Sensor_item);
    Sensor_Amount = 1;
#if USE_GPS
    Sensor_item.sensor_id = CRSF_FRAMETYPE_GPS; // GPS
    Sensor_item.buf_len = GPS_SIZE;
    Sensor_item.sensor_buf = gps_data_buf;
    Sensor_item.exit_cnt = 100;
    list_add_to_end(&Sensor_list, &Sensor_item);
#endif
#if USE_BAT
    list_item_init(&Sensor_list, &Sensor1_item);
    Sensor1_item.sensor_id = CRSF_FRAMETYPE_BATTERY_SENSOR; // 电池
    Sensor1_item.buf_len = 12;
    Sensor1_item.sensor_buf = bat_data_buf;
    Sensor1_item.exit_cnt = 100;
    list_add_to_end(&Sensor_list, &Sensor1_item);
#endif
}

uint32_t send_sensor_tick = 0;
uint8_t send_next_tick = 0;
void Send_Sensors_Loop(void)
{
    if (millis() - send_sensor_tick > 20)
    { 
        if (!list_is_empty(&Sensor_list))
        {
            list_item_t *current_item = list_first_item(&Sensor_list); // 遍历链表
            do
            {
                uart2_send_buf(current_item->sensor_buf, current_item->buf_len);
                current_item = list_next_item(&Sensor_list, current_item);
            } while (current_item != list_first_item(&Sensor_list));
        }
        send_sensor_tick = millis();
    }
}

/*把新传感器加入到链表*/
uint32_t add_sensor_tick = 0;
void Add_Sensor_List_Loop(void)
{
    if (millis() - add_sensor_tick > 50)
    {

        if (!list_is_empty(&Sensor_list))
        {
            list_item_t *current_item = list_last_item(&Sensor_list);

            if (current_item->sensor_id != rx_sensor.id && rx_sensor.id != 0) // 如果没有这个传感器，则添加到链表
            {
                add_sensor_flg = 1;
            }
        }

        if (add_sensor_flg)
        {
            add_sensor_flg = 0;

            clear_list_item(&new_item);
            list_item_init(&Sensor_list, &new_item);
            new_item.sensor_id = rx_sensor.id;
            new_item.buf_len = rx_sensor.pack_size;
            new_item.sensor_buf = rx_sensor.buf;
            new_item.exit_cnt = 100;
            list_add_to_end(&Sensor_list, &new_item);

            Sensor_Amount++;
        }

        add_sensor_tick = millis();
    }
}

/*删除链表中的传感器*/
uint32_t remove_sensor_tick = 0;
void Remove_Sensor_List_Loop(void)
{
    if (millis() - remove_sensor_tick > 100)
    {
        if (!list_is_empty(&Sensor_list))
        {
            list_item_t *current_item = list_first_item(&Sensor_list);
            do
            { // 遍历链表
                if (current_item->exit_cnt == 0)
                {
                    list_remove_item(&Sensor_list, current_item);
                }
                current_item = list_next_item(&Sensor_list, current_item);
            } while (current_item != list_first_item(&Sensor_list));
        }
        // item->exit_cnt--;

        // Sensor_Amount--;
        remove_sensor_tick = millis();
    }
}

uint32_t read_sensor_tick = 0;
void Read_Sensors_Loop(void)
{
    if (millis() - read_sensor_tick > 25) // 40Hz
    {

#ifdef USE_GPS
        uart4_rx_handler();          // 获取GPS原始数据
        ParseGPS();                  // 解析GPS数据
        crsf_pack_gps_data(SendGPS); // 打包GPS数据
#endif

#ifdef USE_BAT
        /*获取电池电压*/

#endif
        read_sensor_tick = millis();
    }
}

uint32_t read_input_sensor_tick = 0;
void Read_Input_Sensors_Loop(void)
{
    if (millis() - read_input_sensor_tick > CRSF_RX_TX_TIMEOUT)
    {
        uart1_rx_handler();

        read_input_sensor_tick = millis();
    }
}
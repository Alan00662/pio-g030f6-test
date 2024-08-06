/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "key.h"
#include "led.h"
#include "driver/usart2.h"
#include "gps.h"
#include "flash.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
save_data_t save_data;
/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void KEY_GPIO_Init(void)
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */

    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);

    /*LED1*/
    GPIO_InitStruct.Pin = KEY_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    LL_GPIO_Init(KEY_GPIO_Port, &GPIO_InitStruct);
}

uint32_t key_scan_tick = 0;
void Key_Scan_Loop(void)
{
    static uint8_t key_cnt = 0;
    if (millis() - key_scan_tick > 100)
    {
        if (LL_GPIO_IsInputPinSet(KEY_GPIO_Port, KEY_Pin) == 0)
        { /** 按键按下 */
            if (key_cnt < 5)
            {
                key_cnt++; /** 扫描计数 */
                // debug_tx2("key_cnt:%d\r\n",key_cnt);
            }
        }
        else
        { /** 按键松开 */
            if (key_cnt >= 5)
            {
                // debug_tx2("key pressed\r\n");
                if (GPS_massage == GPS_Message_ALL)
                {
                    GPS_Message_Only_VTG();
                }
                else if (GPS_massage == GPS_Message_VTG)
                {
                    GPS_Message_All();
                }

            }
            key_cnt = 0x00;
        }

        key_scan_tick = millis();
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

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
#include "led.h"
#include "gps.h"
#include <stdio.h>
#include <string.h>
#include "driver/usart1.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void LED_GPIO_Init(void)
{

    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
    LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);

    /*LED1*/
    GPIO_InitStruct.Pin = LED1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

    /*LED2*/
    GPIO_InitStruct.Pin = LED2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

    /*LED3*/
    GPIO_InitStruct.Pin = LED3_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

    /**/
    LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
    LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
    LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);
}

void led1_on(void)
{
    LL_GPIO_SetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

void led1_off(void)
{
    LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);
}

void led1_toggle(void)
{
    LL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}

void led2_on(void)
{
    LL_GPIO_SetOutputPin(LED2_GPIO_Port, LED2_Pin);
}

void led2_off(void)
{
    LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);
}

void led2_toggle(void)
{
    LL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}

void led3_on(void)
{
    LL_GPIO_SetOutputPin(LED3_GPIO_Port, LED3_Pin);
}

void led3_off(void)
{
    LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);
}

void led3_toggle(void)
{
    LL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
}


uint32_t led_status_tick = 0;
void Led_Status_Loop(void)
{
    static uint8_t led_bink_count = 0;
    uint8_t i = 0;
    uint8_t last_flg = 0;
    char* grm= NULL;
    static uint8_t now_flg = 0;

    if (millis() - led_status_tick > 200)
    {

        if (SendGPS.satellites > 3)
        {
            led1_on();
        }
        else
        {
            led_bink_count++;
            if (led_bink_count > 2)
            {
                led_bink_count = 0;
                led1_toggle();
            }
        }

        if (GPS_massage == GPS_Message_ALL)
        {
            led2_on();
            led3_off();
        }
        else if (GPS_massage == GPS_Message_VTG)
        {
            led3_on();
            led2_off();
        }

        led_status_tick = millis();
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

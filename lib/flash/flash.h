// Copyright (c) 2023 Ziga Miklosic
// All Rights Reserved
////////////////////////////////////////////////////////////////////////////////
/**
*@file      flash.h
*@brief     Flash LL drivers based on STM32 HAL library
*@author    Ziga Miklosic
*@email		ziga.miklosic@gmail.si
*@date      17.05.2023
*@version   V0.1.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup FLASH_API
* @{ <!-- BEGIN GROUP -->
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FLASH_H
#define __FLASH_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

/**
 *  Module version
 */
#define FLASH_VER_MAJOR          ( 0 )
#define FLASH_VER_MINOR          ( 1 )
#define FLASH_VER_DEVELOP        ( 0 )

#define FLASH_ADDR_START         (uint32_t)0x0801F000 

#define FLASH_BUF_SIZE         2

typedef struct save_data_s
{
    uint8_t gps_out_typede;
    // uint8_t save_flg;

}save_data_t;
 
extern save_data_t save_data;
/**
 *  Flash status
 */
typedef enum
{
    eFLASH_OK        = 0x00U,    /**<Normal operation */
    eFLASH_ERROR     = 0x01U,    /**<General error code */
} flash_status_t;

extern uint8_t flash_tx_data1[FLASH_BUF_SIZE];
extern uint8_t flash_tx_data2[FLASH_BUF_SIZE];
extern uint8_t flash_rx_data[FLASH_BUF_SIZE];

////////////////////////////////////////////////////////////////////////////////
// Functions
////////////////////////////////////////////////////////////////////////////////
flash_status_t flash_init       (void);
flash_status_t flash_deinit     (void);
flash_status_t flash_is_init    (bool * const p_is_init);
flash_status_t flash_write      (const uint32_t addr, const uint32_t size, const uint8_t * const p_data);
flash_status_t flash_read       (const uint32_t addr, const uint32_t size, uint8_t * const p_data);
flash_status_t flash_erase      (const uint32_t addr, const uint32_t size);
void flash_init_read(void);
#endif // __FLASH_H

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

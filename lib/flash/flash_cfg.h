// Copyright (c) 2023  Ziga Miklosic
// All Rights Reserved
////////////////////////////////////////////////////////////////////////////////
/**
*@file      flash_cfg.h
*@brief     Configuration for FLASH LL drivers based on STM32 HAL library
*@author    Ziga Miklosic
*@email		ziga.miklosic@gmail.si
*@date      17.05.2023
*@version   V0.1.0
*/
////////////////////////////////////////////////////////////////////////////////
/**
*@addtogroup FLASH_CFG_API
* @{ <!-- BEGIN GROUP -->
*
*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __FLASH_CFG_H
#define __FLASH_CFG_H

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// USER INCLUDE BEGIN...

#include "main.h"


// USER INCLUDE END...

////////////////////////////////////////////////////////////////////////////////
// Definitions
////////////////////////////////////////////////////////////////////////////////

// USER CODE BEGIN...

/**
 *      Enable/Disable dual-bank mode
 */
#define FLASH_CFG_DUAL_BANK_MODE_EN             ( 1 )

#if ( 1 == FLASH_CFG_DUAL_BANK_MODE_EN )

    /**
     *      Single bank size
     *
     *  Unit: bytes
     */
    #define FLACH_CFG_BANK_SIZE                     ( FLASH_BANK_SIZE )

    /**
     *      Start address of Bank 1
     */
    #define FLASH_CFG_BANK1_START_ADDR              ( 0x08000000 )

    /**
     *      Start address of Bank 2
     */
    #define FLASH_CFG_BANK2_START_ADDR              ( 0x08040000 )

#endif

/**
 *      Flash page size
 *
 *  Unit: byte
 */
#define FLASH_CFG_PAGE_SIZE_BYTE                ( FLASH_PAGE_SIZE )

/**
 *      Flash start address
 *
 *  @note   Together with flash size user flash region is defined!
 */
#define FLASH_CFG_START_ADDR                    ( 0x08008000 )

/**
 *  Flash size
 *
 *  @note   Together with flash start address user flash region is defined!
 *
 *  Unit: byte
 */
#define FLASH_CFG_SIZE_BYTE                     ( 480 * 1024 )

/**
 *  Enable/Disable assertions
 */
#define FLASH_CFG_ASSERT_EN                       ( 1 )

// USER CODE END...

// Disable asserts in releases
#ifndef DEBUG
    #undef FLASH_CFG_ASSERT_EN
    #define FLASH_CFG_ASSERT_EN 0
#endif

/**
 *  Assert definition
 */
#if ( 1 == FLASH_CFG_ASSERT_EN )
    // USER CODE BEGIN...
    #define FLASH_ASSERT(x)                       PROJ_CFG_ASSERT(x)
    // USER CODE END...
#else
    #define FLASH_ASSERT(x)                       { ; }
#endif


#endif // __FLASH_CFG_H

////////////////////////////////////////////////////////////////////////////////
/**
* @} <!-- END GROUP -->
*/
////////////////////////////////////////////////////////////////////////////////

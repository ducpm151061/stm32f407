#ifndef __FLASH_H__
#define __FLASH_H__

#include "stdio.h"
#include "string.h"
#include "Hardware.h"

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#define FLASH_REMS_CMD 0x90
#define FLASH_RDID_CMD 0x9F
#define FLASH_SPI_DUMMY 0x00
#define UNSUPPORT_CMD 0x00

#define FLASH_ENABLE 1
#define FLASH_DISABLE 0

/* Dinh nghia cac phien ban flash */
// Cypress
#define FL256P 1
#define FL256S 2
#define FL512S 3
#define FL01GS 4
// GigaDevice
#define GD127C 11
#define GD256C 12
// WinBond
#define WB64F 20
#define WB128F 21
#define WB256F 22

// Manufacture ID
#define CYPRESS_MID 0x01
#define GIGADEVICE_MID 0xC8
#define WINBOND_MID 0xEF

// Flash Region
#define FL256P_MIN_FLASH_1 (uint32_t)0
#define FL256P_MAX_FLASH_1 (uint32_t)16777216 /* 16MB First * 1024 x 1024 */

#define FL256P_MIN_FLASH_2 (uint32_t)16777216
#define FL256P_MAX_FLASH_2 (uint32_t)33554432 /* 16MB Last * 1024 x 1024 */

#define FL512S_MIN_FLASH_1 (uint32_t)0
#define FL512S_MAX_FLASH_1 (uint32_t)67108864 //33554432        /* 32MB First 1024 x 1024 */

#define FL512S_MIN_FLASH_2 (uint32_t)67108864
#define FL512S_MAX_FLASH_2 (uint32_t)67108864 /* 32MB Last * 1024 x 1024 */

#define FL01GS_MIN_FLASH_1 (uint32_t)0
#define FL01GS_MAX_FLASH_1 (uint32_t)67108864 /* 64MB First 1024 x 1024 */

#define FL01GS_MIN_FLASH_2 (uint32_t)67108864
#define FL01GS_MAX_FLASH_2 (uint32_t)134217728 /* 68MB Last * 1024 x 1024 */

typedef enum
{
    CYPRESS = 0,
    GIGADEVICE = 1,
    WINBOND = 2,
    UNKNOWN = 0xFF
} FlashType_t;

typedef struct
{
    uint8_t WREN;    // Write Enable
    uint8_t WRDI;    // Write Disable
    uint8_t RDSR;    // Read Status
    uint8_t READ;    // Read Data (3-byte Address)
    uint8_t READ4;   // Read Data (4-byte Address)
    uint8_t PP;      // Page Programing (3-byte Address)
    uint8_t PP4;     // Page programing (4-byte Address)
    uint8_t SE_4K;   // Erase 4K-byte Sector (3-byte Address)
    uint8_t SE4_4K;  // Erase 4K-byte Sector (4-byte Address)
    uint8_t SE_32K;  // Erase 4K-byte Sector (3-byte Address)
    uint8_t SE4_32K; // Erase 4K-byte Sector (4-byte Address)
    uint8_t SE_64K;  // Erase 4K-byte Sector (3-byte Address)
    uint8_t SE4_64K; // Erase 4K-byte Sector (4-byte Address)
    uint8_t CE;      // Chip Erase
    uint8_t ENTER4B; // Enter 4-byte Address mode
    uint8_t EXIT4B;  // Exit 4-byte Address mode
} FlashCommant_t;

typedef struct
{
    uint8_t ManufactureID;
    uint8_t DeviceID;
    uint8_t DeviceIDL;
    FlashType_t Type;
    uint8_t Version;
    uint32_t Size;
    uint8_t Is4Byte;
    FlashCommant_t Command;
} FlashManager_t;

#define xprintf DEBUG

#endif // __FLASH_H__

/******************************************************************************
 * @file        InternalFlash.c
 * @author
 * @version     V1.0.0
 * @date        15/01/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "DataDefine.h"
#include "FlashInternal.h"
#include "DriverUART.h"
/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static void InternalFlashInitialize(void)
{
    FLASH_Unlock();

    /* Clear All pending flags */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
}

/*****************************************************************************/
/**
 * @brief   :   Erase sector 6 & sector 7
 * @param   :   None
 * @retval  :   0 if success, > 0 if error
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static uint8_t InternalFlashPrepare(uint32_t Sector)
{
    uint8_t ret = 0;

    DisableAllUART();
    InternalFlashInitialize();

    if (FLASH_EraseSector(Sector, VoltageRange_3) != FLASH_COMPLETE)
        ret = 1;

    EnableAllUART();
    if (ret != 0)
        DebugPrint("\rLoi xoa Flash o Sector: %d\r\n", Sector);
    return ret;
}

/*****************************************************************************/
/**
 * @brief   :   Write to internal flash
 * @param   :   Addres, Buffer, Length
 * @retval  :   0 if success, > 0 if error
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static uint8_t InternalWriteBytes(uint32_t Address, uint8_t *Buffer, uint32_t Length)
{
    uint16_t i;
    uint8_t ret = 0;
    FLASH_Status Status;
    uint32_t OldAddress = Address;

    DisableAllUART();
    InternalFlashInitialize();

    for (i = 0; i < Length; i++)
    {
        Status = FLASH_ProgramByte(Address + i, Buffer[i]);

        if (Status != FLASH_COMPLETE)
        {
            ret = 1;
            break;
        }
    }

    if (ret != 0)
    {
        EnableAllUART();
        DebugPrint("\rError in writting to Flash: %X, %d\r\n", Address, Status);
        return ret;
    }

    /* Check written data */
    Address = OldAddress;
    for (i = 0; i < Length; i++)
    {
        if (Buffer[i] != *(__IO uint8_t *)(Address + i))
        {
            ret = 2;
            break;
        }
    }

    EnableAllUART();
    if (ret != 0)
        DebugPrint("\rLoi ghi tai dia chi: %X, gia tri: %d, %d\r\n", Address + i, (*(__IO uint8_t *)Address + i), Buffer[i]);
    return ret;
}

/*****************************************************************************/
/**
 * @brief   :   Read bytes from internal flash
 * @param   :   Addres, Length
 * @retval  :   Buffer
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static void InternalReadBytes(uint32_t Address, uint8_t *Buffer, uint32_t Length)
{
    uint32_t i;

    for (i = 0; i < Length; i++)
    {
        Buffer[i] = *(__IO uint8_t *)(Address + i);
    }
}

/*****************************************************************************/
/**
 * @brief   :   Write byte to internal flash
 * @param   :   Addres, Length
 * @retval  :   0 if success, 1 if error
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */

static uint8_t InternalWriteByte(uint32_t Address, uint8_t Data)
{
    FLASH_Status Status;

    InternalFlashInitialize();

    Status = FLASH_ProgramByte(Address, Data);

    if (Status != FLASH_COMPLETE)
        return 1;
    else
        return 0;
}

/*****************************************************************************/
/**
 * @brief   :   Write int number to internal flash
 * @param   :   Addres, Length
 * @retval  :   0 if success, 1 if error
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static uint8_t InternalWriteInt(uint32_t Address, uint16_t Data)
{
    FLASH_Status Status;

    InternalFlashInitialize();

    Status = FLASH_ProgramByte(Address, Data & 0xFF);
    if (Status != FLASH_COMPLETE)
        return 1;

    Status = FLASH_ProgramByte(Address + 1, Data >> 8);
    if (Status != FLASH_COMPLETE)
        return 2;

    //    Status = FLASH_ProgramHalfWord(Address,Data);
    //    if(Status != FLASH_COMPLETE) return 1;

    return 0;
}

/*****************************************************************************/
/**
 * @brief   :   Write long number to internal flash
 * @param   :   Addres, Length
 * @retval  :   0 if success, 1 if error
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static uint8_t InternalWriteLong(uint32_t Address, uint32_t Data)
{
    FLASH_Status Status;

    InternalFlashInitialize();

    Status = FLASH_ProgramByte(Address, Data & 0xFF);
    if (Status != FLASH_COMPLETE)
        return 1;

    Status = FLASH_ProgramByte(Address + 1, (Data >> 8) & 0xFF);
    if (Status != FLASH_COMPLETE)
        return 2;

    Status = FLASH_ProgramByte(Address + 2, (Data >> 16) & 0xFF);
    if (Status != FLASH_COMPLETE)
        return 3;

    Status = FLASH_ProgramByte(Address + 3, Data >> 24);
    if (Status != FLASH_COMPLETE)
        return 4;

    return 0;
}

/*****************************************************************************/
/**
 * @brief   :   Copy data in internal flash
 * @param   :   Addres, Length
 * @retval  :   0 if success, > 0 if error
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */
static uint8_t InternalCopyData(uint32_t SrcAddress, uint32_t DstAddress, uint32_t Length)
{
    FLASH_Status Status;
    uint16_t i;

    for (i = 0; i < Length; i++)
    {
        Status = FLASH_ProgramByte(DstAddress + i, *(__IO uint8_t *)(SrcAddress + i));

        if (*(__IO uint8_t *)SrcAddress != *(__IO uint8_t *)DstAddress)
            return 1;

        if (Status != FLASH_COMPLETE)
            return 2;
    }
    return 0;
}

/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
uint32_t GetSectorIndex(uint32_t Address)
{
    uint32_t sector = 0;

    if ((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_Sector_0;
    }
    else if ((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_Sector_1;
    }
    else if ((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_Sector_2;
    }
    else if ((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_Sector_3;
    }
    else if ((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_Sector_4;
    }
    else if ((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_Sector_5;
    }
    else if ((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_Sector_6;
    }
    else if ((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_Sector_7;
    }
    else if ((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_Sector_8;
    }
    else if ((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_Sector_9;
    }
    else if ((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_Sector_10;
    }
    else /*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
    {
        sector = FLASH_Sector_11;
    }

    return sector;
}

INTERNAL_FLASH_t Driver_InterFlash =
    {
        InternalFlashInitialize,
        InternalFlashPrepare,
        InternalWriteBytes,
        InternalReadBytes,
        InternalCopyData,
        InternalWriteByte,
        InternalWriteInt,
        InternalWriteLong,
        GetSectorIndex};

/********************************* END OF FILE *******************************/

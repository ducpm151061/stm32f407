/******************************************************************************
 * @file        SourceFileTemplate.c
 * @author
 * @version     V1.0.0
 * @date        15/01/2014
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include "Utilities.h"
#include "SPI_Flash.h"

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern System_t xSystem;

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/
extern void InitSPI(SPI_TypeDef *SPIx);
extern uint16_t SPI_SendByte(SPI_TypeDef *SPIx, uint8_t Data);

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/
static FlashManager_t FlashManager;

/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/
static uint32_t FlashGetSectorSize(void);
static uint8_t Flash_GetVersion(void);
static void Flash_InitCommand(FlashType_t Type);
static void Flash4AddressControl(uint8_t Enable);

//#define DEBUG_SPI_FLASH 1
/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :   0 if init Ok, otherwise return > 0
 * @author  :
 * @created :   15/01/2014
 * @version :
 * @reviewer:
 */

static uint8_t FlashInit(void)
{
    uint8_t ret;
    InitSPI(FLASH_SPI);

    if (xSystem.Debug.ExFlash != 0) {
#ifdef DEBUG_SPI_FLASH
        UART_Printf(DEBUG_UART, "\rFLASH Init Hardware...\r\n");
#endif			
		}
    ret = Flash_GetVersion();
    if (ret == 0)
    {
#ifdef DEBUG_SPI_FLASH
				if (xSystem.Debug.ExFlash != 0) {
					  UART_Printf(DEBUG_UART, " [OK]\r\n");					
				}
#endif
        //xSystem.GLStatus.TrangThaiHeThong[FLASHFAIL] = TRANGTHAI_OK;
        return 0;
    }
    else
    {
#ifdef DEBUG_SPI_FLASH
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, " [FAIL]\r\n");
#endif
        //Beep(200);
        return 2;
    }

    //  // test Flash Bock 128
    //  //  uint8_t buff[512];
    //  xSystem.Flash->EraseSector(128);
    //  xSystem.Flash->EraseSector(128);
    //  for(i=0;i<512;i++) buff[i]=i;
    //  xSystem.Flash->WriteBytes(33554432,buff,512);
    //
    //  for(i=0;i<512;i++) buff[i]=0;
    //  xSystem.Flash->ReadBytes(33554432,buff,512);
    //
    //  DebugPrint("\rRead Flash: ");
    //  for(i=0;i<512;i++) DebugPrint(" %d",buff[i]);
    //
    //  return 0;
}

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
static void FlashChipSelectControl(uint8_t Enable, uint32_t Address)
{
    if (Enable)
    {
        switch (FlashManager.Version)
        {
        case FL256P:
            if (Address >= FL256P_MIN_FLASH_2 && Address <= FL256P_MAX_FLASH_2)
            {
                GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_SET);
                GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_RESET);
            }
            else
            {
                GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_RESET);
                GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_SET);
            }
            break;
        case FL01GS:
            if (Address >= FL01GS_MIN_FLASH_2 && Address <= FL01GS_MAX_FLASH_2)
            {
                GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_SET);
                GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_RESET);
            }
            else
            {
                GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_RESET);
                GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_SET);
            }
            break;
        case FL512S:
            if (Address >= FL512S_MIN_FLASH_2 && Address <= FL512S_MAX_FLASH_2)
            {
                GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_SET);
                GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_RESET);
            }
            else
            {
                GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_RESET);
                GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_SET);
            }
            break;
        default:
            GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_RESET);
            GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_RESET);
            break;
        }
    }
    else
    {
        GPIO_WriteBit(FLASH_CS1_PORT, FLASH_CS1_PIN, Bit_SET);
        GPIO_WriteBit(FLASH_CS2_PORT, FLASH_CS2_PIN, Bit_SET);
    }
}

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
static void FlashWriteControl(uint8_t Enable, uint32_t Address)
{
    if (Enable)
    {
        FlashChipSelectControl(1, Address);
        SPI_SendByte(FLASH_SPI, FlashManager.Command.WREN);
        FlashChipSelectControl(0, 0);
    }
    else
    {
        FlashChipSelectControl(1, Address);
        SPI_SendByte(FLASH_SPI, FlashManager.Command.WRDI);
        FlashChipSelectControl(0, 0);
    }
}

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
static void WaitWriteInProcess(uint32_t Address)
{
    uint32_t TimeOut = 0x2FFFF;
    uint8_t Status = 0;

    FlashChipSelectControl(1, Address);
    SPI_SendByte(FLASH_SPI, FlashManager.Command.RDSR);

    while (TimeOut)
    {
        /* Doc thanh ghi */
        Status = SPI_SendByte(FLASH_SPI, FLASH_SPI_DUMMY);
        TimeOut--;
        if ((Status & 1) == 0)
            break;
        if (TimeOut % 1000 == 0)
            xSystem.Utility->WdtRst();
    }
    FlashChipSelectControl(0, 0);
}

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
static void FlashWritePage(uint32_t Address, uint8_t *Buffer, uint16_t Length)
{
    uint16_t i = 0;
    uint32_t OldAddress = Address;
    uint16_t TimeProcess = 0;

    TimeProcess = xSystem.TickCount1s;
    OldAddress = Address;
    FlashWriteControl(1, Address);
    FlashChipSelectControl(1, Address);

    /* Tinh lai dia chi */
    if (Address >= FL256P_MAX_FLASH_1 && FlashManager.Version == FL256P)
        Address = Address - FL256P_MAX_FLASH_1;
    else if (Address >= FL512S_MAX_FLASH_1 && FlashManager.Version == FL512S)
        Address = Address - FL512S_MAX_FLASH_1;
    else if (Address >= FL01GS_MAX_FLASH_1 && FlashManager.Version == FL01GS)
        Address = Address - FL01GS_MAX_FLASH_1;

    if (FlashManager.Is4Byte == 0)
    {
        /* Gui lenh */
        SPI_SendByte(FLASH_SPI, FlashManager.Command.PP);
    }
    else
    {
        SPI_SendByte(FLASH_SPI, FlashManager.Command.PP4);
        SPI_SendByte(FLASH_SPI, (Address >> 24) & 0xFF);
    }

    /* Gui 3 byte dia chi */
    SPI_SendByte(FLASH_SPI, (Address >> 16) & 0xFF);
    SPI_SendByte(FLASH_SPI, (Address >> 8) & 0xFF);
    SPI_SendByte(FLASH_SPI, Address & 0xFF);

    for (i = 0; i < Length; i++)
        SPI_SendByte(FLASH_SPI, Buffer[i]);

    //  FlashManager.Flash_SPI_Driver->SendData(Buffer, Length);

    TimeProcess = xSystem.TickCount1s - TimeProcess;
#ifdef DEBUG_SPI_FLASH		
    if (TimeProcess > 10)
        if (xSystem.Debug.ExFlash != 0) {
            UART_Printf(DEBUG_UART, "\rFWPage: %d", TimeProcess);
				}
#endif
    FlashChipSelectControl(0, 0);

    TimeProcess = xSystem.TickCount1s;
    WaitWriteInProcess(OldAddress);
    TimeProcess = xSystem.TickCount1s - TimeProcess;
#ifdef DEBUG_SPI_FLASH				
    if (TimeProcess > 10)
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, " PP Wait: %d", TimeProcess);
#endif
}
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
static void FlashWriteBytes(uint32_t Address, uint8_t *Buffer, uint16_t Length)
{
    /* Phan bo du lieu thanh tung page */
    uint16_t OffsetDiaChi = 0;
    uint16_t LuongDuLieuPhaiGhi = 0;
    uint16_t LuongDuLieuDaGhi = 0;

    while (Length)
    {
        OffsetDiaChi = Address % 256;

        if (OffsetDiaChi > 0)
        {
            if (OffsetDiaChi + Length > 256)
                LuongDuLieuPhaiGhi = 256 - OffsetDiaChi;
            else
                LuongDuLieuPhaiGhi = Length;
        }
        else
        {
            if (Length > 256)
                LuongDuLieuPhaiGhi = 256;
            else
                LuongDuLieuPhaiGhi = Length;
        }

        Length -= LuongDuLieuPhaiGhi;

        FlashWritePage(Address, &Buffer[LuongDuLieuDaGhi], LuongDuLieuPhaiGhi);

        /* Cap nhat gia tri */
        LuongDuLieuDaGhi += LuongDuLieuPhaiGhi;

        Address += LuongDuLieuPhaiGhi;
    }
}
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
static void FlashReadBytes(uint32_t Address, uint8_t *Buffer, uint16_t Length)
{
    uint16_t i = 0;

    FlashChipSelectControl(1, Address);

    /* Tinh lai dia chi */
    if (Address >= FL256P_MAX_FLASH_1 && FlashManager.Version == FL256P)
        Address = Address - FL256P_MAX_FLASH_1;
    else if (Address >= FL512S_MAX_FLASH_1 && FlashManager.Version == FL512S)
        Address = Address - FL512S_MAX_FLASH_1;
    else if (Address >= FL01GS_MAX_FLASH_1 && FlashManager.Version == FL01GS)
        Address = Address - FL01GS_MAX_FLASH_1;

    /* Gui lenh */
    if (FlashManager.Is4Byte == 0)
    {
        SPI_SendByte(FLASH_SPI, FlashManager.Command.READ);
    }
    else
    {
        SPI_SendByte(FLASH_SPI, FlashManager.Command.READ4);
        SPI_SendByte(FLASH_SPI, (Address >> 24) & 0xFF);
    }

    /* Gui 3 byte dia chi */
    SPI_SendByte(FLASH_SPI, (Address >> 16) & 0xFF);
    SPI_SendByte(FLASH_SPI, (Address >> 8) & 0xFF);
    SPI_SendByte(FLASH_SPI, Address & 0xFF);

    /* Lay du lieu */
    for (i = 0; i < Length; i++)
        Buffer[i] = SPI_SendByte(FLASH_SPI, FLASH_SPI_DUMMY);

    //  FlashManager.Flash_SPI_Driver->ReceiveData(Buffer, Length,SPI_DUMMY);

    FlashChipSelectControl(0, 0);
}

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
static void FlashEraseSector(uint16_t SectorCount)
{
    uint32_t Address = 0;
    uint32_t OldAddress = Address;
    uint16_t TimeProcess = 0;

    Address = SectorCount * FlashGetSectorSize();
    OldAddress = Address;

    TimeProcess = xSystem.TickCount1s;
#ifdef DEBUG_SPI_FLASH	
    if (xSystem.Debug.ExFlash != 0)
        UART_Printf(DEBUG_UART, "\rXoa sector %u tai dia chi: %x", SectorCount, Address);
#endif
    FlashWriteControl(1, Address);

    FlashChipSelectControl(1, Address);

    /* Tinh lai dia chi */
    if (Address >= FL256P_MAX_FLASH_1 && FlashManager.Version == FL256P)
        Address = Address - FL256P_MAX_FLASH_1;
    else if (Address >= FL512S_MAX_FLASH_1 && FlashManager.Version == FL512S)
        Address = Address - FL512S_MAX_FLASH_1;
    else if (Address >= FL01GS_MAX_FLASH_1 && FlashManager.Version == FL01GS)
        Address = Address - FL01GS_MAX_FLASH_1;

    if (FlashManager.Is4Byte == 0)
    {
        SPI_SendByte(FLASH_SPI, FlashManager.Command.SE_64K);
    }
    else
    {
        SPI_SendByte(FLASH_SPI, FlashManager.Command.SE4_64K);
        SPI_SendByte(FLASH_SPI, (Address >> 24) & 0xFF);
    }

    SPI_SendByte(FLASH_SPI, (Address >> 16) & 0xFF);
    SPI_SendByte(FLASH_SPI, (Address >> 8) & 0xFF);
    SPI_SendByte(FLASH_SPI, Address & 0xFF);
    TimeProcess = xSystem.TickCount1s - TimeProcess;
#ifdef DEBUG_SPI_FLASH
    if (TimeProcess > 10)
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, " Time: %d", TimeProcess);
#endif
				FlashChipSelectControl(0, 0);

    TimeProcess = xSystem.TickCount1s;
    WaitWriteInProcess(OldAddress);
    TimeProcess = xSystem.TickCount1s - TimeProcess;
#ifdef DEBUG_SPI_FLASH
				if (TimeProcess > 10)
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, " ER Wait: %d", TimeProcess);
#endif				
}

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
static uint32_t FlashGetSectorSize(void)
{
    switch (FlashManager.Version)
    {
    case FL01GS:
    case FL512S:
        return 262144;
    default:
        return 65536;
    }
}

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
static uint32_t FlashGetDensity(void)
{
    switch (FlashManager.Version)
    {
    case FL01GS:
        return 134217728;
    case FL512S:
        return 67108864;
    case FL256P:
    case FL256S:
        return 33554432;
    case GD256C:
        return 33554432;
    case GD127C:
        return 16777216;
    case WB64F:
        return 8388608;
    case WB256F:
        return 33554432;
    default:
        return 16777216; // Bang dung luong vung nho Camera
    }
}

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
static uint8_t FlashGetType(void)
{
    return FlashManager.Version;
}

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
static uint8_t Flash_GetVersion(void)
{
    uint8_t i, ret;

    // Reset Variables
    FlashManager.ManufactureID = 0xFF;
    FlashManager.DeviceID = 0xFF;
    FlashManager.Version = 0xFF;
    FlashManager.Type = UNKNOWN;
    FlashManager.Size = 0;

    FlashChipSelectControl(1, 0);

    SPI_SendByte(FLASH_SPI, FLASH_REMS_CMD);

    /* 3 byte address */
    SPI_SendByte(FLASH_SPI, 0);
    SPI_SendByte(FLASH_SPI, 0);
    SPI_SendByte(FLASH_SPI, 0);
    i = 0;
    do
    {
        FlashManager.ManufactureID = SPI_SendByte(FLASH_SPI, 0xFF);
    } while (++i < 15 && FlashManager.ManufactureID == 0);
    FlashManager.DeviceID = SPI_SendByte(FLASH_SPI, 0xFF);
    FlashManager.DeviceIDL = SPI_SendByte(FLASH_SPI, 0xFF);

    FlashChipSelectControl(0, 0);

    //if(FlashManager.ManufactureID!=0 || FlashManager.DeviceID!=0 ) Beep(50);

    if (FlashManager.ManufactureID == CYPRESS_MID)
    {
        FlashManager.Type = CYPRESS;

        switch (FlashManager.DeviceID)
        {
        case 0x18:
            FlashManager.Version = FL256S;
            FlashManager.Size = 33554432;
            break;
        case 0x19:
            FlashManager.Version = FL512S;
            FlashManager.Size = 67108864;
            break;
        }
    }
    else if (FlashManager.ManufactureID == GIGADEVICE_MID)
    {
        FlashManager.Type = GIGADEVICE;

        switch (FlashManager.DeviceID)
        {
        case 0x18:
            FlashManager.Version = GD256C;
            FlashManager.Size = 33554432;
            break;
        case 0x17:
            FlashManager.Version = GD127C;
            FlashManager.Size = 16777216;
            break;
        }
    }
    else if (FlashManager.ManufactureID == WINBOND_MID)
    {
        FlashManager.Type = WINBOND;

        switch (FlashManager.DeviceID)
        {
        case 0x16:
            FlashManager.Version = WB64F;
            FlashManager.Size = 8388608;
            break;

        case 0x18:
            FlashManager.Version = WB256F;
            FlashManager.Size = 33554432;
            break;
        }
    }
    else
    {
#ifdef DEBUG_SPI_FLASH			
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "\rUnknow ID: %02X, %02X, %02X", FlashManager.ManufactureID, FlashManager.DeviceID, FlashManager.DeviceIDL);
#endif    
		}

    if (FlashManager.Version == FL512S)
    {
        FlashChipSelectControl(1, 0);
        SPI_SendByte(FLASH_SPI, FLASH_RDID_CMD);

        for (i = 0; i < 100; i++)
        {
            ret = SPI_SendByte(FLASH_SPI, 0);

            if (i == 0x27)
            {
                if (ret == 0x1B) // FL01GS - 1024 bytes
                {
                    FlashManager.Version = FL01GS;
                    FlashManager.Size = 134217728;
                    break;
                }
            }
        }

        FlashChipSelectControl(0, 0);
    }

    // Kh?i t?o m� l?nh s? d?ng cho t?ng lo?i Flash
    Flash_InitCommand(FlashManager.Type);

    if (FlashManager.Size > 16777216)
        FlashManager.Is4Byte = 1;
    if (FlashManager.Is4Byte == 1)
    {
        Flash4AddressControl(1);
    }

    // Display Data
#ifdef DEBUG_SPI_FLASH		
    if (xSystem.Debug.ExFlash != 0)
        UART_Printf(DEBUG_UART, "\r\t\tMFR: ");
    switch (FlashManager.Type)
    {
    case CYPRESS:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "Cypress");
        break;
    case GIGADEVICE:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "GigaDevice");
        break;
    case WINBOND:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "WinBond");
        break;
    default:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "Unknown: %d", FlashManager.Type);
        break;
    }

    if (xSystem.Debug.ExFlash != 0) {
        UART_Printf(DEBUG_UART, "\r\t\tMPN: ");
		}
    switch (FlashManager.Version)
    {
    case FL256S:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "S25FL256S");
        break;
    case FL512S:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "S25FL512S");
        break;
    case FL01GS:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "S70FL01GS");
        break;
    case GD256C:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "GD25Q256C");
        break;
    case GD127C:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "GD25Q127C");
        break;
    case WB64F:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "W25Q64F");
        break;
    case WB256F:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "W25Q256JV");
        break;
    default:
        if (xSystem.Debug.ExFlash != 0)
            UART_Printf(DEBUG_UART, "UNKNOW, %u", FlashManager.Version);
        break;
    }

    if (xSystem.Debug.ExFlash != 0) {
        UART_Printf(DEBUG_UART, "\r\t\tCAP: %d bytes", FlashManager.Size);
		}
    if (xSystem.Debug.ExFlash != 0) {
        UART_Printf(DEBUG_UART, "\r\t\tSectorSize: %d bytes", xSystem.ExFlash->GetSectorSize());
		}
    if (xSystem.Debug.ExFlash != 0) {
        UART_Printf(DEBUG_UART, "\r\t\tAddr: %s", (FlashManager.Is4Byte == 1) ? "4B" : "3B");
		}
#endif
    if (FlashManager.Version == 0xFF)
        return 0xFF;
    return 0;
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   16/08/2017
 * @comment :
 */
static void Flash_InitCommand(FlashType_t Type)
{
    switch (Type)
    {
    case CYPRESS:
        FlashManager.Command.WREN = 0x06;
        FlashManager.Command.WRDI = 0x04;
        FlashManager.Command.RDSR = 0x05;
        FlashManager.Command.READ = 0x03;
        FlashManager.Command.READ4 = 0x13;
        FlashManager.Command.PP = 0x02;
        FlashManager.Command.PP4 = 0x12;
        FlashManager.Command.SE_4K = 0x20;
        FlashManager.Command.SE4_4K = 0x21;
        FlashManager.Command.SE_32K = UNSUPPORT_CMD;
        FlashManager.Command.SE4_32K = UNSUPPORT_CMD;
        FlashManager.Command.SE_64K = 0xD8;
        FlashManager.Command.SE4_64K = 0xDC;
        FlashManager.Command.CE = 0x60;
        FlashManager.Command.ENTER4B = UNSUPPORT_CMD;
        FlashManager.Command.EXIT4B = UNSUPPORT_CMD;
        break;
    case GIGADEVICE:
        FlashManager.Command.WREN = 0x06;
        FlashManager.Command.WRDI = 0x04;
        FlashManager.Command.RDSR = 0x05;
        FlashManager.Command.READ = 0x03;
        FlashManager.Command.READ4 = 0x13;
        FlashManager.Command.PP = 0x02;
        FlashManager.Command.PP4 = 0x12;
        FlashManager.Command.SE_4K = 0x20;
        FlashManager.Command.SE4_4K = 0x21;
        FlashManager.Command.SE_32K = 0x52;
        FlashManager.Command.SE4_32K = 0x5C;
        FlashManager.Command.SE_64K = 0xD8;
        FlashManager.Command.SE4_64K = 0xDC;
        FlashManager.Command.CE = 0x60;
        FlashManager.Command.ENTER4B = 0xB7;
        FlashManager.Command.EXIT4B = 0xB9;
        break;
    case WINBOND:
        FlashManager.Command.WREN = 0x06;
        FlashManager.Command.WRDI = 0x04;
        FlashManager.Command.RDSR = 0x05;
        FlashManager.Command.READ = 0x03;
        FlashManager.Command.READ4 = 0x13;
        FlashManager.Command.PP = 0x02;
        FlashManager.Command.PP4 = 0x02;
        FlashManager.Command.SE_4K = 0x20;
        FlashManager.Command.SE4_4K = 0x20;
        FlashManager.Command.SE_32K = 0x52;
        FlashManager.Command.SE4_32K = 0x52;
        FlashManager.Command.SE_64K = 0xD8;
        FlashManager.Command.SE4_64K = 0xD8;
        FlashManager.Command.CE = 0x60;
        FlashManager.Command.ENTER4B = 0xB7;
        FlashManager.Command.EXIT4B = 0xB9;
        break;
    default:
        FlashManager.Command.WREN = UNSUPPORT_CMD;
        FlashManager.Command.WRDI = UNSUPPORT_CMD;
        FlashManager.Command.RDSR = UNSUPPORT_CMD;
        FlashManager.Command.READ = UNSUPPORT_CMD;
        FlashManager.Command.READ4 = UNSUPPORT_CMD;
        FlashManager.Command.PP = UNSUPPORT_CMD;
        FlashManager.Command.PP4 = UNSUPPORT_CMD;
        FlashManager.Command.SE_4K = UNSUPPORT_CMD;
        FlashManager.Command.SE4_4K = UNSUPPORT_CMD;
        FlashManager.Command.SE_32K = UNSUPPORT_CMD;
        FlashManager.Command.SE4_32K = UNSUPPORT_CMD;
        FlashManager.Command.SE_64K = UNSUPPORT_CMD;
        FlashManager.Command.SE4_64K = UNSUPPORT_CMD;
        FlashManager.Command.CE = UNSUPPORT_CMD;
        FlashManager.Command.ENTER4B = UNSUPPORT_CMD;
        FlashManager.Command.EXIT4B = UNSUPPORT_CMD;
        break;
    }
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :
 * @created :   08/10/2016
 * @comment :
 */
static void Flash4AddressControl(uint8_t Enable)
{
    if (Enable == 1 && FlashManager.Command.ENTER4B != UNSUPPORT_CMD)
    {
        FlashChipSelectControl(1, 0);
        SPI_SendByte(FLASH_SPI, FlashManager.Command.ENTER4B);
        FlashChipSelectControl(0, 0);
    }
    else if (Enable == 0 && FlashManager.Command.EXIT4B != UNSUPPORT_CMD)
    {
        FlashChipSelectControl(1, 0);
        SPI_SendByte(FLASH_SPI, FlashManager.Command.EXIT4B);
        FlashChipSelectControl(0, 0);
    }
}

Flash_External_t Driver_Flash =
    {
        FlashInit,
        FlashWriteBytes,
        FlashReadBytes,
        FlashEraseSector,
        FlashGetSectorSize,
        FlashGetDensity,
        FlashGetType};

/********************************* END OF FILE *******************************/

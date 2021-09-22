#include "DataDefine.h"
#include "AlcoholMeter.h"
#include "Utilities.h"
#include "UserGuiService.h"

#define FLASH_ADDR_RECORD_BASE 0x0

extern System_t xSystem;
static uint32_t RecordAddressWriting;

uint32_t RecordCount = 0;

void ReadIndexRecord(void)
{
    RecordCount = RTC_ReadBackupRegister(RTC_BKP_DR1);
    xSystem.Record.Name.IndexStore = RecordCount - 1;
}

int32_t GetIndexStore(void)
{
    ReadIndexRecord();
    return (int16_t)xSystem.Record.Name.IndexStore;
}

void StoreIndexRecord(void)
{
    RTC_WriteBackupRegister(RTC_BKP_DR1, RecordCount);
}

void Record_Mem_Prepair(void)
{
    RecordAddressWriting = FLASH_ADDR_RECORD_BASE + xSystem.Record.Name.IndexStore * RECORD_SIZE;

    // Sang Sector moi => Xoa Sector ke tiep
    if (RecordAddressWriting % xSystem.ExFlash->GetSectorSize() < RECORD_SIZE)
    {
        //DebugPrint("\rFlash Prepair Finish: %d %d\r\n", RecordAddressWriting, xSystem.ExFlash->GetSectorSize());
        xSystem.ExFlash->EraseSector(RecordAddressWriting / xSystem.ExFlash->GetSectorSize() + 1);
    }

    // Over mem
    if (xSystem.Record.Name.IndexStore > RECORD_MAXIMUM_STRORE_INFLASH)
    {
        //DebugPrint("\rFlash Prepair Finish: %d %d\r\n", RecordAddressWriting, xSystem.Record.Name.IndexStore);
        xSystem.ExFlash->EraseSector(FLASH_ADDR_RECORD_BASE / xSystem.ExFlash->GetSectorSize());
        xSystem.ExFlash->EraseSector(FLASH_ADDR_RECORD_BASE / xSystem.ExFlash->GetSectorSize() + 1);
        xSystem.Record.Name.IndexStore = 0;
    }
}

void Record_Store(void)
{
    uint8_t Retry;
    int Result;
    uint8_t BuffTemp[RECORD_SIZE] = {0};

    static DateTime_t OldTimeRecord;
    ReadIndexRecord();
    xSystem.Record.Name.IndexStore = RecordCount;
    DebugPrint("\rStore Record: %d\r\n", xSystem.Record.Name.IndexStore);

    xSystem.Keypad.WD_Req = WD_KQ_REPORT;
    strcpy((char *)xSystem.Keypad.Header, Txt_ThongBao);
    if (memcmp((const void *)&OldTimeRecord, (const void *)&xSystem.Record.Name.Time, 6) == 0)
    {
        strcpy((char *)xSystem.Keypad.Str, Txt_KhongLuuThongTin);
        xSystem.Keypad.Status = 1;
        GUI_WD_Change(WD_NOTIFISIMPLE);
        return;
    }
    OldTimeRecord = xSystem.Record.Name.Time;
    //DebugPrint("\rFlash Prepair: 0x%08x. %d\r\n", RecordAddressWriting, xSystem.Record.Name.IndexStore);
    Record_Mem_Prepair();
    //DebugPrint("\rFlash Prepair Finis: 0x%08x. \r\n", RecordAddressWriting);
    strcpy((char *)xSystem.Keypad.Str, Txt_DangLuu);
    xSystem.Keypad.Status = 0;
    GUI_WD_Change(WD_NOTIFISIMPLE);
    // Map Data to Buffer
    DebugPrint("\rFlash Write: 0x%08x. \r\n", RecordAddressWriting);
    Retry = 0;
    do
    {
        DebugPrint("\rFlash Read[%d], Address: 0x%08x. \r\n", Retry, RecordAddressWriting);
        xSystem.ExFlash->ReadBytes(RecordAddressWriting, BuffTemp, RECORD_SIZE);
        //for(i=0;i<RECORD_SIZE-4;i++) DebugPrint("%02x ",BuffTemp[i]);

        DebugPrint("\rFlash Wire[%d], Address: 0x%08x. \r\n", Retry, RecordAddressWriting);
        //for(i=0;i<RECORD_SIZE-4;i++) DebugPrint("%02x ",xSystem.Record.Buff[i]);
        // Add CRC here
        uint16_t crc = CalculateChecksumCRC16((uint8_t *)xSystem.Record.Buff, sizeof(xSystem.Record.Name) - 2);
        xSystem.ExFlash->WriteBytes(RecordAddressWriting, xSystem.Record.Buff, RECORD_SIZE);

        DebugPrint("\rFlash Read[%d], Address: 0x%08x.\r\n", Retry, RecordAddressWriting);
        memset(BuffTemp, 0, sizeof(BuffTemp));
        xSystem.ExFlash->ReadBytes(RecordAddressWriting, BuffTemp, RECORD_SIZE - 4);
        //for(i=0;i<RECORD_SIZE-4;i++) DebugPrint("%02x ",BuffTemp[i]);
        Result = 0;

        uint16_t crc2 = CalculateChecksumCRC16((uint8_t *)BuffTemp, sizeof(xSystem.Record.Name) - 2);

        Result = memcmp(xSystem.Record.Buff, BuffTemp, RECORD_SIZE - 4);
        DebugPrint("\rResult: %d.\r\n", Result);

        if (crc != crc2)
        {
            DebugPrint("\rStoreRecord: CRC ERROR: %04X != %04X\r\n", crc, crc2);
        }
        else
        {
            DebugPrint("\rStoreRecord: CRC OK: %04X == %04X\r\n", crc, crc2);
        }
        if (Result != 0)
        {
            DebugPrint("\rStoreRecord: MemCMP error, reset flash: %04X == %04X\r\n", crc, crc2);
            xSystem.ExFlash->Init();
        }
        else
        {
            strcpy((char *)xSystem.Keypad.Str, Txt_DaLuuThongTin);
            RecordCount++;
            StoreIndexRecord();
            sprintf((char *)BuffTemp, ": %05d.", xSystem.Record.Name.IndexStore);
            strncat((char *)xSystem.Keypad.Str, (const char *)BuffTemp, 10);
            GUI_WD_Change(WD_NOTIFISIMPLE);
        }
    } while (++Retry < 3 & Result != 0);
}

void Record_Read(uint16_t Index)
{
    uint32_t Addr = FLASH_ADDR_RECORD_BASE + Index * RECORD_SIZE;
    xSystem.ExFlash->ReadBytes(Addr, xSystem.Record.Buff, sizeof(xSystem.Record));
    //for(i=0;i<RECORD_SIZE-4;i++) DebugPrint("%02x ",xSystem.Record.Buff[i]);
    // Pase Data From Buffer
}

void FullClear_Record(void)
{
    xSystem.Record.Name.IndexStore = RecordCount = 0;
    RTC_WriteBackupRegister(RTC_BKP_DR1, RecordCount);
    xSystem.ExFlash->EraseSector(FLASH_ADDR_RECORD_BASE / xSystem.ExFlash->GetSectorSize());
    DebugPrint("X�a tat ca ban ghi \n");
}

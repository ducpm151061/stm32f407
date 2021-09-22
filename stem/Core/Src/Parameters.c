/******************************************************************************
 * @file        Parameters.c
 * @author
 * @version     V1.0.0
 * @date        28/02/2016
 * @brief
 ******************************************************************************/

/******************************************************************************
                                   INCLUDES
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "Utilities.h"
#include "Parameters.h"
#include "string.h"

extern void Debugvoid(const char *str, ...);

/*��Ϣ���*/
#define PARA_DEBUG_ON 1
#if PARA_DEBUG_ON
#define PARA_DEBUG DebugPrint
#else
#define PARA_DEBUG
#endif

#ifndef READ_PARAMS_BATCH
#define READ_WRITE_PARAMS_BATCH 1
#endif

/******************************************************************************
                                   GLOBAL VARIABLES
 ******************************************************************************/
extern System_t xSystem;

/******************************************************************************
                                   GLOBAL FUNCTIONS
 ******************************************************************************/

/******************************************************************************
                                   DATA TYPE DEFINE
 ******************************************************************************/
#if 0
__attribute__((at(PARAMETERS_BASE)))  const uint8_t CFG_DATA[1024 * 16] =
{
    //  00  01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31
    /*32-DeviceSerial*/ 'A', 'l', 'M', '_', '2', '1', '0', '5', '0', '0', '1', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*12-Product Date*/ '2', '0', '2', '0', '/', '1', '2', '/', '1', '0', 0, 0,
    /*12-Calib   Date*/ '2', '0', '2', '0', '/', '1', '2', '/', '1', '0', 0, 0,
    /*12-Ress    Date*/ '2', '0', '2', '0', '/', '1', '2', '/', '1', '0', 0, 0,
    /*04-Para 1      */  0, 0, 0, 1,                        // Blow Time Length
    /*04-Para 2      */  0, 0, 0, 1,                        // Blow Press Level
    /*04-Para 3      */  0, 0, 0, 3,                        // Time Off Device
    /*04-Para 4      */  0, 0, 0, 2,                        // Timeout Key
    /*08-Para 5      */ '1', '2', '3', '4', '5', '6', '7', '8', // Passkey
    /*04-Para 6      */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*04-Para 6      */  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /*82-Calib Table */  4, 0, 1, 0, 1, 0, 150, 0, 50, 0, 200, 0, 100, 0, 0, 2, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
#endif

/******************************************************************************
                                   PRIVATE VARIABLES
 ******************************************************************************/
static uint8_t tmpBuff[100];
/******************************************************************************
                                   LOCAL FUNCTIONS
 ******************************************************************************/

/*****************************************************************************/
/**
**/

void ReadConfig(uint8_t ParamNum)
{
    uint8_t i, ConfigTick = 1, tmpCount = 0;
    uint32_t *ptr;
    Long_t TmpLong;
    //  PARA_DEBUG("\rRead Parameter...");
    //  PARA_DEBUG("\rMem Using: %d",MEM_END_ADDR-PARAMETERS_BASE);
    Al_CalibTable_t *AlMCalibData = &xSystem.Parameters.AlMCalibData;
    Parameter_t *Parameters = &xSystem.Parameters;
    xSystem.InternalFlash->Init();
#if READ_WRITE_PARAMS_BATCH
    if (ParamNum != 0)
    {
        return;
    }

    int readSize = sizeof(Parameter_t);
    char *buffTmp = malloc(readSize + 2);
    if (buffTmp == NULL)
        return;
    Parameter_t *ParametersTmp = (Parameter_t *)(buffTmp);
    memset(buffTmp, 0, readSize + 2);
    xSystem.InternalFlash->ReadBytes(PARAMETERS_BASE, (uint8_t *)buffTmp, readSize);
    uint16_t crc = CalculateChecksumCRC16((uint8_t *)buffTmp, readSize - 2);
    uint16_t *pCrc = (uint16_t *)(buffTmp + (readSize - 2));

    DebugPrint("\r\nREAD CONFIG, Size:%d:%d Calc:%04X, CRC16:%04X, pCrc:%04x flashCrc:%94X\r\n",
               readSize, sizeof(xSystem.Parameters.AlMCalibData), crc, xSystem.Parameters.CRC16, *pCrc, ParametersTmp->CRC16);

    if (crc != *pCrc)
    {
        snprintf((char *)Parameters->DeviceID, sizeof(Parameters->DeviceID), "%s", "A00000001");
        snprintf((char *)Parameters->SetupKey, sizeof(Parameters->SetupKey), "%s", "12345678");
        Parameters->BlowTime = 1;
        Parameters->BlowPressure = 1;
        Parameters->TimeAutoTurnOff = 5;
        Parameters->TimeoutKeypad = 2;
        // Invalid config
        //xSystem.TimeOut.Clk1s_UpdateCFGToFlash=1;
        //Al_ValueInTable_t AlMCalibData = xSystem.Parameters.AlMCalibData.s.Data;
        Al_ValueInTable_t *pCalibData = AlMCalibData->s.Data;
        xSystem.Parameters.AlMCalibData.s.Al_MaxValue = 11;
        //AlMCalibData.s.Al_MaxValue = 11;
        pCalibData[0].ValueADC = 0;
        pCalibData[0].ValueMgL = 0;

        pCalibData[1].ValueADC = 187;
        pCalibData[1].ValueMgL = 50;

        pCalibData[2].ValueADC = 383;
        pCalibData[2].ValueMgL = 100;

        pCalibData[3].ValueADC = 435;
        pCalibData[3].ValueMgL = 150;

        pCalibData[4].ValueADC = 686;
        pCalibData[4].ValueMgL = 200;

        pCalibData[5].ValueADC = 714;
        pCalibData[5].ValueMgL = 250;

        pCalibData[6].ValueADC = 831;
        pCalibData[6].ValueMgL = 300;

        pCalibData[7].ValueADC = 999;
        pCalibData[7].ValueMgL = 350;

        pCalibData[8].ValueADC = 1145;
        pCalibData[8].ValueMgL = 400;

        pCalibData[9].ValueADC = 1729;
        pCalibData[9].ValueMgL = 600;

        pCalibData[10].ValueADC = 2894;
        pCalibData[10].ValueMgL = 972;
    }
    else
    {
        memcpy(&xSystem.Parameters, buffTmp, readSize);
    }
    xSystem.Parameters.CRC16 = CalculateChecksumCRC16((uint8_t *)buffTmp, readSize - 2);
    free(buffTmp);
    return;
#endif
    {
        // === Get CFG_DeviceSerial ===
        if (!ParamNum || ParamNum == CFG_DeviceSerial)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_SERIAL_ADDR, xSystem.Parameters.DeviceID, DEVICE_SERIAL_LEN);

            if (xSystem.Parameters.DeviceID[0] == 0 || xSystem.Parameters.DeviceID[0] == 0xFF)
            {
                tmpCount = sprintf((char *)xSystem.Parameters.DeviceID, DEVICE_SERIAL_DF);
                xSystem.Parameters.DeviceID[tmpCount] = 0;
                PARA_DEBUG("\rDefault SERIAL: [%u:%u]:%s", tmpCount, strlen((char *)xSystem.Parameters.DeviceID), xSystem.Parameters.DeviceID);
            }
            PARA_DEBUG("\rDeviceSerial:%s", xSystem.Parameters.DeviceID);
        }
    }
    // === End Get CFG_DeviceSerial ===

    {
        // === Get CFG_Device_ProductDate ===
        if (!ParamNum || ParamNum == CFG_Device_ProductDate)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_PRODUCTDATE_ADDR, xSystem.Parameters.NgayXuatXuong, DEVICE_PRODUCTDATE_LEN);

            if (xSystem.Parameters.NgayXuatXuong[0] == 0 || xSystem.Parameters.NgayXuatXuong[0] == 0xFF)
            {
                tmpCount = sprintf((char *)xSystem.Parameters.NgayXuatXuong, DEVICE_PRODUCTDATE_DF);
                xSystem.Parameters.NgayXuatXuong[tmpCount] = 0;
                PARA_DEBUG("\rDefault NgayXuatXuong: [%u:%u]:%s", tmpCount, strlen((char *)xSystem.Parameters.NgayXuatXuong), xSystem.Parameters.NgayXuatXuong);
            }
            PARA_DEBUG("\rDevice_ProductDate:%s", xSystem.Parameters.NgayXuatXuong);
        }
    }
    // === End Get CFG_Device_ProductDate ===

    {
        // === Get CFG_Device_CalibDate ===
        if (!ParamNum || ParamNum == CFG_Device_CalibDate)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_CALIBDATE_ADDR, xSystem.Parameters.NgayHieuChinh, DEVICE_CALIBDATE_LEN);

            if (xSystem.Parameters.NgayHieuChinh[0] == 0 || xSystem.Parameters.NgayHieuChinh[0] == 0xFF)
            {
                tmpCount = sprintf((char *)xSystem.Parameters.NgayHieuChinh, DEVICE_CALIBDATE_DF);
                xSystem.Parameters.NgayHieuChinh[tmpCount] = 0;
                PARA_DEBUG("\rDefault NgayHieuChinh: [%u:%u]:%s", tmpCount, strlen((char *)xSystem.Parameters.NgayHieuChinh), xSystem.Parameters.NgayHieuChinh);
            }
            PARA_DEBUG("\rDevice_CalibDate:%s", xSystem.Parameters.NgayHieuChinh);
        }
    }
    // === End Get CFG_Device_CalibDate ===

    {
        // === Get CFG_BlowTime ===
        if (!ParamNum || ParamNum == CFG_BlowTime)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_BLOWTIME_ADDR, TmpLong.bytes, 4);
            xSystem.Parameters.BlowTime = (uint8_t)TmpLong.value;
            if (xSystem.Parameters.BlowTime > 5 || xSystem.Parameters.BlowTime == 0)
            {
                PARA_DEBUG("\rBlowTime:0x%x-%d", DEVICE_BLOWTIME_ADDR, xSystem.Parameters.BlowTime);
                xSystem.Parameters.BlowTime = 3;
            }
            PARA_DEBUG("\rBlowTime:%d", xSystem.Parameters.BlowTime);
        }
    }
    // === End Get CFG_BlowTime ===

    {
        // === Get CFG_BlowPressure ===
        if (!ParamNum || ParamNum == CFG_BlowPressure)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_BLOWPRESS_ADDR, TmpLong.bytes, 4);
            xSystem.Parameters.BlowPressure = (uint8_t)TmpLong.value;
            if (xSystem.Parameters.BlowPressure > 5 || xSystem.Parameters.BlowPressure == 0)
            {
                PARA_DEBUG("\rBlowPressure:0x%x-%d", DEVICE_BLOWPRESS_ADDR, xSystem.Parameters.BlowPressure);
                xSystem.Parameters.BlowPressure = 3;
            }
            PARA_DEBUG("\rBlowPressure:%d", xSystem.Parameters.BlowPressure);
        }
    }
    // === End Get CFG_BlowPressure ===

    {
        // === Get CFG_BlowTime ===
        if (!ParamNum || ParamNum == CFG_TimeAutoTurnOff)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_TIMEOFF_ADDR, TmpLong.bytes, 4);
            xSystem.Parameters.TimeAutoTurnOff = (uint8_t)TmpLong.value;
            if (xSystem.Parameters.TimeAutoTurnOff > 5 || xSystem.Parameters.TimeAutoTurnOff == 0)
            {
                PARA_DEBUG("\rTimeAutoTurnOff:%d", xSystem.Parameters.TimeAutoTurnOff);
                xSystem.Parameters.TimeAutoTurnOff = 3;
            }
            PARA_DEBUG("\rTimeAutoTurnOff:%d", xSystem.Parameters.TimeAutoTurnOff);
        }
    }
    // === End Get CFG_BlowTime ===

    {
        // === Get CFG_TimeoutKey ===
        if (!ParamNum || ParamNum == CFG_TimeoutKey)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_KEYTO_ADDR, TmpLong.bytes, 4);
            xSystem.Parameters.TimeoutKeypad = (uint8_t)TmpLong.value;
            if (xSystem.Parameters.TimeoutKeypad > 5 || xSystem.Parameters.TimeoutKeypad == 0)
            {
                PARA_DEBUG("\rTimeoutKey:%d", xSystem.Parameters.TimeoutKeypad);
                xSystem.Parameters.TimeoutKeypad = 3;
            }
            PARA_DEBUG("\rTimeoutKey:%d", xSystem.Parameters.TimeoutKeypad);
        }
    }
    // === End Get CFG_TimeoutKey ===

    {
        // === Get CFG_SetupKey ===
        if (!ParamNum || ParamNum == CFG_SetupKey)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_PASSKEY_ADDR, xSystem.Parameters.SetupKey, DEVICE_PASSKEY_LEN);
            for (i = 0; i < 8; i++)
                if (xSystem.Parameters.SetupKey[i] < '0' || xSystem.Parameters.SetupKey[i] > '9')
                {
                    for (i = 0; i < 8; i++)
                        xSystem.Parameters.SetupKey[i] = '0';
                    break;
                }
            PARA_DEBUG("\rSetupKey:%s", xSystem.Parameters.SetupKey);
        }
    }
    // === End Get CFG_SetupKey ===

    {
        // === Get CFG_ALCalib_Table ===
        if (!ParamNum || ParamNum == CFG_ALCalib_Table)
        {
            xSystem.InternalFlash->ReadBytes(DEVICE_ALM_CALIB_ADDR, AlMCalibData->b, DEVICE_ALM_CALIB_LEN);
            if (AlMCalibData->s.Data[0].ValueADC == 0xFFFF || AlMCalibData->s.Data[0].ValueMgL == 0xFFFF)
            {
                //PARA_DEBUG("\rDefault CalibTable: [%u:%u]:%s",tmpCount,strlen((char*)tmpBuff),tmpBuff);
                //AlMCalibData.s.Al_MaxValue=0;
                Al_ValueInTable_t *pCalibData = AlMCalibData->s.Data;
                AlMCalibData->s.Al_MaxValue = 11;
                pCalibData[0].ValueADC = 0;
                pCalibData[0].ValueMgL = 0;

                pCalibData[1].ValueADC = 187;
                pCalibData[1].ValueMgL = 50;

                pCalibData[2].ValueADC = 383;
                pCalibData[2].ValueMgL = 100;

                pCalibData[3].ValueADC = 435;
                pCalibData[3].ValueMgL = 150;

                pCalibData[4].ValueADC = 686;
                pCalibData[4].ValueMgL = 200;

                pCalibData[5].ValueADC = 714;
                pCalibData[5].ValueMgL = 250;

                pCalibData[6].ValueADC = 831;
                pCalibData[6].ValueMgL = 300;

                pCalibData[7].ValueADC = 999;
                pCalibData[7].ValueMgL = 350;

                pCalibData[8].ValueADC = 1145;
                pCalibData[8].ValueMgL = 400;

                pCalibData[9].ValueADC = 1729;
                pCalibData[9].ValueMgL = 600;

                pCalibData[10].ValueADC = 2894;
                pCalibData[10].ValueMgL = 972;
            }
            //PARA_DEBUG("\rCalibTable[%d]:",AlMCalibData->s.Al_MaxValue);
            for (i = 0; i < AlMCalibData->s.Al_MaxValue; i++)
                PARA_DEBUG("\r%d-%d ", AlMCalibData->s.Data[i].ValueADC, AlMCalibData->s.Data[i].ValueMgL);
        }
    }
}
/*****************************************************************************/
/**
 * @brief   :   Update parameters to memory
 * @param   :
 * @retval  :
 * @author  :   -------------
 * @created :   20/11/2017
 * @version :
 * @reviewer:
 */

void UpdateParametersToMemory(void)
{
    uint16_t i;
    FLASH_Status Status;

    int readSize = sizeof(Parameter_t);
    uint16_t oldCrc = xSystem.Parameters.CRC16;
    uint16_t crc = CalculateChecksumCRC16((uint8_t *)&xSystem.Parameters, readSize - 2);
    uint16_t *pCrc = (uint16_t *)(((char *)&xSystem.Parameters) + (readSize - 2));
    *pCrc = crc;
    DebugPrint("WRITE CONFIG, Size:%d, Calib:%d Old:%04x, Calc:%04X, CRC16:%04X:%04X\r\n",
               sizeof(xSystem.Parameters), sizeof(xSystem.Parameters.AlMCalibData),
               oldCrc, crc, xSystem.Parameters.CRC16, *pCrc);
    xSystem.Parameters.CRC16 = crc;
    xSystem.InternalFlash->Init();
    DebugPrint("WRITE CONFIG AFTER INIT\r\n");

    PARA_DEBUG("\rFlash->Prepare sector [%d]...", xSystem.InternalFlash->GetSectorIndex(PARAMETERS_BASE));
    //return;
    if (xSystem.InternalFlash->Prepare(xSystem.InternalFlash->GetSectorIndex(PARAMETERS_BASE)))
    {
        xSystem.TimeOut.Clk1s_UpdateCFGToFlash = 5;
        PARA_DEBUG(" [FAIL]");
    }
    else
        PARA_DEBUG(" [OK]");

#if READ_WRITE_PARAMS_BATCH
    xSystem.InternalFlash->WriteBytes(PARAMETERS_BASE, (uint8_t *)&xSystem.Parameters, sizeof(Parameter_t));
    DebugPrint("WRITE CONFIG FINISH, Size:%d, Calib:%d Old:%04x, Calc:%04X, CRC16:%04X:%04X\r\n",
               sizeof(xSystem.Parameters), sizeof(xSystem.Parameters.AlMCalibData),
               oldCrc, crc, xSystem.Parameters.CRC16, *pCrc);
#else
    DebugPrint("WRITE CONFIG2, Size:%d, Calib:%d Old:%04x, Calc:%04X, CRC16:%04X:%04X\r\n", sizeof(xSystem.Parameters),
               sizeof(xSystem.Parameters.AlMCalibData), oldCrc, crc, xSystem.Parameters.CRC16, *pCrc);
    Al_CalibTable_t *AlMCalibData = &xSystem.Parameters.AlMCalibData;
    PARA_DEBUG("\rDeviceID:%s", xSystem.Parameters.DeviceID);
    xSystem.InternalFlash->WriteBytes(DEVICE_SERIAL_ADDR, xSystem.Parameters.DeviceID, DEVICE_SERIAL_LEN);
    PARA_DEBUG("\rNgayXuatXuong:%s", xSystem.Parameters.NgayXuatXuong);
    xSystem.InternalFlash->WriteBytes(DEVICE_PRODUCTDATE_ADDR, xSystem.Parameters.NgayXuatXuong, DEVICE_PRODUCTDATE_LEN);
    PARA_DEBUG("\rNgayHieuChinh:%s", xSystem.Parameters.NgayHieuChinh);
    xSystem.InternalFlash->WriteBytes(DEVICE_CALIBDATE_ADDR, xSystem.Parameters.NgayHieuChinh, DEVICE_CALIBDATE_LEN);
    PARA_DEBUG("\rNgayDP:%s", xSystem.Parameters.NgayDP);
    xSystem.InternalFlash->WriteBytes(DEVICE_RESDATE_ADDR, xSystem.Parameters.NgayDP, DEVICE_RESDATE_LEN);
    PARA_DEBUG("\rSetupKey:%s", xSystem.Parameters.SetupKey);
    xSystem.InternalFlash->WriteBytes(DEVICE_PASSKEY_ADDR, xSystem.Parameters.SetupKey, DEVICE_PASSKEY_LEN);

    PARA_DEBUG("\rBlowTime:%d", xSystem.Parameters.BlowTime);
    xSystem.InternalFlash->WriteLong(DEVICE_BLOWTIME_ADDR, xSystem.Parameters.BlowTime);
    PARA_DEBUG("\rBlowPressure:%d", xSystem.Parameters.BlowPressure);
    xSystem.InternalFlash->WriteLong(DEVICE_BLOWPRESS_ADDR, xSystem.Parameters.BlowPressure);
    PARA_DEBUG("\rTimeAutoTurnOff:%d", xSystem.Parameters.TimeAutoTurnOff);
    xSystem.InternalFlash->WriteLong(DEVICE_TIMEOFF_ADDR, xSystem.Parameters.TimeAutoTurnOff);
    PARA_DEBUG("\rBTimeoutKeypad:%d", xSystem.Parameters.TimeoutKeypad);
    xSystem.InternalFlash->WriteLong(DEVICE_KEYTO_ADDR, xSystem.Parameters.TimeoutKeypad);

    xSystem.InternalFlash->WriteBytes(DEVICE_ALM_CALIB_ADDR, (uint8_t *)AlMCalibData->b, DEVICE_ALM_CALIB_LEN);
#endif
    ReadConfig(0);
}

/*****************************************************************************/
/**
 * @brief   :
 * @param   :
 * @retval  :
 * @author  :   -------------
 * @created :   10/11/2014
 * @version :
 * @reviewer:
 */
uint8_t Param_Set_Cfg(uint8_t IDSCFG, uint8_t *Buffer, uint8_t ResponseRequire, char *BufferToResponse, uint8_t *ptrLenght)
{
    uint8_t i, NeedUpdateMemory = 1;
    uint8_t *BufferConfig = Buffer;
    uint16_t Value = 0;

    if (IDSCFG == 0)
        return 0;

    PARA_DEBUG("%s%u : %s", "\rThiet lap cau hinh: ", IDSCFG, BufferConfig);

    switch (IDSCFG)
    {

    default:
        break;
    }

    if (NeedUpdateMemory)
    {
        UpdateParametersToMemory();
    }

    return IDSCFG;
}

void ReloadCalibTable(void)
{
    Al_CalibTable_t *AlMCalibData = &xSystem.Parameters.AlMCalibData;
    ReadConfig(CFG_ALCalib_Table);
    PARA_DEBUG("NboValue: %d", AlMCalibData->s.Al_MaxValue);
}

void Parameter_Tick(void)
{
    //DebugPrint("Parameter_Tick CRC16:%04x, %d, Size:%d %d\r\n",
    //    xSystem.Parameters.CRC16, xSystem.TimeOut.Clk1s_UpdateCFGToFlash,
    //    sizeof(xSystem.Parameters), sizeof(xSystem.Parameters.AlMCalibData));
    if (xSystem.TimeOut.Clk1s_UpdateCFGToFlash > 0)
    {
        xSystem.TimeOut.Clk1s_UpdateCFGToFlash--;
        if (xSystem.TimeOut.Clk1s_UpdateCFGToFlash == 0)
            UpdateParametersToMemory();
    }
}

void SaveConfigToFlash()
{
    xSystem.TimeOut.Clk1s_UpdateCFGToFlash = 2;
}
